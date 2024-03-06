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

std::optional<std::shared_ptr<rvdash::Instruction>> RV32IInstrDecoder::tryDecode(Register<Instruction::Sz> Instr) const {
  auto Opcode = rvdash::Instruction::extractOpcode(Instr);
  auto Funct3 = rvdash::Instruction::extractFunct3(Instr);
  auto Funct7 = rvdash::Instruction::extractFunct7(Instr);

  // R_Instruction
  if (Opcode == 0b011'0011) {
    if (Funct7 == 0b010'0000) {
      switch (Funct3) {
        case (0b000):
          return std::make_shared<rvdash::R_Instruction>(SUB, Instr);
        case (0b101):
          return std::make_shared<rvdash::R_Instruction>(SRA, Instr);
        default:
          return std::nullopt;
      }
    }
    if (Funct7 != 0b000'0000)
      return std::nullopt;
    switch (Funct3) {
      case (0b000):
        return std::make_shared<rvdash::R_Instruction>(ADD, Instr);
      case (0b100):
        return std::make_shared<rvdash::R_Instruction>(XOR, Instr);
      case (0b110):
        return std::make_shared<rvdash::R_Instruction>(OR, Instr); 
      case (0b111):
        return std::make_shared<rvdash::R_Instruction>(AND, Instr);
      case (0b001):
        return std::make_shared<rvdash::R_Instruction>(SLL, Instr);
      case (0b101):
        return std::make_shared<rvdash::R_Instruction>(SRL, Instr);
      case (0b010):
        return std::make_shared<rvdash::R_Instruction>(SLT, Instr);
      case (0b011):
        return std::make_shared<rvdash::R_Instruction>(SLTU, Instr);
      default:
        return std::nullopt;
    }
  } 
  // I_Instruction without ECALL, EBREAK, JALR and loads
  else if (Opcode == 0b001'0011) {
    switch (Funct3) {
      case (0b000):
        return std::make_shared<rvdash::I_Instruction>(ADDI, Instr);
      case (0b100):
        return std::make_shared<rvdash::I_Instruction>(XORI, Instr);
      case (0b110):
        return std::make_shared<rvdash::I_Instruction>(ORI, Instr); 
      case (0b111):
        return std::make_shared<rvdash::I_Instruction>(ANDI, Instr);
      case (0b001): {
        auto Imm = rvdash::Instruction::extractImm_11_0(Instr);
        switch (Imm) {
          case (0b0): 
            return std::make_shared<rvdash::I_Instruction>(SLLI, Instr);
          default:
            return std::nullopt;
        }
      }
      case (0b101): {
        auto Imm = rvdash::Instruction::extractImm_11_0(Instr);
        switch (Imm) {
          case (0b0): 
            return std::make_shared<rvdash::I_Instruction>(SRLI, Instr);
          case (0b0000'0010'0000): 
            return std::make_shared<rvdash::I_Instruction>(SRAI, Instr);
          default:
            return std::nullopt;
        }
       }
      case (0b010):
        return std::make_shared<rvdash::I_Instruction>(SLTI, Instr);
      case (0b011):
        return std::make_shared<rvdash::I_Instruction>(SLTIU, Instr);
      default: 
        return std::nullopt;
    }
  }
  // Loads (I_Instruction)
  else if (Opcode == 0b000'0011) {
    switch (Funct3) {
      case 0b000:
        return std::make_shared<rvdash::I_Instruction>(LB, Instr);
      case 0b001:
        return std::make_shared<rvdash::I_Instruction>(LH, Instr);
      case 0b010:
        return std::make_shared<rvdash::I_Instruction>(LW, Instr);
      case 0b100:
        return std::make_shared<rvdash::I_Instruction>(LBU, Instr);
      case 0b101:
        return std::make_shared<rvdash::I_Instruction>(LHU, Instr);
      default: 
        return std::nullopt;
    }
  }
  // B_Instruction
  else if (Opcode == 0b110'0011) {
    switch (Funct3) {
      case 0b000:
        return std::make_shared<rvdash::B_Instruction>(BEQ, Instr);
      case 0b001:
        return std::make_shared<rvdash::B_Instruction>(BNE, Instr);
      case 0b100:
        return std::make_shared<rvdash::B_Instruction>(BLT, Instr);
      case 0b101:
        return std::make_shared<rvdash::B_Instruction>(BGE, Instr);
      case 0b110:
        return std::make_shared<rvdash::B_Instruction>(BLTU, Instr);
      case 0b111:
        return std::make_shared<rvdash::B_Instruction>(BGEU, Instr);
      default: 
        return std::nullopt;
    }
  }
  // S_Instruction
  else if (Opcode == 0b010'0011) {
    switch (Funct3) {
      case 0b000:
        return std::make_shared<rvdash::S_Instruction>(SB, Instr);
      case 0b001:
        return std::make_shared<rvdash::S_Instruction>(SH, Instr);
      case 0b010:
        return std::make_shared<rvdash::S_Instruction>(SW, Instr);
      default: 
        return std::nullopt;
    }
  }
  // LUI (U_Instruction)
  else if (Opcode == 0b011'0111) {
    return std::make_shared<rvdash::U_Instruction>(LUI, Instr);
  }
  // AUIPC (U_Instruction)
  else if (Opcode == 0b001'0111) {
    return std::make_shared<rvdash::U_Instruction>(AUIPC, Instr);
  }  
  // J_Instruction
  else if (Opcode == 0b110'1111) {
    return std::make_shared<rvdash::J_Instruction>(JAL, Instr);
  } 
  // JALR (I_Instruction)
  else if (Opcode == 0b110'0111) {
    switch (Funct3) {
      case (0b000): 
        return std::make_shared<rvdash::I_Instruction>(JALR, Instr);
      default:
        return std::nullopt;
    }
  }
  /* Not implemented yet

  // ECALL, EBREAK (I_Instuction)
  else if (Opcode == 0b111'0011) {
    if (Funct3 != 0b000)
      return std::nullopt;
    auto Imm = rvdash::Instruction::extractImm_11_0(Instr);
    switch (Imm) {
      case (0b0):
        return std::make_shared<rvdash::I_Instruction>(ECALL, Instr);
      case (0b1):
        return std::make_shared<rvdash::I_Instruction>(EBREAK, Instr);
      default:
        return std::nullopt;
    }
  }
  */
  return std::nullopt;
}

//--------------------------------RV32IInstrExecutor-------------------------------------

void RV32IInstrExecutor::execute(std::shared_ptr<Instruction> Instr) const {
  std::cout << "RV32I execute :";
  Instr->print();
  std::cout << "\n";

  switch(Instr->getType()) {
  case (InstrEncodingType::R):
    executeR_Instr();
    break;
  case (InstrEncodingType::I):
    executeI_Instr();
    break;
  case (InstrEncodingType::S):
    executeS_Instr();
    break;
  case (InstrEncodingType::B):
    executeB_Instr();
    break;
  case (InstrEncodingType::U):
    executeU_Instr();
    break;
  case (InstrEncodingType::J):
    executeJ_Instr();
    break;
  default:
    failWithError("Illegal instruction encoding type");
  }
}

void RV32IInstrExecutor::executeR_Instr() const {
  std::cout << "execute_R_Instr\n";
}

void RV32IInstrExecutor::executeI_Instr() const {
  std::cout << "execute_I_Instr\n";
};

void RV32IInstrExecutor::executeS_Instr() const {
  std::cout << "execute_S_Instr\n";
};

void RV32IInstrExecutor::executeB_Instr() const {
  std::cout << "execute_B_Instr\n";
};

void RV32IInstrExecutor::executeU_Instr() const {
  std::cout << "execute_U_Instr\n";
};

void RV32IInstrExecutor::executeJ_Instr() const {
  std::cout << "execute_J_Instr\n";
};

//----------------------------------RV32IInstrSet----------------------------------------

std::ostream& operator<<(std::ostream& stream, const typename RV32I::RV32IInstrSet& counter)
{
    counter.print();
    return stream;
}

} // namespace RV32I
} // namespace rvdash

