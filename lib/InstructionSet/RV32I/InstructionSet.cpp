#include "rvdash/InstructionSet/RV32I/InstructionSet.h"




namespace rvdash {
namespace RV32I {

rvdash::Instruction ADD(/* Instr */ 0b0000000'00000'00000'000'00000'0110011,
                        InstrEncodingType::R, Extensions::RV32I);
// rvdash::R_Instruction  SUB(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /*
// Funct7 */ 0b010'0000, Extensions::RV32I); rvdash::R_Instruction  XOR(/*
// Opcode */ 0b011'0011, /* Funct3 */ 0b100, /* Funct7 */ 0b000'0000,
// Extensions::RV32I); rvdash::R_Instruction   OR(/* Opcode */ 0b011'0011, /*
// Funct3 */ 0b110, /* Funct7 */ 0b000'0000, Extensions::RV32I);
// rvdash::R_Instruction  AND(/* Opcode */ 0b011'0011, /* Funct3 */ 0b111, /*
// Funct7 */ 0b000'0000, Extensions::RV32I); rvdash::R_Instruction  SLL(/*
// Opcode */ 0b011'0011, /* Funct3 */ 0b001, /* Funct7 */ 0b000'0000,
// Extensions::RV32I); rvdash::R_Instruction  SRL(/* Opcode */ 0b011'0011, /*
// Funct3 */ 0b101, /* Funct7 */ 0b000'0000, Extensions::RV32I);
// rvdash::R_Instruction  SRA(/* Opcode */ 0b011'0011, /* Funct3 */ 0b101, /*
// Funct7 */ 0b010'0000, Extensions::RV32I); rvdash::R_Instruction  SLT(/*
// Opcode */ 0b011'0011, /* Funct3 */ 0b010, /* Funct7 */ 0b000'0000,
// Extensions::RV32I); rvdash::R_Instruction SLTU(/* Opcode */ 0b011'0011, /*
// Funct3 */ 0b011, /* Funct7 */ 0b000'0000, Extensions::RV32I);
//
// rvdash::I_Instruction  ADDI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b000,
// Extensions::RV32I); rvdash::I_Instruction  XORI(/* Opcode */ 0b001'0011, /*
// Funct3 */ 0b100, Extensions::RV32I); rvdash::I_Instruction   ORI(/* Opcode */
// 0b001'0011, /* Funct3 */ 0b110, Extensions::RV32I); rvdash::I_Instruction
// ANDI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b111, Extensions::RV32I);
// rvdash::I_Instruction  SLLI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b001, /*
// Imm_11_0 */ 0b0000'0000'0000, Extensions::RV32I); rvdash::I_Instruction
// SRLI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b101, /* Imm_11_0 */
// 0b0000'0000'0000, Extensions::RV32I); rvdash::I_Instruction  SRAI(/* Opcode
// */ 0b001'0011, /* Funct3 */ 0b101, /* Imm_11_0 */ 0b0000'0010'0000,
// Extensions::RV32I); rvdash::I_Instruction  SLTI(/* Opcode */ 0b001'0011, /*
// Funct3 */ 0b010, Extensions::RV32I); rvdash::I_Instruction SLTIU(/* Opcode */
// 0b001'0011, /* Funct3 */ 0b011, Extensions::RV32I);
//
// rvdash::I_Instruction   LB(/* Opcode */ 0b000'0011, /* Funct3 */ 0b000,
// Extensions::RV32I); rvdash::I_Instruction   LH(/* Opcode */ 0b000'0011, /*
// Funct3 */ 0b001, Extensions::RV32I);
rvdash::Instruction LW(/* Instr */ 0b0000000'00000'00000'010'00000'0000011,
                       InstrEncodingType::I, Extensions::RV32I);
// rvdash::I_Instruction  LBU(/* Opcode */ 0b000'0011, /* Funct3 */ 0b100,
// Extensions::RV32I); rvdash::I_Instruction  LHU(/* Opcode */ 0b000'0011, /*
// Funct3 */ 0b101, Extensions::RV32I);
//
// rvdash::S_Instruction SB(/* Opcode */ 0b010'0011, /* Funct3 */ 0b000,
// Extensions::RV32I); rvdash::S_Instruction SH(/* Opcode */ 0b010'0011, /*
// Funct3 */ 0b001, Extensions::RV32I); rvdash::S_Instruction SW(/* Opcode */
// 0b010'0011, /* Funct3 */ 0b010, Extensions::RV32I);
//
// rvdash::B_Instruction  BEQ(/* Opcode */ 0b110'0011, /* Funct3 */ 0b000,
// Extensions::RV32I); rvdash::B_Instruction  BNE(/* Opcode */ 0b110'0011, /*
// Funct3 */ 0b001, Extensions::RV32I); rvdash::B_Instruction  BLT(/* Opcode */
// 0b110'0011, /* Funct3 */ 0b100, Extensions::RV32I); rvdash::B_Instruction
// BGE(/* Opcode */ 0b110'0011, /* Funct3 */ 0b101, Extensions::RV32I);
// rvdash::B_Instruction BLTU(/* Opcode */ 0b110'0011, /* Funct3 */ 0b110,
// Extensions::RV32I); rvdash::B_Instruction BGEU(/* Opcode */ 0b110'0011, /*
// Funct3 */ 0b111, Extensions::RV32I);
//
// rvdash::J_Instruction  JAL(/* Opcode */ 0b110'1111, Extensions::RV32I);
// rvdash::I_Instruction JALR(/* Opcode */ 0b110'0111, /* Funct3 */ 0b000,
// Extensions::RV32I);
//
rvdash::Instruction LUI(/* Instr */ 0b0000000'00000'00000'000'00000'0110111,
                        InstrEncodingType::U, Extensions::RV32I);
// rvdash::U_Instruction AUIPC(/* Opcode */ 0b001'0111, Extensions::RV32I);
//
// rvdash::I_Instruction  ECALL(/* Opcode */ 0b111'0011, /* Funct3 */ 0b000,
// /*Imm_11_0 */ 0b0, Extensions::RV32I);
rvdash::Instruction EBREAK(/* Instr */ 0b000000000001'00000'000'00000'1110011,
                           InstrEncodingType::I, Extensions::RV32I);

//---------------------------------RV32IInstrDecoder-------------------------------------

//--------------------------------RV32IInstrExecutor-------------------------------------

std::shared_ptr<RV32IInstrExecutor> RV32IInstrExecutor::SingleExecutor;
std::shared_ptr<RegistersSet<32>> RV32IInstrExecutor::Registers;

std::shared_ptr<RV32IInstrExecutor> RV32IInstrExecutor::getExecutorInstance(std::shared_ptr<RegistersSet<32>> Regs) {
  if (SingleExecutor == nullptr)
    SingleExecutor = std::make_shared<RV32IInstrExecutor>(Regs);
  return SingleExecutor;
}

//----------------------------------RV32IInstrSet----------------------------------------

std::ostream &operator<<(std::ostream &Stream,
                         const typename RV32I::RV32IInstrSet &Set) {
  Set.dump(Stream);
  return Stream;
}

} // namespace RV32I
} // namespace rvdash

