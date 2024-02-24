#ifndef R32I_INSTRUCTION_SET_H
#define R32I_INSTRUCTION_SET_H

#include "RVdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace RV32I {

class RV32IInstrSet;
const size_t RV32ISz = 32;
using Register = std::bitset<RV32ISz>;


enum class InstrGroup {
  LoadsAndStores,
  ControlTransferInstr,
  MemoryOrdInstr,
  ComputInstr,
  EnvCallsAndBreakpoints,
};


enum class Opcode {
  LUI,
  AUIPC,
  JAL,
  JALR,
  BEQ,
  BNE,
  BLT,
  BGE,
  BLTU,
  BGEU,
  LB,
  LH,
  LW,
  LBU,
  LHU,
  SB,
  SH,
  SW,
  ADDI,
  SLTI,
  SLTIU,
  XORI,
  ORI,
  ANDI,
  SLLI,
  SRLI,
  SRAI,
  ADD,
  SUB,
  SLL,
  SLT,
  SLTU,
  XOR,
  SRL,
  SRA,
  OR,
  AND,
  FENCE,
  FENCE_I,
  ECALL,
  EBREAK
};

rvdash::R_Instruction<RV32ISz>  ADD(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /* Funct7 */ 0b000'0000, Extentions::RV32I);
rvdash::R_Instruction<RV32ISz>  SUB(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /* Funct7 */ 0b010'0000, Extentions::RV32I);
rvdash::R_Instruction<RV32ISz>  XOR(/* Opcode */ 0b011'0011, /* Funct3 */ 0b100, /* Funct7 */ 0b000'0000, Extentions::RV32I);
rvdash::R_Instruction<RV32ISz>   OR(/* Opcode */ 0b011'0011, /* Funct3 */ 0b110, /* Funct7 */ 0b000'0000, Extentions::RV32I);
rvdash::R_Instruction<RV32ISz>  AND(/* Opcode */ 0b011'0011, /* Funct3 */ 0b111, /* Funct7 */ 0b000'0000, Extentions::RV32I);
rvdash::R_Instruction<RV32ISz>  SLL(/* Opcode */ 0b011'0011, /* Funct3 */ 0b001, /* Funct7 */ 0b000'0000, Extentions::RV32I);
rvdash::R_Instruction<RV32ISz>  SRL(/* Opcode */ 0b011'0011, /* Funct3 */ 0b101, /* Funct7 */ 0b000'0000, Extentions::RV32I);
rvdash::R_Instruction<RV32ISz>  SRA(/* Opcode */ 0b011'0011, /* Funct3 */ 0b101, /* Funct7 */ 0b010'0000, Extentions::RV32I);
rvdash::R_Instruction<RV32ISz>  SLT(/* Opcode */ 0b011'0011, /* Funct3 */ 0b010, /* Funct7 */ 0b000'0000, Extentions::RV32I);
rvdash::R_Instruction<RV32ISz> SLTU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b011, /* Funct7 */ 0b000'0000, Extentions::RV32I);

rvdash::I_Instruction<RV32ISz>  ADDI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b000, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>  XORI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b100, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>   ORI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b110, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>  ANDI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b111, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>  SLLI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b001, /* Imm_11_0 */ 0b0000'0000'0000, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>  SRLI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b101, /* Imm_11_0 */ 0b0000'0000'0000, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>  SRAI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b101, /* Imm_11_0 */ 0b0000'0010'0000, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>  SLTI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b010, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz> SLTIU(/* Opcode */ 0b001'0011, /* Funct3 */ 0b011, Extentions::RV32I);

rvdash::I_Instruction<RV32ISz>   LB(/* Opcode */ 0b000'0011, /* Funct3 */ 0b000, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>   LH(/* Opcode */ 0b000'0011, /* Funct3 */ 0b001, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>   LW(/* Opcode */ 0b000'0011, /* Funct3 */ 0b010, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>  LBU(/* Opcode */ 0b000'0011, /* Funct3 */ 0b100, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz>  LHU(/* Opcode */ 0b000'0011, /* Funct3 */ 0b101, Extentions::RV32I);

rvdash::S_Instruction<RV32ISz> SB(/* Opcode */ 0b010'0011, /* Funct3 */ 0b000, Extentions::RV32I);
rvdash::S_Instruction<RV32ISz> SH(/* Opcode */ 0b010'0011, /* Funct3 */ 0b001, Extentions::RV32I);
rvdash::S_Instruction<RV32ISz> SW(/* Opcode */ 0b010'0011, /* Funct3 */ 0b010, Extentions::RV32I);

rvdash::B_Instruction<RV32ISz>  BEQ(/* Opcode */ 0b110'0011, /* Funct3 */ 0b000, Extentions::RV32I);
rvdash::B_Instruction<RV32ISz>  BNE(/* Opcode */ 0b110'0011, /* Funct3 */ 0b001, Extentions::RV32I);
rvdash::B_Instruction<RV32ISz>  BLT(/* Opcode */ 0b110'0011, /* Funct3 */ 0b100, Extentions::RV32I);
rvdash::B_Instruction<RV32ISz>  BGE(/* Opcode */ 0b110'0011, /* Funct3 */ 0b101, Extentions::RV32I);
rvdash::B_Instruction<RV32ISz> BLTU(/* Opcode */ 0b110'0011, /* Funct3 */ 0b110, Extentions::RV32I);
rvdash::B_Instruction<RV32ISz> BGEU(/* Opcode */ 0b110'0011, /* Funct3 */ 0b111, Extentions::RV32I);

rvdash::J_Instruction<RV32ISz>  JAL(/* Opcode */ 0b110'1111, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz> JALR(/* Opcode */ 0b110'0111, /* Funct3 */ 0b000, Extentions::RV32I);

rvdash::U_Instruction<RV32ISz>   LUI(/* Opcode */ 0b011'0111, Extentions::RV32I);
rvdash::U_Instruction<RV32ISz> AUIPC(/* Opcode */ 0b001'0111, Extentions::RV32I);

rvdash::I_Instruction<RV32ISz>  ECALL(/* Opcode */ 0b111'0011, /* Funct3 */ 0b000, /* Imm_11_0 */ 0b0, Extentions::RV32I);
rvdash::I_Instruction<RV32ISz> EBREAK(/* Opcode */ 0b111'0011, /* Funct3 */ 0b000, /* Imm_11_0 */ 0b1, Extentions::RV32I);

//----------------------------------InstrDecoder-----------------------------------------

class InstrDecoder : public rvdash::InstrDecoder {
public:
  InstrDecoder() {};
  std::optional<std::shared_ptr<rvdash::Instruction<RV32ISz>>> tryDecode(Register Instr) const override {
    auto Opcode = rvdash::R_Instruction<RV32ISz>::extractOpcode(Instr);
    auto Funct3 = rvdash::R_Instruction<RV32ISz>::extractFunct3(Instr);
    auto Funct7 = rvdash::R_Instruction<RV32ISz>::extractFunct7(Instr);

    // R_Instruction
    if (Opcode == 0b011'0011) {
      if (Funct7 == 0b010'0000) {
        if (Funct3 == 0b000)
          return std::make_shared<rvdash::R_Instruction<RV32ISz>>(SUB, Instr);
        return Funct3 == 0b101 ? std::optional(std::make_shared<rvdash::R_Instruction<RV32ISz>>(SRA, Instr)) : std::nullopt;
      }
      if (Funct7 != 0b000'0000)
        return std::nullopt;
      switch (Funct3) {
        case (0b000):
          return std::make_shared<rvdash::R_Instruction<RV32ISz>>(ADD, Instr);
        case (0b100):
          return std::make_shared<rvdash::R_Instruction<RV32ISz>>(XOR, Instr);
        case (0b110):
          return std::make_shared<rvdash::R_Instruction<RV32ISz>>(OR, Instr); 
        case (0b111):
          return std::make_shared<rvdash::R_Instruction<RV32ISz>>(AND, Instr);
        case (0b001):
          return std::make_shared<rvdash::R_Instruction<RV32ISz>>(SLL, Instr);
        case (0b101):
          return std::make_shared<rvdash::R_Instruction<RV32ISz>>(SRL, Instr);
        case (0b010):
          return std::make_shared<rvdash::R_Instruction<RV32ISz>>(SLT, Instr);
        case (0b011):
          return std::make_shared<rvdash::R_Instruction<RV32ISz>>(SLTU, Instr);
      }
      return std::nullopt;
    } 
    // I_Instruction without ECALL, EBREAK, JALR and loads
    else if (Opcode == 0b001'0011) {
      switch (Funct3) {
        case (0b000):
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(ADDI, Instr);
        case (0b100):
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(XORI, Instr);
        case (0b110):
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(ORI, Instr); 
        case (0b111):
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(ANDI, Instr);
        case (0b001): {
          auto Imm = rvdash::R_Instruction<RV32ISz>::extractImm_11_0(Instr);
          return Imm == 0b0 ? std::optional(std::make_shared<rvdash::I_Instruction<RV32ISz>>(SLLI, Instr)) : std::nullopt;
        }
        case (0b101): {
          auto Imm = rvdash::R_Instruction<RV32ISz>::extractImm_11_0(Instr);
          if (Imm == 0b0)
            return std::make_shared<rvdash::I_Instruction<RV32ISz>>(SRLI, Instr);
          
          return Imm == 0b0000'0010'0000 ? std::optional(std::make_shared<rvdash::I_Instruction<RV32ISz>>(SRAI, Instr))
                 : std::nullopt;
        }
        case (0b010):
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(SLTI, Instr);
        case (0b011):
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(SLTIU, Instr);
      }
      return std::nullopt;
    }
    // Loads (I_Instruction)
    else if (Opcode == 0b000'0011) {
      switch (Funct3) {
        case 0b000:
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(LB, Instr);
        case 0b001:
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(LH, Instr);
        case 0b010:
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(LW, Instr);
        case 0b100:
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(LBU, Instr);
        case 0b101:
          return std::make_shared<rvdash::I_Instruction<RV32ISz>>(LHU, Instr);
      }
      return std::nullopt;     
    }
    // B_Instruction
    else if (Opcode == 0b110'0011) {
      switch (Funct3) {
        case 0b000:
          return std::make_shared<rvdash::B_Instruction<RV32ISz>>(BEQ, Instr);
        case 0b001:
          return std::make_shared<rvdash::B_Instruction<RV32ISz>>(BNE, Instr);
        case 0b100:
          return std::make_shared<rvdash::B_Instruction<RV32ISz>>(BLT, Instr);
        case 0b101:
          return std::make_shared<rvdash::B_Instruction<RV32ISz>>(BGE, Instr);
        case 0b110:
          return std::make_shared<rvdash::B_Instruction<RV32ISz>>(BLTU, Instr);
        case 0b111:
          return std::make_shared<rvdash::B_Instruction<RV32ISz>>(BGEU, Instr);
      }
      return std::nullopt;
    }
    // S_Instruction
    else if (Opcode == 0b010'0011) {
      switch (Funct3) {
        case 0b000:
          return std::make_shared<rvdash::S_Instruction<RV32ISz>>(SB, Instr);
        case 0b001:
          return std::make_shared<rvdash::S_Instruction<RV32ISz>>(SH, Instr);
        case 0b010:
          return std::make_shared<rvdash::S_Instruction<RV32ISz>>(SW, Instr);
      }
      return std::nullopt;
    }
    // LUI (U_Instruction)
    else if (Opcode == 0b011'0111) {
      return std::make_shared<rvdash::U_Instruction<RV32ISz>>(LUI, Instr);
    }
    // AUIPC (U_Instruction)
    else if (Opcode == 0b001'0111) {
      return std::make_shared<rvdash::U_Instruction<RV32ISz>>(AUIPC, Instr);
    }  
    // J_Instruction
    else if (Opcode == 0b110'1111) {
      return std::make_shared<rvdash::J_Instruction<RV32ISz>>(JAL, Instr);
    } 
    // JALR (I_Instruction)
    else if (Opcode == 0b110'0111) {
      return Funct3 == 0b000 ? std::optional(std::make_shared<rvdash::I_Instruction<RV32ISz>>(JALR, Instr)) : std::nullopt;
    } 
    // ECALL, EBREAK (I_Instuction)
    else if (Opcode == 0b111'0011) {
      auto Imm = rvdash::R_Instruction<RV32ISz>::extractImm_11_0(Instr);
      if (Funct3 != 0b000)
        return std::nullopt;
      if (Imm == 0b0)
        return std::make_shared<rvdash::I_Instruction<RV32ISz>>(ECALL, Instr);
      return Imm == 0b1 ? std::optional(std::make_shared<rvdash::I_Instruction<RV32ISz>>(EBREAK, Instr)) : std::nullopt;
    }
    return std::nullopt;
  }
};

//----------------------------------InstrExecutor----------------------------------------

class InstrExecutor : public rvdash::InstrExecutor {

public:
    InstrExecutor() {};
    void execute(const Instruction<RV32ISz> &Instr) const override {
      std::cout << "execute\n"; 
    }
};

//------------------------------------InstrSet-------------------------------------------

class RV32IInstrSet {
protected:
  std::shared_ptr<InstrDecoder> Decoder;
  std::shared_ptr<InstrExecutor> Executor;
  std::optional<std::shared_ptr<RegistersSet<AddrSpaceSz>>> Registers;


public:
  RV32IInstrSet() {
    Decoder = std::make_shared<InstrDecoder>();
    Executor = std::make_shared<InstrExecutor>();
    Registers = std::make_shared<RegistersSet<RV32ISz>>(32);
    Registers.value()->addNamedRegister("pc");
    Registers.value()->getNamedRegister("pc") = 0;
  }

  ~RV32IInstrSet() {};

  void print() const { std::cout << "RV32IInstrSet\n"; }

  std::optional<std::shared_ptr<Instruction<AddrSpaceSz>>> tryDecode(Register Instr) const {
    return Decoder->tryDecode(Instr);
  }

};


std::ostream& operator<<(std::ostream& stream, const typename RV32I::RV32IInstrSet& counter)
{
    stream << "Value: ";
    counter.print();
    return stream;
}

} // namespace RV32I
} // namespace rvdash

#endif// R32I_INSTRUCTION_SET_H

