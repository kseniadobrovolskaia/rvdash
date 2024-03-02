#ifndef M_INSTRUCTION_SET_H
#define M_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace M {

//---------------------------------MInstrDecoder-----------------------------------------

class MInstrDecoder {
public:
  MInstrDecoder() {};
  std::optional<std::shared_ptr<rvdash::Instruction>> tryDecode(Register<Instruction::Sz> Instr) const;

};

//---------------------------------MInstrExecutor----------------------------------------

class MInstrExecutor {

public:
    MInstrExecutor() {};
    void execute(std::shared_ptr<Instruction> Instr) const;
};

//-----------------------------------MInstrSet-------------------------------------------

class MInstrSet {
protected:
  MInstrDecoder Decoder;
  MInstrExecutor Executor;

public:
  MInstrSet() {};

  virtual ~MInstrSet() {};

  void print() const { std::cout << "MInstrSet\n"; }

  std::optional<std::shared_ptr<Instruction>> tryDecode(Register<Instruction::Sz> Instr) const {
    return Decoder.tryDecode(Instr);
  }

  bool tryExecute(std::shared_ptr<Instruction> Instr) const {
    if (Instr->getExtension() != Extensions::M)
      return true;
    Executor.execute(Instr); 
    return false;
  }



};

std::ostream& operator<<(std::ostream& Stream, const typename M::MInstrSet& Set);

} // namespace M
} // namespace rvdash

#endif// M_INSTRUCTION_SET_H

