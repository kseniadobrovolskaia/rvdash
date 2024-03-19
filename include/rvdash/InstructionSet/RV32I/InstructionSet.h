#ifndef RV32I_INSTRUCTION_SET_H
#define RV32I_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace RV32I {

class RV32IInstrSet;

template <typename InstrSetType>
using FunctType = void (*)(Instruction, InstrSetType &Set);

//-----------------------------extern_RV32I::Instructions--------------------------------

extern Instruction ADD;
extern Instruction SUB;
extern Instruction XOR;
extern Instruction OR;
extern Instruction AND;
extern Instruction SLL;
extern Instruction SRL;
// extern Instruction SRA;
// extern Instruction SLT;
// extern Instruction SLTU;
//
// extern Instruction ADDI;
// extern Instruction XORI;
// extern Instruction ORI;
// extern Instruction ANDI;
// extern Instruction SLLI;
// extern Instruction SRLI;
// extern Instruction SRAI;
// extern Instruction SLTI;
// extern Instruction SLTIU;
//
extern Instruction LB;
extern Instruction LH;
extern Instruction LW;
// extern Instruction LBU;
// extern Instruction LHU;
//
extern Instruction SB;
extern Instruction SH;
extern Instruction SW;

// extern Instruction BEQ;
// extern Instruction BNE;
// extern Instruction BLT;
// extern Instruction BGE;
// extern Instruction BLTU;
// extern Instruction BGEU;
//
extern Instruction JAL;
extern Instruction JALR;
//
extern Instruction LUI;
// extern Instruction AUIPC;
//
// extern Instruction ECALL;
extern Instruction EBREAK;

