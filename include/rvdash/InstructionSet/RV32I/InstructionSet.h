#ifndef RV32I_INSTRUCTION_SET_H
#define RV32I_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"

namespace rvdash {
namespace RV32I {

class RV32IInstrSet;

template <unsigned OldBits, unsigned NewBits>
constexpr inline int32_t signExtend(uint32_t Value) {
  static_assert(NewBits > 0, "Bit width must be greater than 0");
  static_assert(OldBits <= NewBits, "Bit width out of range");
  return int32_t(Value << (NewBits - OldBits)) >> (NewBits - OldBits);
}

//--------------------------------RV32IRegistersSet--------------------------------------

class RV32IRegistersSet final : public RegistersSet<32> {

public:
  RV32IRegistersSet() : RegistersSet("X", 32) { OwnRegs[0] = 0; };

  void setRegister(unsigned RegIdx, const Register<32> &NewValue) override {
    if (RegIdx == 0)
      return;
    OwnRegs.at(RegIdx) = NewValue;
  }

  void setRegister(unsigned RegIdx, const Register<32> &NewValue,
                   std::ostream &LogFile) override {
    setRegister(RegIdx, NewValue);
    logChange(RegIdx, NewValue, LogFile);
  }
};

//--------------------------------RV32IInstrExecutor-------------------------------------

/**
 * @brief class RV32IInstrExecutor - singleton RV32I executor needed to
 *                                   contain static functions to execute
 *                                   all extension instructions.
 */
class RV32IInstrExecutor {
  static std::shared_ptr<RV32IRegistersSet> Registers;

  RV32IInstrExecutor() {};
  RV32IInstrExecutor(std::shared_ptr<RV32IRegistersSet> Regs) {
    Registers = Regs;
  };

public:
  static RV32IInstrExecutor &
  getExecutorInstance(std::shared_ptr<RV32IRegistersSet> Regs);

