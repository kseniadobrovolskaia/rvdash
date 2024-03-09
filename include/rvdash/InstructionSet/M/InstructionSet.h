#ifndef M_INSTRUCTION_SET_H
#define M_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace M {

//-----------------------------extern
//M::Instructions------------------------------------

extern R_Instruction MUL;
extern R_Instruction MULH;
extern R_Instruction MULSU;
extern R_Instruction MULU;
extern R_Instruction DIV;
extern R_Instruction DIVU;
extern R_Instruction REM;
extern R_Instruction REMU;

//---------------------------------MInstrExecutor----------------------------------------

class MInstrExecutor {
public:
  MInstrExecutor(){};
  void execute(std::shared_ptr<Instruction> Instr,
               Instruction::FuncExecutor_t Func) const {
    std::cout << "M execute: ";
    Func(Instr);
    Instr->print();
    std::cout << "\n";
  }

  static void executeMUL(std::shared_ptr<Instruction> Instr) {
    std::cout << "Execute MUL\n\n";
  };
};

//---------------------------------MInstrDecoder-----------------------------------------

class MInstrDecoder {
public:
  MInstrDecoder(){};
  std::optional<std::tuple<std::shared_ptr<rvdash::Instruction>,
                           Instruction::FuncExecutor_t>>
  tryDecode(Register<Instruction::Sz> Instr) const {
    auto Opcode = rvdash::Instruction::extractOpcode(Instr);
    auto Funct3 = rvdash::Instruction::extractFunct3(Instr);
    auto Funct7 = rvdash::Instruction::extractFunct7(Instr);
    if ((Opcode == 0b000'0000) && (Funct3 == 0b000) && (Funct7 == 0b000'0000))
      return std::tuple{std::make_shared<rvdash::R_Instruction>(MUL, Instr),
                        &MInstrExecutor::executeMUL};
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

  std::optional<
      std::tuple<std::shared_ptr<Instruction>, Instruction::FuncExecutor_t>>
  tryDecode(Register<Instruction::Sz> Instr) const {
    return Decoder.tryDecode(Instr);
  }

  bool tryExecute(std::shared_ptr<Instruction> Instr,
                  Instruction::FuncExecutor_t Func) const {
    if (Instr->getExtension() != Extensions::M)
      return true;
    Executor.execute(Instr, Func);
    return false;
  }
};

std::ostream& operator<<(std::ostream& Stream, const typename M::MInstrSet& Set);

} // namespace M
} // namespace rvdash

#endif// M_INSTRUCTION_SET_H

