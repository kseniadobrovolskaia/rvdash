#ifndef M_INSTRUCTION_SET_H
#define M_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace M {

template <typename InstrSetType>
using FunctType = void (*)(Instruction, const InstrSetType &Set);

//-----------------------------extern_M::Instructions------------------------------------

// extern Instruction MUL;
// extern Instruction MULH;
// extern Instruction MULSU;
// extern Instruction MULU;
// extern Instruction DIV;
// extern Instruction DIVU;
// extern Instruction REM;
// extern Instruction REMU;

//---------------------------------MInstrExecutor----------------------------------------

class MInstrExecutor {
public:
  MInstrExecutor(){};

  template <typename InstrSetType>
  void execute(Instruction Instr, FunctType<InstrSetType> Func,
               const InstrSetType &Set) const {
    std::cout << "M execute: ";

    Func(Instr, Set);
    Instr.print();
    std::cout << "\n";
  }

  template <typename InstrSetType>
  static void executeMUL(Instruction Instr, const InstrSetType &Set) {
    std::cout << "Execute MUL\n\n";
  }
};

//---------------------------------MInstrDecoder-----------------------------------------

class MInstrDecoder {
public:
  MInstrDecoder(){};
  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, FunctType<InstrSetType>>>
  tryDecode(Register<Instruction::Sz> Instr) const {
    return std::nullopt;
  }
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

  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, FunctType<InstrSetType>>>
  tryDecode(Register<Instruction::Sz> Instr, const InstrSetType &Set) const {
    return Decoder.tryDecode<InstrSetType>(Instr);
  }

  template <typename InstrSetType>
  bool tryExecute(Instruction Instr, FunctType<InstrSetType> Funct,
                  const InstrSetType &Set) const {
    if (Instr.Ex != Extensions::M)
      return true;
    Executor.execute(Instr, Funct, Set);
    return false;
  }
};

std::ostream& operator<<(std::ostream& Stream, const typename M::MInstrSet& Set);

} // namespace M
} // namespace rvdash

#endif// M_INSTRUCTION_SET_H

