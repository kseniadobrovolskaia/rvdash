#include "rvdash/InstructionSet/M/InstructionSet.h"




namespace rvdash {
namespace M {

enum class Opcode {
  MUL,
  MULH,
  MULSU,
  MULU,
  DIV,
  DIVU,
  REM,
  REMU
};

rvdash::R_Instruction   MUL(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /* Funct7 */ 0b000'0001, Extensions::M);
rvdash::R_Instruction  MULH(/* Opcode */ 0b011'0011, /* Funct3 */ 0b001, /* Funct7 */ 0b000'0001, Extensions::M);
rvdash::R_Instruction MULSU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b010, /* Funct7 */ 0b000'0001, Extensions::M);
rvdash::R_Instruction  MULU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b011, /* Funct7 */ 0b000'0001, Extensions::M);
rvdash::R_Instruction   DIV(/* Opcode */ 0b011'0011, /* Funct3 */ 0b100, /* Funct7 */ 0b000'0001, Extensions::M);
rvdash::R_Instruction  DIVU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b101, /* Funct7 */ 0b000'0001, Extensions::M);
rvdash::R_Instruction   REM(/* Opcode */ 0b011'0011, /* Funct3 */ 0b110, /* Funct7 */ 0b000'0001, Extensions::M);
rvdash::R_Instruction  REMU(/* Opcode */ 0b011'0001, /* Funct3 */ 0b111, /* Funct7 */ 0b000'0001, Extensions::M);

//-----------------------------------MInstrDecoder---------------------------------------

std::optional<std::shared_ptr<rvdash::Instruction>> MInstrDecoder::tryDecode(Register<Instruction::Sz> Instr) const {
  auto Opcode = rvdash::Instruction::extractOpcode(Instr);
  auto Funct3 = rvdash::Instruction::extractFunct3(Instr);
  auto Funct7 = rvdash::Instruction::extractFunct7(Instr);
  if ((Opcode == 0b000'0000) && (Funct3 == 0b000) && (Funct7 == 0b000'0000))
    return std::make_shared<rvdash::R_Instruction>(MUL, Instr);
  return std::nullopt;
}

//---------------------------------MInstrExecutor----------------------------------------

void MInstrExecutor::execute(std::shared_ptr<Instruction> Instr) const {
  std::cout << "M execute: ";
  Instr->print();
  std::cout << "\n"; 
}

//-----------------------------------MInstrSet-------------------------------------------


std::ostream& operator<<(std::ostream& Stream, const typename M::MInstrSet& Set)
{
    Set.print();
    return Stream;
}

} // namespace M
} // namespace rvdash

