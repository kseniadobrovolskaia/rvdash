#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <type_traits>
#include <variant>

#include "rvdash/InstructionSet/Instruction.h"
#include "rvdash/InstructionSet/Registers.h"




namespace rvdash {

namespace RV32I {
extern Instruction EBREAK;
}

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

//Is it possible to identify a basic set by the presence of PC in it?
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

// AddrSz means size of address space and program counter register PC
template <typename MemoryType, size_t AddrSz, typename... Exts>
class InstrSet : private Exts... {

protected:
  Register<AddrSz> *PC;
  MemoryType &Memory;

public:
  using ExecuteFunctType = void (*)(Instruction, const InstrSet &Set);

  InstrSet(MemoryType &Mem) : Exts()..., Memory(Mem) {
    if (!isThereBase())
      failWithError("One base set must be selected");
    PC = extractPC();
  };

  auto extractPC() {
    auto OptPC = (getPC<AddrSz>(static_cast<Exts *>(this)) ^ ...);
    if (!OptPC.has_value())
      failWithError("No extension returned the program counter");
    if (OptPC.value()->size() != AddrSz)
      failWithError(
          "The program counter size does not match the address space size");

    return OptPC.value();
  }

  void print() const {
    std::cout << "Instruction Set:\n";
    (std::cout << ... << static_cast<const Exts&>(*this)) << "\n";
  }

  Register<AddrSz> getProgramCounter() const { return *PC; }

  MemoryType &getMemory() const { return Memory; }

  bool isThereBase() const {
    return (isBaseSet(static_cast<const Exts&>(*this)) ^ ...);
  }

  std::tuple<Instruction, ExecuteFunctType>
  decode(Register<Instruction::Sz> Instr) const {
    auto Result = (static_cast<const Exts&>(*this).tryDecode(Instr, *this) ^ ...);
    if (!Result.has_value())
      failWithError("Illegal instruction");
    return Result.value();
  }

  void executeProgram(unsigned long long PcValue, MemoryType &Mem) const {
    *PC = PcValue;
    bool Run = true;
    Register<Instruction::Sz> Cmd;
    // Machine cycle
    while (Run) { 
      // Fetch
      Mem.load(PC->to_ulong(), Instruction::Sz, Cmd);
      if (isSame(Cmd, RV32I::EBREAK.Bits, /* Mask = all_ones */ -1))
        return;
      ++*PC;
      // Decode
      auto [Instr, Func] = decode(Cmd);
      // Execute
      execute(Instr, Func);
    }
  }

  template <typename Variant>
  void execute(Instruction &Instr, Variant Functs) const {
    auto Result =
        (static_cast<const Exts &>(*this).tryExecute(Instr, Functs, *this) && ...);
    if (Result)
      failWithError("Fail execution");
  }
};

//-------------------------------------CPU------------------------------------------------

template <typename MemoryType, typename InstrSetType,
          size_t AddrSz = MemoryType::getAddrSpaceSz()>
class CPU {

private:
  MemoryType &VirtualMemory;
  InstrSetType ExtSet;

public:
  CPU(MemoryType &Mem, const InstrSetType &E)
      : VirtualMemory(Mem), ExtSet(std::move(E)) {
    if (AddrSz > MemoryType::getAddrSpaceSz())
      failWithError("The requested address space size exceeds the size that "
                    "virtual memory has");
  };

  void print() const { ExtSet.print(); }

  void execute(unsigned long long Pc, const std::vector<Register<Instruction::Sz>> &Program) const {
    if (Pc % AddrSz != 0)
      failWithError("Unaligned PC start address");
    unsigned long long NumStore = 0;
    for (const auto &Cmd : Program) {
      VirtualMemory.store(NumStore, AddrSz, Cmd);
      NumStore += AddrSz;
    }
    std::cout << "====================Simulation started====================\n";
    ExtSet.executeProgram(Pc, VirtualMemory);
    std::cout << "====================Simulation stopped====================\n";
  }

  std::tuple<Instruction, typename InstrSetType::ExecuteFunctType>
  decode(Register<Instruction::Sz> Instr) const {
    return ExtSet.tryDecode(Instr);
  }

  bool isThereBase() {
    return ExtSet.isThereBase();
  }
};

} // namespace rvdash
#endif // INSTRUCTION_SET_H

