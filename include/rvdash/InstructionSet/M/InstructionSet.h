#ifndef M_INSTRUCTION_SET_H
#define M_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace M {

const size_t RV32ISz = 32;
using Register = std::bitset<RV32ISz>;

//---------------------------------MInstrDecoder-----------------------------------------

class MInstrDecoder {
public:
  MInstrDecoder() {};
  std::optional<std::shared_ptr<rvdash::Instruction<RV32ISz>>> tryDecode(Register Instr) const;

};

//---------------------------------MInstrExecutor----------------------------------------

class MInstrExecutor {

public:
    MInstrExecutor() {};
    void execute(const Instruction<RV32ISz> &Instr) const {
      std::cout << "execute\n"; 
    }
};

//-----------------------------------MInstrSet-------------------------------------------

class MInstrSet {
protected:
  MInstrDecoder Decoder;
  MInstrExecutor Executor;

public:
  MInstrSet() {};

  ~MInstrSet() {};

  void print() const { std::cout << "MInstrSet\n"; }

  std::optional<std::shared_ptr<Instruction<RV32ISz>>> tryDecode(Register Instr) const {
    return Decoder.tryDecode(Instr);
  }

};

std::ostream& operator<<(std::ostream& Stream, const typename M::MInstrSet& Set);

} // namespace M
} // namespace rvdash

#endif// M_INSTRUCTION_SET_H

