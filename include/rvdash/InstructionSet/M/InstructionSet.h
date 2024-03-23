#ifndef M_INSTRUCTION_SET_H
#define M_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace M {

//---------------------------------MInstrExecutor----------------------------------------

class MInstrExecutor {

public:
  MInstrExecutor(){};

  template <typename InstrSetType>
  void execute(Instruction Instr, ExecuteFuncType<InstrSetType> Func,
               InstrSetType &Set) {
    Set.LogFile << "M execute: ";

    Func(Instr, Set);
    Instr.dump(Set.LogFile);
    Set.LogFile << "\n";
  }

  template <typename InstrSetType>
  static void executeMUL(Instruction Instr, InstrSetType &Set) {
    auto LogFile = Set.getLogFile();
    LogFile << "Execute MUL\n\n";
  }
};

//---------------------------------MInstrDecoder-----------------------------------------

class MInstrDecoder {
public:
  MInstrDecoder(){};
  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, ExecuteFuncType<InstrSetType>>>
  tryDecode(Register<Instruction::Sz> Instr) {
    return std::nullopt;
  }
};

//-----------------------------------MInstrSet-------------------------------------------

class MInstrSet {
protected:
  MInstrDecoder Decoder;
  MInstrExecutor Executor;

public:
  MInstrSet() : Executor(){};

  ~MInstrSet(){};

  void dump(std::ostream &Stream) const { Stream << "MInstrSet\n"; }
  void print() const { dump(std::cout); }

  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, ExecuteFuncType<InstrSetType>>>
  tryDecode(Register<Instruction::Sz> Instr, InstrSetType &Set) {
    return Decoder.tryDecode<InstrSetType>(Instr);
  }

  template <typename InstrSetType>
  bool tryExecute(Instruction Instr, ExecuteFuncType<InstrSetType> Funct,
                  InstrSetType &Set) {
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

