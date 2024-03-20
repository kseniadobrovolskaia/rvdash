#include "rvdash/InstructionSet/RV32I/InstructionSet.h"




namespace rvdash {
namespace RV32I {

rvdash::Instruction ADD(/* Instr */ 0b0000000'00000'00000'000'00000'0110011,
                        InstrEncodingType::R, Extensions::RV32I);
rvdash::Instruction SUB(/* Instr */ 0b0100000'00000'00000'000'00000'0110011,
                        InstrEncodingType::R, Extensions::RV32I);
rvdash::Instruction XOR(/* Instr */ 0b0000000'00000'00000'100'00000'0110011,
                        InstrEncodingType::R, Extensions::RV32I);
rvdash::Instruction OR(/* Instr */ 0b0000000'00000'00000'110'00000'0110011,
                       InstrEncodingType::R, Extensions::RV32I);
rvdash::Instruction AND(/* Instr */ 0b0000000'00000'00000'111'00000'0110011,
                        InstrEncodingType::R, Extensions::RV32I);
rvdash::Instruction SLL(/* Instr */ 0b0000000'00000'00000'001'00000'0110011,
                        InstrEncodingType::R, Extensions::RV32I);
rvdash::Instruction SRL(/* Instr */ 0b0000000'00000'00000'101'00000'0110011,
                        InstrEncodingType::R, Extensions::RV32I);
rvdash::Instruction SRA(/* Instr */ 0b0100000'00000'00000'101'00000'0110011,
                        InstrEncodingType::R, Extensions::RV32I);
rvdash::Instruction SLT(/* Instr */ 0b0000000'00000'00000'010'00000'0110011,
                        InstrEncodingType::R, Extensions::RV32I);
rvdash::Instruction SLTU(/* Instr */ 0b0000000'00000'00000'011'00000'0110011,
                         InstrEncodingType::R, Extensions::RV32I);

rvdash::Instruction ADDI(/* Instr */ 0b000000000000'00000'000'00000'0010011,
                         InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction XORI(/* Instr */ 0b000000000000'00000'100'00000'0010011,
                         InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction ORI(/* Instr */ 0b000000000000'00000'110'00000'0010011,
                        InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction ANDI(/* Instr */ 0b000000000000'00000'111'00000'0010011,
                         InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction SLLI(/* Instr */ 0b000000000000'00000'001'00000'0010011,
                         InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction SRLI(/* Instr */ 0b000000000000'00000'101'00000'0010011,
                         InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction SRAI(/* Instr */ 0b000000100000'00000'101'00000'0010011,
                         InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction SLTI(/* Instr */ 0b000000000000'00000'010'00000'0010011,
                         InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction SLTIU(/* Instr */ 0b000000000000'00000'011'00000'0010011,
                          InstrEncodingType::I, Extensions::RV32I);

rvdash::Instruction LB(/* Instr */ 0b0000000'00000'00000'000'00000'0000011,
                       InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction LH(/* Instr */ 0b0000000'00000'00000'001'00000'0000011,
                       InstrEncodingType::I, Extensions::RV32I);
rvdash::Instruction LW(/* Instr */ 0b0000000'00000'00000'010'00000'0000011,
                       InstrEncodingType::I, Extensions::RV32I);
// rvdash::I_Instruction  LBU(/* Opcode */ 0b000'0011, /* Funct3 */ 0b100,
// Extensions::RV32I); rvdash::I_Instruction  LHU(/* Opcode */ 0b000'0011, /*
// Funct3 */ 0b101, Extensions::RV32I);

rvdash::Instruction SB(/* Instr */ 0b0000000'00000'00000'000'00000'0100011,
                       InstrEncodingType::S, Extensions::RV32I);
rvdash::Instruction SH(/* Instr */ 0b0000000'00000'00000'001'00000'0100011,
                       InstrEncodingType::S, Extensions::RV32I);
rvdash::Instruction SW(/* Instr */ 0b0000000'00000'00000'010'00000'0100011,
                       InstrEncodingType::S, Extensions::RV32I);

// rvdash::B_Instruction  BEQ(/* Opcode */ 0b110'0011, /* Funct3 */ 0b000,
// Extensions::RV32I); rvdash::B_Instruction  BNE(/* Opcode */ 0b110'0011, /*
// Funct3 */ 0b001, Extensions::RV32I); rvdash::B_Instruction  BLT(/* Opcode */
// 0b110'0011, /* Funct3 */ 0b100, Extensions::RV32I); rvdash::B_Instruction
// BGE(/* Opcode */ 0b110'0011, /* Funct3 */ 0b101, Extensions::RV32I);
// rvdash::B_Instruction BLTU(/* Opcode */ 0b110'0011, /* Funct3 */ 0b110,
// Extensions::RV32I); rvdash::B_Instruction BGEU(/* Opcode */ 0b110'0011, /*
// Funct3 */ 0b111, Extensions::RV32I);

rvdash::Instruction JAL(/* Instr */ 0b0000000000000000000000000'1101111,
                        InstrEncodingType::J, Extensions::RV32I);
rvdash::Instruction JALR(/* Instr */ 0b000000000000'00000'000'00000'1100111,
                         InstrEncodingType::I, Extensions::RV32I);

rvdash::Instruction LUI(/* Instr */ 0b0000000'00000'00000'000'00000'0110111,
                        InstrEncodingType::U, Extensions::RV32I);
rvdash::Instruction AUIPC(/* Instr */ 0b0000000'00000'00000'000'00000'0010111,
                          InstrEncodingType::U, Extensions::RV32I);

rvdash::Instruction ECALL(/* Instr */ 0b000000000000'00000'000'00000'1110011,
                          InstrEncodingType::I, Extensions::RV32I);
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

