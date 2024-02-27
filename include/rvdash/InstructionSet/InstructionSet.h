#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include "rvdash/InstructionSet/Instruction.h"
#include "rvdash/InstructionSet/Registers.h"




namespace rvdash {

//------------------------------------Extensions-----------------------------------------

enum class Extensions {
  RV32I,
  M,
  A,
  F,
  D,
};

//------------------------------------InstrSet-------------------------------------------

template <typename... Types>
void printEx(Types const& ... args) {
  (std::cout << ... << args) << "\n";
}

template <typename Set>
bool isBaseSet(Set S) {
  return false;
}

template <size_t Sz, typename Set>
std::optional<Register<Sz>*> getPC(Set S) {
  return std::nullopt;
}

template <size_t Sz>
std::optional<Register<Sz>*> operator||(std::optional<Register<Sz>*> Lhs, std::optional<Register<Sz>*> Rhs) {
  return Lhs.has_value() ? Lhs : Rhs;
}


template <size_t Sz, typename... Exts>
class InstrSet : private Exts... {
protected:
  Register<Sz> *PC;

public:
  InstrSet() : Exts()... {};

  void print() const {     
    std::cout << "Instruction Set:\n";
    printEx(static_cast<const Exts&>(*this)...);
  }

  void execute(const Instruction<Sz> &Instr) const {
    
  }
  
  bool isThereBase() {
    return (isBaseSet(static_cast<const Exts&>(*this)) || ...);
  }

  void extractPC() {
    auto OptPC = (getPC<32>(static_cast<const Exts&>(*this)) || ...);
    assert(OptPC.has_value());
    PC = OptPC.value();
    std::cout << "PC = " << *PC << "\n" ;
  }
  
  std::optional<std::shared_ptr<Instruction<Sz>>> tryDecode(Register<Sz> Instr) const {
    auto Result = (static_cast<const Exts&>(*this).tryDecode(Instr) || ...);
    assert(Result.has_value());
    std::cout << "Result of Decode :\n" << *Result.value() << "\n";
    return Result;
  }
};

//-------------------------------------CPU------------------------------------------------

template <size_t Sz, typename... Exts>
class CPU {
private:
  unsigned PC = 0;
  InstrSet<Sz, Exts...> ExtSet;

public:

  void add(const InstrSet<Sz, Exts...> &E) { ExtSet = E; }
  void print()
  {
      ExtSet.print();
  }

  std::optional<std::shared_ptr<Instruction<Sz>>> tryDecode(Register<Sz> Instr) const {
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

