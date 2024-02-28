#ifndef R32I_INSTRUCTION_SET_H
#define R32I_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace RV32I {

const size_t RV32ISz = 32;
using Register = Register<RV32ISz>;

//--------------------------------RV32IInstrDecoder--------------------------------------

class RV32IInstrDecoder {
public:
  RV32IInstrDecoder() {};
  std::optional<std::shared_ptr<rvdash::Instruction<RV32ISz>>> tryDecode(Register Instr) const; 
};

//--------------------------------RV32IInstrExecutor-------------------------------------

class RV32IInstrExecutor {

public:
    RV32IInstrExecutor() {};

    void execute(std::shared_ptr<Instruction<RV32ISz>> Instr) const;

};

//----------------------------------RV32IInstrSet----------------------------------------

class RV32IInstrSet {
protected:
  RV32IInstrDecoder Decoder;
  RV32IInstrExecutor Executor;
  RegistersSet<RV32ISz> Registers;

public:
  RV32IInstrSet() : Registers(X0, X31 - X0) {
    Registers.addNamedRegister("pc");
    ++Registers.getNamedRegister("pc");
  }

  ~RV32IInstrSet() {};

  Register *getPC() { return &Registers.getNamedRegister("pc"); }

  void print() const { std::cout << "RV32IInstrSet\n"; }

  std::optional<std::shared_ptr<Instruction<RV32ISz>>> tryDecode(Register Instr) const {
    return Decoder.tryDecode(Instr);
  }

  bool tryExecute(std::shared_ptr<Instruction<RV32ISz>> Instr) const {
    if (Instr->getExtension() != Extensions::RV32I)
      return true;
    Executor.execute(Instr); 
    return false;
  }

};


std::ostream& operator<<(std::ostream& stream, const typename RV32I::RV32IInstrSet& counter);

} // namespace RV32I

template <>
bool isBaseSet<RV32I::RV32IInstrSet>(RV32I::RV32IInstrSet S);

template <>
std::optional<Register<RV32I::RV32ISz>*> findPC<RV32I::RV32ISz, RV32I::RV32IInstrSet>(RV32I::RV32IInstrSet S);

} // namespace rvdash

#endif// R32I_INSTRUCTION_SET_H

