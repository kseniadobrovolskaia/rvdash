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

template<typename ...>
using Void_t = void;

template<typename T, typename = Void_t<>>
struct HasPc : std::false_type {};

template<typename T>
struct HasPc<T, Void_t<typename T::PcSz_t>> : std::true_type {};

//Is it possible to identify a basic set by the presence of PC in it?
template <typename Set>
bool isBaseSet(Set S) {
  if constexpr (HasPc<Set>::value)
    return true;
  return false;
}

template <size_t PcSz, typename Set>
std::optional<Register<PcSz>*> findPC(Set S) {
  if constexpr (HasPc<Set>::value)
    return S.getPC();
  return std::nullopt;
}

template <size_t Sz>
std::optional<Register<Sz>*> operator||(std::optional<Register<Sz>*> Lhs, std::optional<Register<Sz>*> Rhs) {
  if (Lhs.has_value() && Rhs.has_value())
    throw std::logic_error("More than one set of instructions was able to get PC");
  return Lhs.has_value() ? Lhs : Rhs;
}

//------------------------------------InstrSet-------------------------------------------

// AddrSz means size of address space and program counter register PC
template <size_t AddrSz, typename... Exts>
class InstrSet : private Exts... {

protected:
  Register<AddrSz> *PC;
  Memory<AddrSz> &VirtualMemory;

public:
 
  InstrSet(Memory<AddrSz> &Mem) : Exts()... , VirtualMemory(Mem) /*, PC(extractPC())*/ {};

  void extractPC() {
    auto OptPC = (findPC<AddrSz>(static_cast<const Exts&>(*this)) || ...);
    if (!OptPC.has_value())
      throw std::logic_error("No extension returned the program counter");
    PC = OptPC.value();
  } 
  
  void print() const {
    std::cout << "Instruction Set:\n";
    (std::cout << ... << static_cast<const Exts&>(*this)) << "\n";
  }

  void execute(std::shared_ptr<Instruction> &Instr) const {
    auto Result = (static_cast<const Exts&>(*this).tryExecute(Instr) && ...);
    if (Result)
      throw std::logic_error("Fail execution");
  }

  Register<AddrSz> getPC() const { return *PC; }
  
  bool isThereBase() const {
    return (isBaseSet(static_cast<const Exts&>(*this)) || ...);
  }

  std::shared_ptr<Instruction> decode(Register<Instruction::Sz> Instr) const {
    auto Result = (static_cast<const Exts&>(*this).tryDecode(Instr) || ...);
    if (!Result.has_value())
      throw std::logic_error("Illegal instruction");
    std::cout << "Result of Decode :\n" << *Result.value() << "\n";
    return Result.value();
  }
};

//-------------------------------------CPU------------------------------------------------

template <size_t AddrSz, typename... Exts>
class CPU {

private:
  InstrSet<AddrSz, Exts...> ExtSet;

public:

  CPU(const InstrSet<AddrSz, Exts...> &E) : ExtSet(std::move(E)) {};
  
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

  void extractPC() {
    ExtSet.extractPC();
  }

};

} // namespace rvdash

#endif // INSTRUCTION_SET_H

