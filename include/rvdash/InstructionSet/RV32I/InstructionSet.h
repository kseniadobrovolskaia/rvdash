#ifndef RV32I_INSTRUCTION_SET_H
#define RV32I_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace RV32I {

class RV32IInstrSet;

template <unsigned OldBits>
constexpr inline int32_t signExtend(uint32_t Value) {
  static_assert(OldBits > 0, "Bit width must be greater than 0");
  static_assert(OldBits <= 32, "Bit width out of range");
  return int32_t(Value << (32 - OldBits)) >> (64 - OldBits);
}

//--------------------------------RV32IInstrExecutor-------------------------------------

//
class RV32IInstrExecutor {
  static std::shared_ptr<RegistersSet<32>> Registers;
  static std::shared_ptr<RV32IInstrExecutor> SingleExecutor;

public:
  RV32IInstrExecutor() {};
  RV32IInstrExecutor(std::shared_ptr<RegistersSet<32>> Regs) {
    Registers = Regs;
  };
 
  static std::shared_ptr<RV32IInstrExecutor> getExecutorInstance(std::shared_ptr<RegistersSet<32>> Regs);

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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
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
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
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
    int Rs1Value = Registers->getRegister(Rs1).to_ulong();
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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lb "
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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lh "
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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lb "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().load(ResultAddr, /* Size */ 1, Result);
    Result = signExtend<CHAR_BIT>(Result.to_ulong());
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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lh "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().load(ResultAddr, /* Size */ Instruction::Sz / 2, Result);
    Result = signExtend<Instruction::Sz / 2>(Result.to_ulong());
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
    int Imm = signExtend<12>(Instr.extractImm_11_0());
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;
    Set.LogFile << "lw "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().load(ResultAddr, /* Size */ Instruction::Sz, Result);
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
    int Imm = signExtend<7>(Instr.extractImm_11_5());
    auto ResultAddr = Rs1Value + Imm;
    Set.LogFile << "sb "
                << "X" << int(Rs2) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().store(ResultAddr, /* Size */ 1, Registers->getRegister(Rs2),
                          Set.LogFile);
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
    int Imm = signExtend<7>(Instr.extractImm_11_5());
    auto ResultAddr = Rs1Value + Imm;
    Set.LogFile << "sh "
                << "X" << int(Rs2) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().store(ResultAddr, /* Size */ Instruction::Sz / 2,
                          Registers->getRegister(Rs2), Set.LogFile);
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
    int Imm = signExtend<7>(Instr.extractImm_11_5());
    auto ResultAddr = Rs1Value + Imm;
    Set.LogFile << "sw "
                << "X" << int(Rs2) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
    Set.getMemory().store(ResultAddr, /* Size */ Instruction::Sz,
                          Registers->getRegister(Rs2), Set.LogFile);
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
    int Imm = signExtend<13>(Instr.extractImm_J() << 1);
    auto OldPc = Registers->getNamedRegister("pc");
    auto DistAddr = OldPc.to_ulong() + Imm - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BEQ");
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
    int Imm = signExtend<13>(Instr.extractImm_J() << 1);
    auto OldPc = Registers->getNamedRegister("pc");
    auto DistAddr = OldPc.to_ulong() + Imm - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BNE");
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
    int Imm = signExtend<13>(Instr.extractImm_J() << 1);
    auto OldPc = Registers->getNamedRegister("pc");
    auto DistAddr = OldPc.to_ulong() + Imm - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BLT");
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
    int Imm = signExtend<13>(Instr.extractImm_J() << 1);
    auto OldPc = Registers->getNamedRegister("pc");
    auto DistAddr = OldPc.to_ulong() + Imm - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BGE");
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
    int Imm = signExtend<13>(Instr.extractImm_J() << 1);
    auto OldPc = Registers->getNamedRegister("pc");
    auto DistAddr = OldPc.to_ulong() + Imm - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BLTU");
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
    int Imm = signExtend<13>(Instr.extractImm_J() << 1);
    auto OldPc = Registers->getNamedRegister("pc");
    auto DistAddr = OldPc.to_ulong() + Imm - Instruction::Sz_b;
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned BGEU");
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
    int Imm = signExtend<21>(Instr.extractImm_J() << 1);
    auto OldPc = Registers->getNamedRegister("pc");
    auto RdValue = OldPc.to_ulong() + Instruction::Sz_b;
    auto DistAddr = Imm - Instruction::Sz_b;
    Registers->setRegister(Rd, RdValue, Set.LogFile);
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned JAL");
    Set.LogFile << "jal "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setNamedRegister("pc", DistAddr, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "jal from " << OldPc.to_ulong()
                << " to " << DistAddr + Instruction::Sz_b << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeJALR(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    int Imm = signExtend<12>(Instr.extractImm_11_0());
    auto Rs1 = Instr.extractRs1();
    auto OldPc = Registers->getNamedRegister("pc");
    auto RdValue = OldPc.to_ulong() + Instruction::Sz_b;
    auto DistAddr = int(Imm + Registers->getRegister(Rs1).to_ulong()) / 2 * 2 -
                    Instruction::Sz_b;
    Registers->setRegister(Rd, RdValue, Set.LogFile);
    if (DistAddr % Instruction::Sz_b != 0)
      failWithError("Misaligned JALR");
    Set.LogFile << "jalr "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
                << std::dec << int(Rs1) << ")\n";
    Set.LogFile << std::dec;
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
    int Imm = signExtend<20>(Instr.extractImm_31_12());
    Set.LogFile << "lui "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "\n";
    Set.LogFile << std::dec;
    Registers->setRegister(Rd, Imm, Set.LogFile);
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "rd (X" << int(Rd) << ") = " << Imm
                << ", Imm = " << Imm << "\n";
#endif
  }

  template <typename InstrSetType>
  static void executeAUIPC(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    std::bitset<32> ImmBitset = Instr.extractImm_31_12() << 12;
    auto Imm = ImmBitset.to_ulong();
    auto OldPc = Registers->getNamedRegister("pc").to_ulong();
    auto RdValue = OldPc + Imm;
    Registers->setRegister(Rd, RdValue, Set.LogFile);
    Set.LogFile << "auipc "
                << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "\n";
    Set.LogFile << std::dec;
#ifdef DEBUG
    Set.LogFile << "Debug: " << std::dec << "auipc addr = " << OldPc
                << ", Rd = " << RdValue << "\n";
#endif
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeECALL(Instruction Instr, InstrSetType &Set) {
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
    case 64: {
      auto Fd = Registers->getRegister(10).to_ulong();
      auto Ptr = Registers->getRegister(11).to_ulong();
      auto Size = Registers->getRegister(12).to_ulong();
      Set.LogFile << " write(" << Fd << ", " << Ptr << ", " << Size << ")\n";
      std::bitset<CHAR_BIT> Str;
      for (auto Byte = 0; Byte < Size; Byte++, Ptr++) {
        Set.getMemory().load(Ptr, /* Size */ 1, Str);
        write(Fd, reinterpret_cast<const void *>(&Str), 1);
      }
      break;
    }
    case 93: {
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

class RV32IInstrDecoder {

public:
  RV32IInstrDecoder() {};

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

class RV32IInstrSet {
public:
  static const size_t PcSz = 32;
  static const size_t RegSz = 32;

protected:
  std::shared_ptr<RegistersSet<RegSz>> Registers;
  RV32IInstrDecoder Decoder;
  std::shared_ptr<RV32IInstrExecutor> Executor;

public:
  RV32IInstrSet()
      : Registers(std::make_shared<RegistersSet<RegSz>>("X", 32)),
        Executor(RV32IInstrExecutor::getExecutorInstance(Registers)) {
    Registers->addNamedRegister("pc");
  }

  ~RV32IInstrSet(){};

  Register<PcSz> *getPC() { return &Registers->getNamedRegister("pc"); }

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
    Executor->execute(Instr, Funct, MainSet);
    return false;
  }
};

} // namespace RV32I
} // namespace rvdash

#endif // RV32I_INSTRUCTION_SET_H