  template <typename InstrSetType>
  void execute(Instruction Instr, ExecuteFuncType<InstrSetType> Func,
               InstrSetType &Set) {
    Func(Instr, Set);
  }

//---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeADD(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Result = Rs1Value + Rs2Value;
    Set.LogFile << "add "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSUB(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Result = Rs1Value - Rs2Value;
    Set.LogFile << "sub "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeXOR(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    auto Rs2Value = Registers->getRegister(Rs2);
    auto Result = Rs1Value ^ Rs2Value;
    Set.LogFile << "xor "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeOR(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    auto Rs2Value = Registers->getRegister(Rs2);
    auto Result = Rs1Value | Rs2Value;
    Set.LogFile << "or "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeAND(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    auto Rs2Value = Registers->getRegister(Rs2);
    auto Result = Rs1Value & Rs2Value;
    Set.LogFile << "and "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result.to_ulong() << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value.to_ulong() << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value.to_ulong() << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSLL(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    std::bitset<5> Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Result = Rs1Value << Rs2Value.to_ulong();
    Set.LogFile << "sll "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result.to_ulong() << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value.to_ulong() << ", rs2(lower 5 bits) (X"
                << int(Rs2) << ") = " << Rs2Value.to_ulong() << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSRL(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    std::bitset<5> Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Result = Rs1Value >> Rs2Value.to_ulong();
    Set.LogFile << "slr "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2(lover 5 bits) (X" << int(Rs2)
                << ") = " << Rs2Value << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSRA(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    std::bitset<5> Rs2Value = Registers->getRegister(Rs2).to_ulong();
    std::bitset<Instruction::Sz> Result =
        int(Rs1Value.to_ulong()) >> Rs2Value.to_ulong();
    Set.LogFile << "sra "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2(lover 5 bits) (X" << int(Rs2)
                << ") = " << Rs2Value << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSLT(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
    int Rs2Value = Registers->getRegister(Rs2).to_ulong();
    bool Result = Rs1Value < Rs2Value;
    Set.LogFile << "slt "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, int(Result), Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSLTU(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    unsigned Rs1Value = Registers->getRegister(Rs1).to_ulong();
    unsigned Rs2Value = Registers->getRegister(Rs2).to_ulong();
    bool Result = Rs1Value < Rs2Value;
    Set.LogFile << "sltu "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
                << "\n";
    Registers->setRegister(Rd, int(Result), Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << "\n";
#endif
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeADDI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    if (Rd == 0 && Rs1 == 0 && Imm == 0) {
      Set.LogFile << "nop\n";
      return;
    }
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Result = Rs1Value + Imm;
    Set.LogFile << "addi "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeXORI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Result = Rs1Value ^ Imm;
    Set.LogFile << "xori "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeORI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Result = Rs1Value | Imm;
    Set.LogFile << "ori "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeANDI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Result = Rs1Value & Imm;
    Set.LogFile << "andi "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSLLI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    unsigned Rs1Value = Registers->getRegister(Rs1).to_ulong();
    std::bitset<5> Imm = Instr.extractImm_11_0();
    auto Result = Rs1Value << Imm.to_ulong();
    Set.LogFile << "slli "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", 0x" << std::hex
                << Imm.to_ulong() << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value
                << ", Imm (lower 5 bits) = " << Imm.to_ulong() << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSRLI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    unsigned Rs1Value = Registers->getRegister(Rs1).to_ulong();
    std::bitset<5> Imm = Instr.extractImm_11_0();
    auto Result = Rs1Value >> Imm.to_ulong();
    Set.LogFile << "srli "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", 0x" << std::hex
                << Imm.to_ulong() << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value
                << ", Imm (lower 5 bits) = " << Imm.to_ulong() << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSRAI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
    std::bitset<5> Imm = Instr.extractImm_11_0();
    auto Result = int(Rs1Value) >> Imm.to_ulong();
    Set.LogFile << "srai "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", 0x" << std::hex
                << Imm.to_ulong() << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value
                << ", Imm (lower 5 bits) = " << Imm.to_ulong() << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSLTI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
    bool Result = Rs1Value < Imm;
    Set.LogFile << "slti "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, int(Result), Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSLTIU(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    unsigned Imm = Instr.extractImm_11_0();
    unsigned Rs1Value = Registers->getRegister(Rs1).to_ulong();
    bool Result = Rs1Value < Imm;
    Set.LogFile << "sltiu "
                << "X" << int(Rd) << ", X" << int(Rs1) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, int(Result), Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeLBU(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lbu "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().load(ResultAddr, /* Size */ 1, Result);
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeLHU(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lhu "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().load(ResultAddr, /* Size */ 2, Result);
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeLB(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lb "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().load(ResultAddr, /* Size */ 1, Result);
    Result = signExtend<CHAR_BIT, 32>(Result.to_ulong());
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeLH(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lh "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().load(ResultAddr, /* Size */ 2, Result);
    Result = signExtend<2 * CHAR_BIT, 32>(Result.to_ulong());
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeLW(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lw "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().load(ResultAddr, /* Size */ 4, Result);
    Registers->setRegister(Rd, Result, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd)
                << ") = " << Result << ", rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", Imm = " << Imm << "\n";
#endif
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeSB(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    int Imm = signExtend<12, 32>(Instr.extractImm_S());
    auto ResultAddr = Rs1Value + Imm;
    Set.LogFile << "sb "
                << "X" << int(Rs2) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().store(
        ResultAddr, /* Size */ 1,
        std::bitset<8>(Registers->getRegister(Rs2).to_ulong()), Set.LogFile);
#ifdef DEBUG
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    Set.LogFile << "Debug: " << std::dec << "rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSH(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    int Imm = signExtend<12, 32>(Instr.extractImm_S());
    auto ResultAddr = Rs1Value + Imm;
    Set.LogFile << "sh "
                << "X" << int(Rs2) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().store(
        ResultAddr, /* Size */ 2,
        std::bitset<16>(Registers->getRegister(Rs2).to_ulong()), Set.LogFile);
#ifdef DEBUG
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    Set.LogFile << "Debug: " << std::dec << "rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeSW(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    int Imm = signExtend<12, 32>(Instr.extractImm_S());
    auto ResultAddr = Rs1Value + Imm;
    Set.LogFile << "sw "
                << "X" << int(Rs2) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().store(ResultAddr, /* Size */ 4, Registers->getRegister(Rs2),
                          Set.LogFile);
#ifdef DEBUG
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    Set.LogFile << "Debug: " << std::dec << "rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << ", Imm = " << Imm << "\n";
#endif
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeBEQ(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_B());
    auto OldPc = Registers->getNamedRegister("pc");
    int DistAddr = OldPc.to_ulong() + (Imm << 1) - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BEQ : " + std::to_string(DistAddr));
    Set.LogFile << "beq "
                << "X" << int(Rs1) << ", X" << int(Rs2) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    if (Rs1Value == Rs2Value)
      Registers->setNamedRegister("pc", DistAddr, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << ", Imm (offset) = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeBNE(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_B());
    auto OldPc = Registers->getNamedRegister("pc");
    int DistAddr = OldPc.to_ulong() + (Imm << 1) - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BNE: " + std::to_string(DistAddr));
    Set.LogFile << "bne "
                << "X" << int(Rs1) << ", X" << int(Rs2) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    if (Rs1Value != Rs2Value)
      Registers->setNamedRegister("pc", DistAddr, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << ", Imm (offset) = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeBLT(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    int Rs2Value = Registers->getRegister(Rs2).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_B());
    auto OldPc = Registers->getNamedRegister("pc");
    int DistAddr = OldPc.to_ulong() + (Imm << 1) - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BLT: " + std::to_string(DistAddr));
    Set.LogFile << "blt "
                << "X" << int(Rs1) << ", X" << int(Rs2) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    if (Rs1Value < Rs2Value)
      Registers->setNamedRegister("pc", DistAddr, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << ", Imm (offset) = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeBGE(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    int Rs2Value = Registers->getRegister(Rs2).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_B());
    auto OldPc = Registers->getNamedRegister("pc");
    int DistAddr = OldPc.to_ulong() + (Imm << 1) - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BGE: " + std::to_string(DistAddr));
    Set.LogFile << "bge "
                << "X" << int(Rs1) << ", X" << int(Rs2) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    if (Rs1Value >= Rs2Value)
      Registers->setNamedRegister("pc", DistAddr, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << ", Imm (offset) = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeBLTU(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    unsigned Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    unsigned Rs2Value = Registers->getRegister(Rs2).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_B());
    auto OldPc = Registers->getNamedRegister("pc");
    int DistAddr = OldPc.to_ulong() + (Imm << 1) - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BLTU: " + std::to_string(DistAddr));
    Set.LogFile << "bltu "
                << "X" << int(Rs1) << ", X" << int(Rs2) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    if (Rs1Value < Rs2Value)
      Registers->setNamedRegister("pc", DistAddr, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << ", Imm (offset) = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeBGEU(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    unsigned Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    unsigned Rs2Value = Registers->getRegister(Rs2).to_ulong();
    int Imm = signExtend<12, 32>(Instr.extractImm_B());
    auto OldPc = Registers->getNamedRegister("pc");
    int DistAddr = OldPc.to_ulong() + (Imm << 1) - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BGEU: " + std::to_string(DistAddr));
    Set.LogFile << "bgeu "
                << "X" << int(Rs1) << ", X" << int(Rs2) << ", 0x" << std::hex
                << Imm << "\n";
    Set.LogFile << std::dec;
    if (Rs1Value >= Rs2Value)
      Registers->setNamedRegister("pc", DistAddr, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rs1 (X" << int(Rs1)
                << ") = " << Rs1Value << ", rs2 (X" << int(Rs2)
                << ") = " << Rs2Value << ", Imm (offset) = " << Imm << "\n";
#endif
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeJAL(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    int Imm = signExtend<20, 32>(Instr.extractImm_J());
    auto OldPc = Registers->getNamedRegister("pc");
    auto RdValue = OldPc.to_ulong() + Instruction::Sz_b;
    int DistAddr = OldPc.to_ulong() + (Imm << 1) - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned JAL: " + std::to_string(DistAddr));
    Set.LogFile << "jal "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, RdValue, Set.LogFile);
    Registers->setNamedRegister("pc", DistAddr, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "jal from " << OldPc.to_ulong()
                << " to " << DistAddr + Instruction::Sz_b << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeJALR(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    int Imm = signExtend<12, 32>(Instr.extractImm_11_0());
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto OldPc = Registers->getNamedRegister("pc");
    auto RdValue = OldPc.to_ulong() + Instruction::Sz_b;
    int DistAddr = (Imm + Rs1Value) / 2 * 2 - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned JALR: " + std::to_string(DistAddr));
    Set.LogFile << "jalr "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, RdValue, Set.LogFile);
    Registers->setNamedRegister("pc", DistAddr, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "jalr from " << OldPc.to_ulong()
                << " to " << DistAddr + Instruction::Sz_b << "\n";
#endif
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeLUI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    int Imm = Instr.extractImm_31_12();
    Set.LogFile << "lui "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, Imm << 12, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd) << ") = " << Imm
                << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeAUIPC(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    int AsmImm = Instr.extractImm_31_12();
    auto Imm = AsmImm << 12;
    auto OldPc = Registers->getNamedRegister("pc").to_ulong();
    auto RdValue = OldPc + Imm;
    Set.LogFile << "auipc "
                << "X" << int(Rd) << ", " << std::hex << "0x" << AsmImm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, RdValue, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "auipc addr = " << OldPc
                << ", Rd = " << RdValue << "\n";
#endif
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeECALL(Instruction Instr, InstrSetType &Set) {
    enum { WRITE_SYSCALL = 64, EXIT_SYSCALL = 93 };

    Set.LogFile << "ecall";
    // In RISC-V the ecall instruction is used to do system call.
    // Calling convention for syscalls:
    //
    //    Syscall arguments are loaded into a0-a5 (X10 - X15)
    //    Syscall number is loaded into a7 (X17)
    //
    auto SysNum = Registers->getRegister(17).to_ulong();
    // Write system call
    switch (SysNum) {
    case WRITE_SYSCALL: {
      auto Fd = Registers->getRegister(10).to_ulong();
      auto Ptr = Registers->getRegister(11).to_ulong();
      auto Size = Registers->getRegister(12).to_ulong();
      Set.LogFile << " write(" << Fd << ", " << Ptr << ", " << Size << ")\n";
      std::bitset<CHAR_BIT> Str;
      for (auto Byte = 0; Byte < static_cast<int>(Size); Byte++, Ptr++) {
        Set.getMemory().load(Ptr, /* Size */ 1, Str);
        write(Fd, reinterpret_cast<const void *>(&Str), 1);
      }
      break;
    }
    case EXIT_SYSCALL: {
      auto ErCode = Registers->getRegister(10).to_ulong();
      Set.LogFile << " exit(" << ErCode << ")\n";
      Set.stop();
      break;
    }
    default:
      failWithError("Unknown syscall number " + std::to_string(SysNum));
    }
  }

  template <typename InstrSetType>
  static void executeEBREAK(Instruction Instr, InstrSetType &Set) {
    Set.LogFile << "ebreak\n";
    Set.stop();
  }
};

//--------------------------------RV32IInstrDecoder--------------------------------------

/**
 * @brief class RV32IInstrDecoder - singleton RV32I decoder works like this:
 *                                  1. Table of all instructions is filled in.
 *                                  2. When a new instruction needs to be
 *                                     decoded, masks of all instructions are
 *                                     sequentially applied to it
 *                                  3. If no matches are found, then
 *                                     std::nullopt is returned.
 */
class RV32IInstrDecoder {

  RV32IInstrDecoder() {};

public:
  static RV32IInstrDecoder &getDecoderInstance();

  template <typename InstrSetType>
  struct InstMapElem {
    Instruction Instr;
    std::bitset<Instruction::Sz> Mask;
    ExecuteFuncType<InstrSetType> Func;
  };
  
  template <typename InstrSetType>
  static std::vector<InstMapElem<InstrSetType>> registerInstrs() {
    std::vector<InstMapElem<InstrSetType>> InstrMap;

#define ADD_INSTR(Name, Instr, Mask, EncodingType)                             \
  Instruction Name(Instr, InstrEncodingType::EncodingType, Extensions::RV32I); \
  InstrMap.emplace_back(Name, Mask,                                            \
                        &RV32IInstrExecutor::execute##Name<InstrSetType>);
#include "DefineInstrs.h"
#undef ADD_INSTR

    return InstrMap;
  }

  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, ExecuteFuncType<InstrSetType>>>
  tryDecode(Register<Instruction::Sz> Instr) {
    static std::vector<InstMapElem<InstrSetType>> InstrMap = registerInstrs<InstrSetType>();

    for (auto &SetInstr : InstrMap) {
      if (isSame(Instr, SetInstr.Instr.Bits, SetInstr.Mask))
        return std::tuple{
            Instruction(Instr, SetInstr.Instr.Type, Extensions::RV32I),
            SetInstr.Func};
    }
    return std::nullopt;
  }
};

//----------------------------------RV32IInstrSet----------------------------------------

/**
 * @brief class RV32IInstrSet - this is the base set RV32I. It contains 32
 *                              32-bit X-registers, 32-bit PC and 39
 *                              instructions. The Decoder and Executor are
 *                              responsible for decoding and execution.
 */
class RV32IInstrSet {

  std::shared_ptr<RV32IRegistersSet> Registers;
  RV32IInstrDecoder &Decoder;
  RV32IInstrExecutor &Executor;

public:
  RV32IInstrSet()
      : Registers(std::make_shared<RV32IRegistersSet>()),
        Decoder(RV32IInstrDecoder::getDecoderInstance()),
        Executor(RV32IInstrExecutor::getExecutorInstance(Registers)) {
    Registers->addNamedRegister("pc");
  }

  Register<32> *getPC() { return &Registers->getNamedRegister("pc"); }

  void dump(std::ostream &Stream) const {
    Stream << "\nRV32IInstrSet:\n";
    Registers->dump(Stream);
  }
  void print() const { dump(std::cout); }

  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, ExecuteFuncType<InstrSetType>>>
  tryDecode(Register<Instruction::Sz> Instr, InstrSetType &MainSet) {
    return Decoder.tryDecode<InstrSetType>(Instr);
  }

  template <typename InstrSetType>
  bool tryExecute(Instruction Instr, ExecuteFuncType<InstrSetType> Funct,
                  InstrSetType &MainSet) {
    if (Instr.Ex != Extensions::RV32I)
      return true;
    Executor.execute(Instr, Funct, MainSet);
    return false;
  }
};

} // namespace RV32I
} // namespace rvdash

#endif // RV32I_INSTRUCTION_SET_H
