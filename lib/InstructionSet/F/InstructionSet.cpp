#include "rvdash/InstructionSet/F/InstructionSet.h"




namespace rvdash {
namespace F {

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

rvdash::R_Instruction<RV32ISz>   MUL(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /* Funct7 */ 0b000'0001, Extensions::F);
rvdash::R_Instruction<RV32ISz>  MULH(/* Opcode */ 0b011'0011, /* Funct3 */ 0b001, /* Funct7 */ 0b000'0001, Extensions::F);
rvdash::R_Instruction<RV32ISz> MULSU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b010, /* Funct7 */ 0b000'0001, Extensions::F);
rvdash::R_Instruction<RV32ISz>  MULU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b011, /* Funct7 */ 0b000'0001, Extensions::F);
rvdash::R_Instruction<RV32ISz>   DIV(/* Opcode */ 0b011'0011, /* Funct3 */ 0b100, /* Funct7 */ 0b000'0001, Extensions::F);
rvdash::R_Instruction<RV32ISz>  DIVU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b101, /* Funct7 */ 0b000'0001, Extensions::F);
rvdash::R_Instruction<RV32ISz>   REM(/* Opcode */ 0b011'0011, /* Funct3 */ 0b110, /* Funct7 */ 0b000'0001, Extensions::F);
rvdash::R_Instruction<RV32ISz>  REMU(/* Opcode */ 0b011'0001, /* Funct3 */ 0b111, /* Funct7 */ 0b000'0001, Extensions::F);

//-----------------------------------FInstrDecoder---------------------------------------

std::optional<std::shared_ptr<rvdash::Instruction<RV32ISz>>> FInstrDecoder::tryDecode(Register Instr) const {
  auto Opcode = rvdash::R_Instruction<RV32ISz>::extractOpcode(Instr);
  auto Funct3 = rvdash::R_Instruction<RV32ISz>::extractFunct3(Instr);
  auto Funct7 = rvdash::R_Instruction<RV32ISz>::extractFunct7(Instr);

  return std::nullopt;
}

std::ostream& operator<<(std::ostream& Stream, const typename F::FInstrSet& Set)
{
    Set.print();
    return Stream;
}

} // namespace F
} // namespace rvdash

