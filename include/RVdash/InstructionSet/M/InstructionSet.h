#ifndef M_INSTRUCTION_SET_H
#define M_INSTRUCTION_SET_H

#include "RVdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace M {

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
  MUL,
  MULH,
  MULSU,
  MULU,
  DIV,
  DIVU,
  REM,
  REMU
};

rvdash::R_Instruction<RV32ISz>   MUL(/* Opcode */ 0b011'0011, /* Funct3 */ 0b000, /* Funct7 */ 0b000'0001, Extentions::M);
rvdash::R_Instruction<RV32ISz>  MULH(/* Opcode */ 0b011'0011, /* Funct3 */ 0b001, /* Funct7 */ 0b000'0001, Extentions::M);
rvdash::R_Instruction<RV32ISz> MULSU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b010, /* Funct7 */ 0b000'0001, Extentions::M);
rvdash::R_Instruction<RV32ISz>  MULU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b011, /* Funct7 */ 0b000'0001, Extentions::M);
rvdash::R_Instruction<RV32ISz>   DIV(/* Opcode */ 0b011'0011, /* Funct3 */ 0b100, /* Funct7 */ 0b000'0001, Extentions::M);
rvdash::R_Instruction<RV32ISz>  DIVU(/* Opcode */ 0b011'0011, /* Funct3 */ 0b101, /* Funct7 */ 0b000'0001, Extentions::M);
rvdash::R_Instruction<RV32ISz>   REM(/* Opcode */ 0b011'0011, /* Funct3 */ 0b110, /* Funct7 */ 0b000'0001, Extentions::M);
rvdash::R_Instruction<RV32ISz>  REMU(/* Opcode */ 0b011'0001, /* Funct3 */ 0b111, /* Funct7 */ 0b000'0001, Extentions::M);
i
//----------------------------------InstrDecoder-----------------------------------------

class InstrDecoder : public rvdash::InstrDecoder {
public:
  InstrDecoder() {};
  std::optional<std::shared_ptr<rvdash::Instruction<RV32ISz>>> tryDecode(Register Instr) const override {
    auto Opcode = rvdash::R_Instruction<RV32ISz>::extractOpcode(Instr);
    auto Funct3 = rvdash::R_Instruction<RV32ISz>::extractFunct3(Instr);
    auto Funct7 = rvdash::R_Instruction<RV32ISz>::extractFunct7(Instr);

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

class MInstrSet {
protected:
  std::shared_ptr<InstrDecoder> Decoder;
  std::shared_ptr<InstrExecutor> Executor;
  std::optional<std::shared_ptr<RegistersSet<AddrSpaceSz>>> Registers;


public:
  MInstrSet() {
    Decoder = std::make_shared<InstrDecoder>();
    Executor = std::make_shared<InstrExecutor>();
    Registers = std::make_shared<RegistersSet<RV32ISz>>(32);
    Registers.value()->addNamedRegister("pc");
    Registers.value()->getNamedRegister("pc") = 0;
  }

  ~MInstrSet() {};

  void print() const { std::cout << "MInstrSet\n"; }

  std::optional<std::shared_ptr<Instruction<AddrSpaceSz>>> tryDecode(Register Instr) const {
    return Decoder->tryDecode(Instr);
  }

};


std::ostream& operator<<(std::ostream& stream, const typename M::MInstrSet& counter)
{
    stream << "Value: ";
    counter.print();
    return stream;
}

} // namespace M
} // namespace rvdash

#endif// M_INSTRUCTION_SET_H

