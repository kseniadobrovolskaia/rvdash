#ifndef F_INSTRUCTION_SET_H
#define F_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace F {

const size_t RV32ISz = 32;
using Register = std::bitset<RV32ISz>;

//---------------------------------FInstrDecoder-----------------------------------------

class FInstrDecoder {
public:
  FInstrDecoder() {};
  std::optional<std::shared_ptr<rvdash::Instruction<RV32ISz>>> tryDecode(Register Instr) const;

};

//---------------------------------FInstrExecutor----------------------------------------

class FInstrExecutor {

public:
    FInstrExecutor() {};
    void execute(const Instruction<RV32ISz> &Instr) const {
      std::cout << "execute\n"; 
    }
};

//-----------------------------------FInstrSet-------------------------------------------

class FInstrSet {
protected:
  FInstrDecoder Decoder;
  FInstrExecutor Executor;
  RegistersSet<RV32ISz> Registers;

public:
  FInstrSet() : Registers(32) {
    Registers.addNamedRegister("pc");
    Registers.getNamedRegister("pc") = 0;
  }

  ~FInstrSet() {};

  void print() const { std::cout << "FInstrSet\n"; }

  std::optional<std::shared_ptr<Instruction<RV32ISz>>> tryDecode(Register Instr) const {
    return Decoder.tryDecode(Instr);
  }

};

std::ostream& operator<<(std::ostream& stream, const typename F::FInstrSet& counter);

} // namespace F
} // namespace rvdash

#endif// F_INSTRUCTION_SET_H

