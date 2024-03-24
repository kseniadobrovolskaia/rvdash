#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <type_traits>
#include <variant>

#include "rvdash/InstructionSet/Instruction.h"
#include "rvdash/InstructionSet/Registers.h"

#define DEBUG
#undef DEBUG

namespace rvdash {

//------------------------------------Extensions-----------------------------------------

enum class Extensions {
  RV32I,
  M,
  A,
  F,
  D,
};

//------------------------------------ExtractPC------------------------------------------

template <typename Set>
concept HasPc = requires(Set *S) {
  S->getPC();
};

template <typename Set> bool isBaseSet(Set) {
  if constexpr (HasPc<Set>)
    return true;
  return false;
}

template <size_t PcSz, typename Set>
std::optional<Register<PcSz> *> getPC(Set *S) {
  if constexpr (HasPc<Set>)
    return S->getPC();
  return std::nullopt;
}

template <size_t Sz>
std::optional<Register<Sz>*> operator^(std::optional<Register<Sz>*> Lhs, std::optional<Register<Sz>*> Rhs) {
  if (Lhs.has_value() && Rhs.has_value())
    failWithError("More than one set of instructions was able to get PC");
  return Lhs.has_value() ? Lhs : Rhs;
}

template <typename T1>
std::optional<std::tuple<Instruction, T1>>
operator^(std::optional<std::tuple<Instruction, T1>> Lhs,
          std::optional<std::tuple<Instruction, T1>> Rhs) {
  if (Lhs.has_value() && Rhs.has_value())
    failWithError(
        "More than one set of instructions was able to decode Instruction");
  if (Lhs.has_value())
    return Lhs;
  return Rhs;
}

//------------------------------------InstrSet-------------------------------------------

template <typename InstrSetType>
using ExecuteFuncType = void (*)(Instruction, InstrSetType &Set);

/**
 * @brief class InstrSet - main part of CPU, contains (private inheritance) all
 *                         possible extensions (the basic set too). AddrSz
 *                         means size of address space and program counter
 *                         register PC.
 */
template <typename MemoryType, size_t AddrSz, typename... Exts>
class InstrSet : private Exts... {

protected:
  Register<AddrSz> *PC;
  MemoryType &Memory;

public:
  volatile bool Stop = false;
  std::ostream &LogFile;

  using ExecuteFuncT = ExecuteFuncType<InstrSet>;

  InstrSet(MemoryType &Mem, std::ostream &File = std::cout)
      : Exts()..., Memory(Mem), LogFile(File) {
    if (!isThereBase())
      failWithError("One base set must be selected");
    PC = extractPC();
  }

  /**
   * @brief stop - function to stop execution of the machine cycle.
   */
  void stop() { Stop = true; }

  /**
   * @brief extractPC - function to find the basic set and get the program
   *                    counter using the concept HasPC
   */
  auto extractPC() {
    auto OptPC = (getPC<AddrSz>(static_cast<Exts *>(this)) ^ ...);
    if (!OptPC.has_value())
      failWithError("No extension returned the program counter");
    if (OptPC.value()->size() != AddrSz)
      failWithError(
          "The program counter size does not match the address space size");

    return OptPC.value();
  }

  void dump() const { dump(LogFile); }
  void dump(std::ostream &Stream) const {
    Stream << "Instruction Set consist:\n";
    (Stream << ... << static_cast<const Exts &>(*this)) << "\n";
  }
  void print() const { dump(std::cout); }

  Register<AddrSz> getProgramCounter() const { return *PC; }
  std::ostream &getLogFile() { return LogFile; }
  MemoryType &getMemory() const { return Memory; }

  /**
   * @brief isThereBase - function to identify a basic set by the presence of PC
   *                      in it. Is it possible?
   */
  bool isThereBase() const {
    return (isBaseSet(static_cast<const Exts&>(*this)) ^ ...);
  }

