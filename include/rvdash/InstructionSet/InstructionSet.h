#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <type_traits>

#include "rvdash/InstructionSet/Instruction.h"
#include "rvdash/InstructionSet/Registers.h"
#include "rvdash/Memory/Memory.h"




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
std::optional<Register<Sz>*> operator||(std::optional<Register<Sz>*> Lhs, std::optional<Register<Sz>*> Rhs) {
  if (Lhs.has_value() && Rhs.has_value())
    failWithError("More than one set of instructions was able to get PC");
  return Lhs.has_value() ? Lhs : Rhs;
}

//------------------------------------InstrSet-------------------------------------------

// AddrSz means size of address space and program counter register PC
template <size_t AddrSz, typename... Exts>
class InstrSet : private Exts... {

protected:
  Register<AddrSz> *PC;

public:
  InstrSet() : Exts()... {
    if (!isThereBase())
      failWithError("Basic set must be specified");
    PC = extractPC();
  };

  auto extractPC() {
    auto OptPC = (getPC<AddrSz>(static_cast<Exts *>(this)) || ...);
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

  void execute(std::shared_ptr<Instruction> &Instr) const {
    auto Result = (static_cast<const Exts&>(*this).tryExecute(Instr) && ...);
    if (Result)
      failWithError("Fail execution");
  }

  Register<AddrSz> getProgramCounter() const { return *PC; }

  bool isThereBase() const {
    return (isBaseSet(static_cast<const Exts&>(*this)) || ...);
  }

  std::shared_ptr<Instruction> decode(Register<Instruction::Sz> Instr) const {
    auto Result = (static_cast<const Exts&>(*this).tryDecode(Instr) || ...);
    if (!Result.has_value())
      failWithError("Illegal instruction");
    std::cout << "Result of Decode :\n" << *Result.value() << "\n";
    return Result.value();
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

  void print() const {
      ExtSet.print();
  }

  void execute(const std::vector<Register<Instruction::Sz>> &Program) const {
    for (const auto& Cmd : Program) {
      auto Instr = ExtSet.decode(Cmd);
      ExtSet.execute(Instr);
    }
  }

  std::shared_ptr<Instruction> decode(Register<Instruction::Sz> Instr) const {
    return ExtSet.tryDecode(Instr);
  }

  bool isThereBase() {
    return ExtSet.isThereBase();
  }
};

} // namespace rvdash

#endif // INSTRUCTION_SET_H

