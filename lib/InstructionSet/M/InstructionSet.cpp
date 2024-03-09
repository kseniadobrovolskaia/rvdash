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

//----------------------------------MInstrDecoder----------------------------------------

//---------------------------------MInstrExecutor----------------------------------------

//-----------------------------------MInstrSet-------------------------------------------

std::ostream &operator<<(std::ostream &Stream,
                         const typename M::MInstrSet &Set) {
  Set.print();
  return Stream;
}

} // namespace M
} // namespace rvdash