//--------------------------------RV32IInstrExecutor-------------------------------------

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
  void execute(Instruction Instr, FunctType<InstrSetType> Func,
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

    std::cout << "add "
              << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
              << "\n";
    Registers->setRegister(Rd, Result);

    std::cout << "Debug: " << std::dec << "rd (X" << int(Rd) << ") = " << Result
              << ", rs1 (X" << int(Rs1) << ") = " << Rs1Value << ", rs2 (X"
              << int(Rs2) << ") = " << Rs2Value << "\n";
  }

  template <typename InstrSetType>
  static void executeSUB(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Result = Rs1Value - Rs2Value;

    std::cout << "sub "
              << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
              << "\n";
    Registers->setRegister(Rd, Result);

    std::cout << "Debug: " << std::dec << "rd (X" << int(Rd) << ") = " << Result
              << ", rs1 (X" << int(Rs1) << ") = " << Rs1Value << ", rs2 (X"
              << int(Rs2) << ") = " << Rs2Value << "\n";
  }

  template <typename InstrSetType>
  static void executeXOR(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    auto Rs2Value = Registers->getRegister(Rs2);
    auto Result = Rs1Value ^ Rs2Value;

    std::cout << "xor "
              << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
              << "\n";
    Registers->setRegister(Rd, Result);

    std::cout << "Debug: " << std::dec << "rd (X" << int(Rd) << ") = " << Result
              << ", rs1 (X" << int(Rs1) << ") = " << Rs1Value << ", rs2 (X"
              << int(Rs2) << ") = " << Rs2Value << "\n";
  }

  template <typename InstrSetType>
  static void executeOR(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    auto Rs2Value = Registers->getRegister(Rs2);
    auto Result = Rs1Value | Rs2Value;

    std::cout << "or "
              << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
              << "\n";
    Registers->setRegister(Rd, Result);

    std::cout << "Debug: " << std::dec << "rd (X" << int(Rd) << ") = " << Result
              << ", rs1 (X" << int(Rs1) << ") = " << Rs1Value << ", rs2 (X"
              << int(Rs2) << ") = " << Rs2Value << "\n";
  }

  template <typename InstrSetType>
  static void executeAND(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    auto Rs2Value = Registers->getRegister(Rs2);
    auto Result = Rs1Value & Rs2Value;

    std::cout << "and "
              << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
              << "\n";
    Registers->setRegister(Rd, Result);

    std::cout << "Debug: " << std::dec << "rd (X" << int(Rd)
              << ") = " << Result.to_ulong() << ", rs1 (X" << int(Rs1)
              << ") = " << Rs1Value.to_ulong() << ", rs2 (X" << int(Rs2)
              << ") = " << Rs2Value.to_ulong() << "\n";
  }

  template <typename InstrSetType>
  static void executeSLL(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    std::bitset<5> Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Result = Rs1Value << Rs2Value.to_ulong();

    std::cout << "sll "
              << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
              << "\n";
    Registers->setRegister(Rd, Result);

    std::cout << "Debug: " << std::dec << "rd (X" << int(Rd)
              << ") = " << Result.to_ulong() << ", rs1 (X" << int(Rs1)
              << ") = " << Rs1Value.to_ulong() << ", rs2(lower 5 bits) (X"
              << int(Rs2) << ") = " << Rs2Value.to_ulong() << "\n";
  }

  template <typename InstrSetType>
  static void executeSRL(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1);
    std::bitset<5> Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Result = Rs1Value >> Rs2Value.to_ulong();

    std::cout << "slr "
              << "X" << int(Rd) << ", X" << int(Rs1) << ", X" << int(Rs2)
              << "\n";
    Registers->setRegister(Rd, Result);

    std::cout << "Debug: " << std::dec << "rd (X" << int(Rd) << ") = " << Result
              << ", rs1 (X" << int(Rs1) << ") = " << Rs1Value
              << ", rs2(lover 5 bits) (X" << int(Rs2) << ") = " << Rs2Value
              << "\n";
  }

  template <typename InstrSetType>
  static void executeSRA(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLT(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLTU(Instruction Instr, InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeADDI(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeXORI(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeORI(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeANDI(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLLI(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSRLI(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSRAI(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLTI(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLTIU(Instruction Instr, InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeLB(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Imm = Instr.extractImm_11_0();
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;

    std::cout << "lb "
              << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
              << std::dec << int(Rs1) << ")\n";
    std::cout << std::dec;
    Set.getMemory().load(ResultAddr, 8, Result);
    Registers->setRegister(Rd, Result);
  }

  template <typename InstrSetType>
  static void executeLH(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Imm = Instr.extractImm_11_0();
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;

    std::cout << "lh "
              << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
              << std::dec << int(Rs1) << ")\n";
    std::cout << std::dec;
    Set.getMemory().load(ResultAddr, 16, Result);
    Registers->setRegister(Rd, Result);
  }

  template <typename InstrSetType>
  static void executeLW(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Imm = Instr.extractImm_11_0();
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;

    std::cout << "lw "
              << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
              << std::dec << int(Rs1) << ")\n";
    std::cout << std::dec;
    Set.getMemory().load(ResultAddr, 32, Result);
    Registers->setRegister(Rd, Result);
  }

  template <typename InstrSetType>
  static void executeLBU(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeLHU(Instruction Instr, InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeSB(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Imm = Instr.extractImm_11_5();
    auto ResultAddr = Rs1Value + Imm;

    std::cout << "sb "
              << "X" << int(Rs2) << ", " << std::hex << "0x" << Imm << "(X"
              << std::dec << int(Rs1) << ")\n";
    std::cout << std::dec;
    Set.getMemory().store(ResultAddr, 8, Registers->getRegister(Rs2));
  }

  template <typename InstrSetType>
  static void executeSH(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Imm = Instr.extractImm_11_5();
    auto ResultAddr = Rs1Value + Imm;

    std::cout << "sh "
              << "X" << int(Rs2) << ", " << std::hex << "0x" << Imm << "(X"
              << std::dec << int(Rs1) << ")\n";
    std::cout << std::dec;
    Set.getMemory().store(ResultAddr, 16, Registers->getRegister(Rs2));
  }

  template <typename InstrSetType>
  static void executeSW(Instruction Instr, InstrSetType &Set) {
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2 = Instr.extractRs2();
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Imm = Instr.extractImm_11_5();
    auto ResultAddr = Rs1Value + Imm;

    std::cout << "sw "
              << "X" << int(Rs2) << ", " << std::hex << "0x" << Imm << "(X"
              << std::dec << int(Rs1) << ")\n";
    std::cout << std::dec;
    Set.getMemory().store(ResultAddr, 32, Registers->getRegister(Rs2));
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeBEQ(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBNE(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBLT(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBGE(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBLTU(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBGEU(Instruction Instr, InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeJAL(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Imm = Instr.extractImm_J();
    auto OldPc = Registers->getNamedRegister("pc");
    auto RdValue = OldPc.to_ulong() + Instruction::Sz;
    auto DistAddr = Imm - Instruction::Sz;
    Registers->setRegister(Rd, RdValue);

    if (DistAddr % Instruction::Sz != 0)
      failWithError("Misaligned JAL");
    std::cout << "jal "
              << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "\n";
    std::cout << std::dec;
    std::cout << "Debug: " << std::dec << "jal from " << OldPc.to_ulong()
              << " to " << DistAddr + Instruction::Sz << "\n";
    Registers->setNamedRegister("pc", DistAddr);
  }

  template <typename InstrSetType>
  static void executeJALR(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Imm = Instr.extractImm_11_0();
    auto Rs1 = Instr.extractRs1();
    auto OldPc = Registers->getNamedRegister("pc");
    auto RdValue = OldPc.to_ulong() + Instruction::Sz;
    auto DistAddr =
        Imm + Registers->getRegister(Rs1).to_ulong() - Instruction::Sz;
    Registers->setRegister(Rd, RdValue);

    if (DistAddr % Instruction::Sz != 0)
      failWithError("Misaligned JALR");
    std::cout << "jalr "
              << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "(X"
              << std::dec << int(Rs1) << ")\n";
    std::cout << std::dec;
    std::cout << "Debug: " << std::dec << "jalr from " << OldPc.to_ulong()
              << " to " << DistAddr + Instruction::Sz << "\n";
    Registers->setNamedRegister("pc", DistAddr);
  }

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeLUI(Instruction Instr, InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Imm = Instr.extractImm_31_12();

    std::cout << "lui "
              << "X" << int(Rd) << ", " << std::hex << "0x" << Imm << "\n";
    std::cout << std::dec;
    std::cout << "Debug: " << std::dec << "rd (X" << int(Rd) << ") = " << Imm
              << ", Imm = " << Imm << "\n";
    Registers->setRegister(Rd, Imm);
  }

  template <typename InstrSetType>
  static void executeAUIPC(Instruction Instr, InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeECALL(Instruction Instr, InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeEBREAK(Instruction Instr, InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

};

//--------------------------------RV32IInstrDecoder--------------------------------------

class RV32IInstrDecoder {

public:
  RV32IInstrDecoder() {};

  template <typename InstrSetType>
  struct InstMapElem {
    Instruction Instr;
    std::bitset<Instruction::Sz> Mask;
    FunctType<InstrSetType> Func;
  };
  
  template <typename InstrSetType>
  static std::vector<InstMapElem<InstrSetType>> registerInstrs() {
    std::vector<InstMapElem<InstrSetType>> InstrMap;

    InstrMap.emplace_back(ADD, 0xfe00707f,
                          &RV32IInstrExecutor::executeADD<InstrSetType>);
    InstrMap.emplace_back(SUB, 0xfe00707f,
                          &RV32IInstrExecutor::executeSUB<InstrSetType>);
    InstrMap.emplace_back(XOR, 0xfe00707f,
                          &RV32IInstrExecutor::executeXOR<InstrSetType>);
    InstrMap.emplace_back(OR, 0xfe00707f,
                          &RV32IInstrExecutor::executeOR<InstrSetType>);
    InstrMap.emplace_back(AND, 0xfe00707f,
                          &RV32IInstrExecutor::executeAND<InstrSetType>);
    InstrMap.emplace_back(SLL, 0xfe00707f,
                          &RV32IInstrExecutor::executeSLL<InstrSetType>);
    InstrMap.emplace_back(SRL, 0xfe00707f,
                          &RV32IInstrExecutor::executeSRL<InstrSetType>);

    InstrMap.emplace_back(LB, 0x0000707f,
                          &RV32IInstrExecutor::executeLB<InstrSetType>);
    InstrMap.emplace_back(LH, 0x0000707f,
                          &RV32IInstrExecutor::executeLH<InstrSetType>);
    InstrMap.emplace_back(LW, 0x0000707f,
                          &RV32IInstrExecutor::executeLW<InstrSetType>);

    InstrMap.emplace_back(LUI, 0x0000007f,
                          &RV32IInstrExecutor::executeLUI<InstrSetType>);

    InstrMap.emplace_back(EBREAK, 0xfff0707f,
                          &RV32IInstrExecutor::executeEBREAK<InstrSetType>);

    InstrMap.emplace_back(SB, 0x0000707f,
                          &RV32IInstrExecutor::executeSB<InstrSetType>);
    InstrMap.emplace_back(SH, 0x0000707f,
                          &RV32IInstrExecutor::executeSH<InstrSetType>);
    InstrMap.emplace_back(SW, 0x0000707f,
                          &RV32IInstrExecutor::executeSW<InstrSetType>);

    InstrMap.emplace_back(JAL, 0x0000007f,
                          &RV32IInstrExecutor::executeJAL<InstrSetType>);
    InstrMap.emplace_back(JALR, 0x00000067,
                          &RV32IInstrExecutor::executeJALR<InstrSetType>);
    // to be continued


    return InstrMap;
  }

  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, FunctType<InstrSetType>>>
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

  virtual ~RV32IInstrSet() {};

  Register<PcSz> *getPC() { return &Registers->getNamedRegister("pc"); }

  void dump(std::ostream &Stream) const {
    Stream << "\nRV32IInstrSet:\n";
    Registers->dump(Stream);
  }

  void print() const { dump(std::cout); }

  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, FunctType<InstrSetType>>>
  tryDecode(Register<Instruction::Sz> Instr, InstrSetType &MainSet) {
    return Decoder.tryDecode<InstrSetType>(Instr);
  }

  template <typename InstrSetType>
  bool tryExecute(Instruction Instr, FunctType<InstrSetType> Funct,
                  InstrSetType &MainSet) {
    if (Instr.Ex != Extensions::RV32I)
      return true;
    Executor->execute(Instr, Funct, MainSet);
    return false;
  }
};

std::ostream &operator<<(std::ostream &Stream,
                         const typename RV32I::RV32IInstrSet &Set);

} // namespace RV32I
} // namespace rvdash

#endif // RV32I_INSTRUCTION_SET_H
