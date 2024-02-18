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

rvdash::R_Instruction<RV32ISz>  ADD(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /* Funct7 */ 0b000'0000);
rvdash::R_Instruction<RV32ISz>  AND(/* Opcode */ 0b011'0011, /* Funct3 */ 0b111, /* Funct7 */ 0b000'0000);
rvdash::R_Instruction<RV32ISz>   OR(/* Opcode */ 0b011'0011, /* Funct3 */ 0b110, /* Funct7 */ 0b000'0000);
rvdash::R_Instruction<RV32ISz>  SLL(/* Opcode */ 0b011'0011, /* Funct3 */ 0b001, /* Funct7 */ 0b000'0000);
rvdash::R_Instruction<RV32ISz> SLTU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b011, /* Funct7 */ 0b000'0000);
rvdash::R_Instruction<RV32ISz>  SUB(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /* Funct7 */ 0b010'0000);
rvdash::R_Instruction<RV32ISz>  XOR(/* Opcode */ 0b011'0011, /* Funct3 */ 0b100, /* Funct7 */ 0b000'0000);
rvdash::R_Instruction<RV32ISz> SLLI(/* Opcode */ 0b011'0001, /* Funct3 */ 0b001, /* Funct7 */ 0b000'0000);
rvdash::R_Instruction<RV32ISz> SRAI(/* Opcode */ 0b011'0001, /* Funct3 */ 0b101, /* Funct7 */ 0b010'0000);
rvdash::R_Instruction<RV32ISz> SRLI(/* Opcode */ 0b011'0001, /* Funct3 */ 0b101, /* Funct7 */ 0b000'0000);

rvdash::U_Instruction<RV32ISz>   LUI(/* Opcode */ 0b011'0111);
rvdash::U_Instruction<RV32ISz> AUIPC(/* Opcode */ 0b001'0111);

rvdash::B_Instruction<RV32ISz>  BEQ(/* Opcode */ 0b110'0011, /* Funct3 */ 0b000);
rvdash::B_Instruction<RV32ISz>  BGE(/* Opcode */ 0b110'0011, /* Funct3 */ 0b101);
rvdash::B_Instruction<RV32ISz> BGEU(/* Opcode */ 0b110'0011, /* Funct3 */ 0b111);
rvdash::B_Instruction<RV32ISz>  BLT(/* Opcode */ 0b110'0011, /* Funct3 */ 0b100);
rvdash::B_Instruction<RV32ISz> BLTU(/* Opcode */ 0b110'0011, /* Funct3 */ 0b110);
rvdash::B_Instruction<RV32ISz>  BNE(/* Opcode */ 0b110'0011, /* Funct3 */ 0b001);

rvdash::J_Instruction<RV32ISz> JAL(/* Opcode */ 0b110'1111);

rvdash::S_Instruction<RV32ISz> SB(/* Opcode */ 0b010'0011, /* Funct3 */ 0b000);
rvdash::S_Instruction<RV32ISz> SH(/* Opcode */ 0b010'0011, /* Funct3 */ 0b001);
rvdash::S_Instruction<RV32ISz> SW(/* Opcode */ 0b010'0011, /* Funct3 */ 0b010);

rvdash::I_Instruction<RV32ISz> ADDI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b000);
rvdash::I_Instruction<RV32ISz> ANDI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b111);
rvdash::I_Instruction<RV32ISz>  ORI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b110);
rvdash::I_Instruction<RV32ISz> XORI(/* Opcode */ 0b001'0011, /* Funct3 */ 0b100);

rvdash::I_Instruction<RV32ISz> JALR(/* Opcode */ 0b110'0111, /* Funct3 */ 0b000);

rvdash::I_Instruction<RV32ISz>   LB(/* Opcode */ 0b000'0011, /* Funct3 */ 0b000);
rvdash::I_Instruction<RV32ISz>  LBU(/* Opcode */ 0b000'0011, /* Funct3 */ 0b100);
rvdash::I_Instruction<RV32ISz>   LH(/* Opcode */ 0b000'0011, /* Funct3 */ 0b001);
rvdash::I_Instruction<RV32ISz>  LHU(/* Opcode */ 0b000'0011, /* Funct3 */ 0b101);
rvdash::I_Instruction<RV32ISz>   LW(/* Opcode */ 0b000'0011, /* Funct3 */ 0b010);

rvdash::I_Instruction<RV32ISz>  FENCE(/* Opcode */ 0b001'0011, /* Funct3 */ 0b100);
rvdash::I_Instruction<RV32ISz> EBREAK(/* Instr */ 0b0000'0000'0001'00000'000'00000);

//----------------------------------InstrDecoder-----------------------------------------

class InstrDecoder : public rvdash::InstrDecoder {
public:
  InstrDecoder() {};
  std::optional<std::shared_ptr<rvdash::Instruction<RV32ISz>>> tryDecode(Register Instr) const override {
   auto Opcode = rvdash::R_Instruction<RV32ISz>::extractOpcode(Instr);
   auto Funct3 = rvdash::R_Instruction<RV32ISz>::extractFunct3(Instr);
   auto Funct7 = rvdash::R_Instruction<RV32ISz>::extractFunct7(Instr);

      if (Funct7 == 0b000'0000 || Funct7 == 0b010'0000) {
        if (Funct7 == 0b010'0000) {
          if (Funct3 == 0b000) {
            return Opcode == 0b0110011 ? std::make_shared<rvdash::R_Instruction<RV32ISz>>(SUB, Instr) : nullptr;
          }
          if (Funct3 == 0b101) {
            return Opcode == 0b001'0011 ? std::make_shared<rvdash::R_Instruction<RV32ISz>>(SRAI, Instr) : nullptr;
          }
          return nullptr;
        }
        if (Opcode == 0b011'0011 || Opcode == 0b001'0011) {
          if (Opcode == 0b001'0011) {
            if (Funct3 == 0b101)
              return std::make_shared<rvdash::R_Instruction<RV32ISz>>(SRLI, Instr);
            return Funct7 == 0b001 ? std::make_shared<rvdash::R_Instruction<RV32ISz>>(SLLI, Instr) : nullptr;
          }
          switch (Funct3) {
          case (0b000):
            return std::make_shared<rvdash::R_Instruction<RV32ISz>>(ADD, Instr);
          case (0b111):
            return std::make_shared<rvdash::R_Instruction<RV32ISz>>(AND, Instr);
          case (0b110):
            return std::make_shared<rvdash::R_Instruction<RV32ISz>>(OR, Instr); 
          case (0b001):
            return std::make_shared<rvdash::R_Instruction<RV32ISz>>(SLL, Instr);
          case (0b011):
            return std::make_shared<rvdash::R_Instruction<RV32ISz>>(SLTU, Instr);
          case (0b100):
            return std::make_shared<rvdash::R_Instruction<RV32ISz>>(XOR, Instr);
          }
        }
        return nullptr;
      }
      
      return nullptr;
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

class RV32IInstrSet : public InstrSet {

public:
  RV32IInstrSet() : InstrSet() {
    Decoder = std::make_shared<InstrDecoder>();
    Executor = std::make_shared<InstrExecutor>();
    Registers = std::make_shared<RegistersSet<RV32ISz>>(32);
    Registers.value()->addNamedRegister("pc");
    Registers.value()->getNamedRegister("pc") = 0;
  }
  ~RV32IInstrSet() {};

};

} // namespace RV32I
} // namespace rvdash

#endif// R32I_INSTRUCTION_SET_H

