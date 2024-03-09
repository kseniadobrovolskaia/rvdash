#include "rvdash/InstructionSet/RV32I/InstructionSet.h"




namespace rvdash {
namespace RV32I {

enum class Opcode {
  ADD,
  SUB,
  XOR,
  OR,
  AND,
  SLL,
  SRL,
  SRA,
  SLT,
  SLTU,
  ADDI,
  XORI,
  ORI,
  ANDI,
  SLLI,
  SRLI,
  SRAI,
  SLTI,
  SLTIU,
  LB,
  LH,
  LW,
  LBU,
  LHU,
  SB,
  SH,
  SW,
  BEQ,
  BNE,
  BLT,
  BGE,
  BLTU,
  BGEU,
  JAL,
  JALR,
  LUI,
  AUIPC,
  // ECALL,
  // EBREAK
};

rvdash::R_Instruction  ADD(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /* Funct7 */ 0b000'0000, Extensions::RV32I);
rvdash::R_Instruction  SUB(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /* Funct7 */ 0b010'0000, Extensions::RV32I);
rvdash::R_Instruction  XOR(/* Opcode */ 0b011'0011, /* Funct3 */ 0b100, /* Funct7 */ 0b000'0000, Extensions::RV32I);
rvdash::R_Instruction   OR(/* Opcode */ 0b011'0011, /* Funct3 */ 0b110, /* Funct7 */ 0b000'0000, Extensions::RV32I);
rvdash::R_Instruction  AND(/* Opcode */ 0b011'0011, /* Funct3 */ 0b111, /* Funct7 */ 0b000'0000, Extensions::RV32I);
rvdash::R_Instruction  SLL(/* Opcode */ 0b011'0011, /* Funct3 */ 0b001, /* Funct7 */ 0b000'0000, Extensions::RV32I);
rvdash::R_Instruction  SRL(/* Opcode */ 0b011'0011, /* Funct3 */ 0b101, /* Funct7 */ 0b000'0000, Extensions::RV32I);
rvdash::R_Instruction  SRA(/* Opcode */ 0b011'0011, /* Funct3 */ 0b101, /* Funct7 */ 0b010'0000, Extensions::RV32I);
rvdash::R_Instruction  SLT(/* Opcode */ 0b011'0011, /* Funct3 */ 0b010, /* Funct7 */ 0b000'0000, Extensions::RV32I);
rvdash::R_Instruction SLTU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b011, /* Funct7 */ 0b000'0000, Extensions::RV32I);

rvdash::I_Instruction  ADDI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b000, Extensions::RV32I);
rvdash::I_Instruction  XORI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b100, Extensions::RV32I);
rvdash::I_Instruction   ORI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b110, Extensions::RV32I);
rvdash::I_Instruction  ANDI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b111, Extensions::RV32I);
rvdash::I_Instruction  SLLI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b001, /* Imm_11_0 */ 0b0000'0000'0000, Extensions::RV32I);
rvdash::I_Instruction  SRLI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b101, /* Imm_11_0 */ 0b0000'0000'0000, Extensions::RV32I);
rvdash::I_Instruction  SRAI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b101, /* Imm_11_0 */ 0b0000'0010'0000, Extensions::RV32I);
rvdash::I_Instruction  SLTI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b010, Extensions::RV32I);
rvdash::I_Instruction SLTIU(/* Opcode */ 0b001'0011, /* Funct3 */ 0b011, Extensions::RV32I);

rvdash::I_Instruction   LB(/* Opcode */ 0b000'0011, /* Funct3 */ 0b000, Extensions::RV32I);
rvdash::I_Instruction   LH(/* Opcode */ 0b000'0011, /* Funct3 */ 0b001, Extensions::RV32I);
rvdash::I_Instruction   LW(/* Opcode */ 0b000'0011, /* Funct3 */ 0b010, Extensions::RV32I);
rvdash::I_Instruction  LBU(/* Opcode */ 0b000'0011, /* Funct3 */ 0b100, Extensions::RV32I);
rvdash::I_Instruction  LHU(/* Opcode */ 0b000'0011, /* Funct3 */ 0b101, Extensions::RV32I);

rvdash::S_Instruction SB(/* Opcode */ 0b010'0011, /* Funct3 */ 0b000, Extensions::RV32I);
rvdash::S_Instruction SH(/* Opcode */ 0b010'0011, /* Funct3 */ 0b001, Extensions::RV32I);
rvdash::S_Instruction SW(/* Opcode */ 0b010'0011, /* Funct3 */ 0b010, Extensions::RV32I);

rvdash::B_Instruction  BEQ(/* Opcode */ 0b110'0011, /* Funct3 */ 0b000, Extensions::RV32I);
rvdash::B_Instruction  BNE(/* Opcode */ 0b110'0011, /* Funct3 */ 0b001, Extensions::RV32I);
rvdash::B_Instruction  BLT(/* Opcode */ 0b110'0011, /* Funct3 */ 0b100, Extensions::RV32I);
rvdash::B_Instruction  BGE(/* Opcode */ 0b110'0011, /* Funct3 */ 0b101, Extensions::RV32I);
rvdash::B_Instruction BLTU(/* Opcode */ 0b110'0011, /* Funct3 */ 0b110, Extensions::RV32I);
rvdash::B_Instruction BGEU(/* Opcode */ 0b110'0011, /* Funct3 */ 0b111, Extensions::RV32I);

rvdash::J_Instruction  JAL(/* Opcode */ 0b110'1111, Extensions::RV32I);
rvdash::I_Instruction JALR(/* Opcode */ 0b110'0111, /* Funct3 */ 0b000, Extensions::RV32I);

rvdash::U_Instruction   LUI(/* Opcode */ 0b011'0111, Extensions::RV32I);
rvdash::U_Instruction AUIPC(/* Opcode */ 0b001'0111, Extensions::RV32I);

// rvdash::I_Instruction  ECALL(/* Opcode */ 0b111'0011, /* Funct3 */ 0b000, /*
// Imm_11_0 */ 0b0, Extensions::RV32I); rvdash::I_Instruction EBREAK(/* Opcode
// */ 0b111'0011, /* Funct3 */ 0b000, /* Imm_11_0 */ 0b1, Extensions::RV32I);

//---------------------------------RV32IInstrDecoder-------------------------------------

//--------------------------------RV32IInstrExecutor-------------------------------------

RegistersSet<32> RV32IInstrExecutor::Registers = RegistersSet<32>();

void RV32IInstrExecutor::executeR_Instr(const R_Instruction &Instr, int) {
  std::cout << "execute_R_Instr\n";
}

void RV32IInstrExecutor::executeADD(std::shared_ptr<Instruction> Instr) {
  auto AddInstr = *static_cast<R_Instruction *>(Instr.get());
  auto Rd = AddInstr.Rd.to_ulong();
  auto Rs1 = AddInstr.Rs1.to_ulong();
  auto Rs2 = AddInstr.Rs2.to_ulong();

  Registers[Rd] = Registers[Rs1].to_ulong() + Registers[Rs2].to_ulong();

  std::cout << "Execute ADD\n";
  std::cout << "Rd = " << Rd << ", Rs1 = " << Rs1 << ", rs2 = " << Rs2
            << "\n\n";
};

void RV32IInstrExecutor::executeSUB(std::shared_ptr<Instruction> Instr) {
  std::cout << "Execute SUB\n";
};
void RV32IInstrExecutor::executeXOR(std::shared_ptr<Instruction> Instr) {
  std::cout << "Execute XOR\n";
};
void RV32IInstrExecutor::executeOR(std::shared_ptr<Instruction> Instr) {
  std::cout << "Execute  OR\n";
}
void RV32IInstrExecutor::executeAND(std::shared_ptr<Instruction> Instr) {
  std::cout << "Execute AND\n";
};
void RV32IInstrExecutor::executeSLL(std::shared_ptr<Instruction> Instr) {
  std::cout << "Execute SLL\n";
};
void RV32IInstrExecutor::executeSRL(std::shared_ptr<Instruction> Instr) {
  std::cout << "Execute SRL\n";
};
void RV32IInstrExecutor::executeSRA(std::shared_ptr<Instruction> Instr) {
  std::cout << "Execute SRA\n";
};
void RV32IInstrExecutor::executeSLT(std::shared_ptr<Instruction> Instr) {
  std::cout << "Execute SLT\n";
};
void RV32IInstrExecutor::executeSLTU(std::shared_ptr<Instruction> Instr) {
  std::cout << "Execute SLTU\n";
};

void RV32IInstrExecutor::executeI_Instr() { std::cout << "execute_I_Instr\n"; };

void RV32IInstrExecutor::executeS_Instr() { std::cout << "execute_S_Instr\n"; };

void RV32IInstrExecutor::executeB_Instr() { std::cout << "execute_B_Instr\n"; };

void RV32IInstrExecutor::executeU_Instr() { std::cout << "execute_U_Instr\n"; };

void RV32IInstrExecutor::executeJ_Instr() { std::cout << "execute_J_Instr\n"; };

//----------------------------------RV32IInstrSet----------------------------------------

std::ostream &operator<<(std::ostream &Stream,
                         const typename RV32I::RV32IInstrSet &Set) {
  Set.dump(Stream);
  return Stream;
}

} // namespace RV32I
} // namespace rvdash

