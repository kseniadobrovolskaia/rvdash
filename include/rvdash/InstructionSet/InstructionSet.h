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

template <typename Set>
bool isBaseSet(Set S) {
  return false;
}

template <size_t Sz, typename Set>
std::optional<Register<Sz>*> findPC(Set S) {
  return std::nullopt;
}

template <size_t Sz>
std::optional<Register<Sz>*> operator||(std::optional<Register<Sz>*> Lhs, std::optional<Register<Sz>*> Rhs) {
  return Lhs.has_value() ? Lhs : Rhs;
}

// Sz means size of program counter register PC
template <size_t Sz, typename... Exts>
class InstrSet : private Exts... {

protected:
  Register<Sz> *PC;

public:
 
  InstrSet() : Exts()... { extractPC(); };

  void extractPC() {
    auto OptPC = (findPC<Sz>(static_cast<const Exts&>(*this)) || ...);
    if (!OptPC.has_value())
      throw std::logic_error("No extension returned the program counter");
    PC = OptPC.value();
  } 
  
  void print() const {     
    std::cout << "Instruction Set:\n";
    (std::cout << ... << static_cast<const Exts&>(*this)) << "\n";
  }

  void execute(std::shared_ptr<Instruction<Sz>> &Instr) const {
    auto Result = (static_cast<const Exts&>(*this).tryExecute(Instr) && ...);
    if (Result)
      throw std::logic_error("Fail execution");
  }

  Register<Sz> getPC() const { return *PC; }
  
  bool isThereBase() {
    return (isBaseSet(static_cast<const Exts&>(*this)) || ...);
  }

  
  std::shared_ptr<Instruction<Sz>> decode(Register<Sz> Instr) const {
    auto Result = (static_cast<const Exts&>(*this).tryDecode(Instr) || ...);
    if (!Result.has_value())
      throw std::logic_error("Illegal instruction");
    std::cout << "Result of Decode :\n" << *Result.value() << "\n";
    return Result.value();
  }
};

//-------------------------------------CPU------------------------------------------------

template <size_t Sz, typename... Exts>
class CPU {

private:
  InstrSet<Sz, Exts...> ExtSet;

public:

  void add(const InstrSet<Sz, Exts...> &E) { ExtSet = E; }
  void print()
  {
      ExtSet.print();
  }

  void execute(const std::vector<Register<Sz>> &Programm) const {
    for (const auto& Cmd : Programm) {
      auto Instr = ExtSet.decode(Cmd);
      ExtSet.execute(Instr);
    }
  }

  std::shared_ptr<Instruction<Sz>> decode(Register<Sz> Instr) const {
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