  /**
   * @brief decode - function to decoding Instr,
   *                 using sequential substitution of all extensions.
   *                 When the extension has identified its instruction, it
   *                 returns Instruction and a pointer to the function to
   *                 execute. If the given instruction does not belong to
   *                 this extension, then it returns std::nullopt and this
   *                 nullopt is ignored by the overloaded ^ operator.
   */
  std::tuple<Instruction, ExecuteFuncT>
  decode(Register<Instruction::Sz> Instr) {
    auto Result = (static_cast<Exts &>(*this).tryDecode(Instr, *this) ^ ...);
    if (!Result.has_value())
      failWithError("Illegal instruction: " + Instr.to_string());
    return Result.value();
  }

  /**
   * @brief execute - function to execution Instr. Its idea is the same as in
   *                  the decoding function. It quickly realizes that the
   *                  instruction does not belong to this extension using the
   *                  field Instruction::Ex (Extentions).
   */
  template <typename Variant> void execute(Instruction &Instr, Variant Functs) {
    auto Result =
        (static_cast<Exts &>(*this).tryExecute(Instr, Functs, *this) && ...);
    if (Result)
      failWithError("Fail execution");
  }

  /**
   * @brief executeProgram - function to excution of the main machine cycle,
   *                         using sequential substitution of its extensions.
   *                         When the extension has identified its instruction,
   *                         it returns Instr and a pointer to the function to
   *                         execute (Func). If the given instruction does not
   *                         belong to this extension, then it returns
   *                         std::nullopt.
   */
  void executeProgram(unsigned long long PcValue) {
    setPC(PcValue);
    // Machine cycle
    do {
      step();
      increasePC();
    } while (!Stop);
  }

  /**
   * @brief step - function for one step of the machine cycle.
   */
  void step() {
    if (Stop)
      failWithError("Step is impossible");
    Register<Instruction::Sz> Cmd;
    // Fetch
    Memory.load(PC->to_ulong(), /* Size */ Instruction::Sz / CHAR_BIT, Cmd);
    // Decode
    auto [Instr, Func] = decode(Cmd);
    // Execute
    execute(Instr, Func);
  }

  void increasePC() const { ++*PC; }
  void setPC(unsigned long long PcValue) const { *PC = PcValue; }
};

//-------------------------------------CPU------------------------------------------------

/**
 * @brief class CPU - a class that controls the simulation process from writing
 *                    a program into virtual memory to executing instructions
 *                    (this is the model).
 */
template <typename MemoryType, typename InstrSetType,
          size_t AddrSz = MemoryType::getAddrSpaceSz()>
class CPU {

private:
  MemoryType &VirtualMemory;
  InstrSetType &ExtSet;

  std::ostream &LogFile;

public:
  CPU(MemoryType &Mem, InstrSetType &E)
      : VirtualMemory(Mem), ExtSet(E), LogFile(E.LogFile) {
    if (AddrSz > MemoryType::getAddrSpaceSz())
      failWithError("The requested address space size exceeds the size that "
                    "virtual memory has");
  }

  void dump() const { dump(LogFile); }
  void dump(std::ostream &Stream) const {
    Stream << "\nCPU:\n";
    ExtSet.dump(Stream);
    std::ofstream File("Mem.dump");
    VirtualMemory.dump(File);
    Stream << "Virtual memory dump in file Mem.dump\n";
  }
  void print() const { dump(std::cout); }

  void
  storeProgramInVirtualMemory(const std::vector<Register<CHAR_BIT>> &Program) {
    unsigned long long NumStore = 0;
    for (const auto &Byte : Program) {
      VirtualMemory.store(NumStore, /* Size */ 1, Byte);
      NumStore++;
    }
  }

  void execute(unsigned long long Pc,
               const std::vector<Register<CHAR_BIT>> &Program) {
    if (Pc % AddrSz != 0)
      failWithError("Unaligned PC start address");

    storeProgramInVirtualMemory(Program);
#ifdef DEBUG
    std::ofstream File("Mem_debug.dump");
    VirtualMemory.dump(File);
#endif
    LogFile << "====================Simulation started====================\n";
    ExtSet.executeProgram(Pc);
    LogFile << "===================Simulation completed===================\n";
  }

  void step() const { ExtSet.step(); }
  void increasePC() const { ExtSet.increasePC(); }
  void setPC(unsigned long long PcValue) const { ExtSet.setPC(PcValue); }
};

} // namespace rvdash
#endif // INSTRUCTION_SET_H

