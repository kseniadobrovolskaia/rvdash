#ifndef RV32I_INSTRUCTION_SET_H
#define RV32I_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace RV32I {

class RV32IInstrSet;

template <typename InstrSetType>
using FunctType = void (*)(Instruction, const InstrSetType &Set);

//-----------------------------extern_RV32I::Instructions--------------------------------

// extern Instruction SUB;
extern Instruction ADD;
// extern Instruction SUB;
// extern Instruction XOR;
// extern Instruction OR;
// extern Instruction AND;
// extern Instruction SLL;
// extern Instruction SRL;
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
// extern Instruction LB;
// extern Instruction LH;
extern Instruction LW;
// extern Instruction LBU;
// extern Instruction LHU;
//
// extern Instruction SB;
// extern Instruction SH;
// extern Instruction SW;
//
// extern Instruction BEQ;
// extern Instruction BNE;
// extern Instruction BLT;
// extern Instruction BGE;
// extern Instruction BLTU;
// extern Instruction BGEU;
//
// extern Instruction JAL;
// extern Instruction JALR;
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
               const InstrSetType &Set) const {
    Func(Instr, Set);
  }

//---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeADD(Instruction Instr, const InstrSetType &Set) {
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs2 = Instr.extractRs2();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Rs2Value = Registers->getRegister(Rs2).to_ulong();
    auto Result = Rs1Value + Rs2Value; 

    Registers->setRegister(Rd, Result);

    std::cout << "Execute ADD: ";
    std::cout << "Rd (X" << Rd << ") = " << Result << ", Rs1 (X" << Rs1 << ") = " << Rs1Value << ", Rs2 (X" << Rs2 << ") = " << Rs2Value
              << "\n\n";
  }

  template <typename InstrSetType>
  static void executeSUB(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeXOR(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeOR(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeAND(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLL(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSRL(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSRA(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLT(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLTU(Instruction Instr, const InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeADDI(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeXORI(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeORI(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeANDI(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLLI(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSRLI(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSRAI(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLTI(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSLTIU(Instruction Instr, const InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeLB(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeLH(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeLW(Instruction Instr, const InstrSetType &Set) {
    std::cout << "Execute LW: ";
    auto Rd = Instr.extractRd();
    auto Rs1 = Instr.extractRs1();
    auto Rs1Value = Registers->getRegister(Rs1).to_ulong();
    auto Imm = Instr.extractImm_11_0();
    auto ResultAddr = Rs1Value + Imm;
    Register<Instruction::Sz> Result;

    Set.getMemory().load(ResultAddr, 32, Result);

    Registers->setRegister(Rd, Result);

    std::cout << "Rd (X" << Rd << ") = " << Result << ", Rs1 (X" << Rs1 << ") = " << Rs1Value << ", Imm = " << Imm
              << "\n\n";
  }

  template <typename InstrSetType>
  static void executeLBU(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeLHU(Instruction Instr, const InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeSB(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSH(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeSW(Instruction Instr, const InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeBEQ(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBNE(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBLT(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBGE(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBLTU(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeBGEU(Instruction Instr, const InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeJAL(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeJALR(Instruction Instr, const InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeLUI(Instruction Instr, const InstrSetType &Set) {
    std::cout << "Execute LUI: ";
    auto Rd = Instr.extractRd();
    auto Imm = Instr.extractImm_31_12();

    Registers->setRegister(Rd, Imm);

    std::cout << "Rd (X" << Rd << ") = " << Imm << ", Imm = " << Imm
              << "\n\n";
  }

  template <typename InstrSetType>
  static void executeAUIPC(Instruction Instr, const InstrSetType &Set) {}

  //---------------------------------------------------------------------------------------

  template <typename InstrSetType>
  static void executeECALL(Instruction Instr, const InstrSetType &Set) {}

  template <typename InstrSetType>
  static void executeEBREAK(Instruction Instr, const InstrSetType &Set) {}

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
    InstrMap.emplace_back(LW, 0x0000707f,
                          &RV32IInstrExecutor::executeLW<InstrSetType>);
    InstrMap.emplace_back(LUI, 0x0000007f,
                          &RV32IInstrExecutor::executeLUI<InstrSetType>);
    InstrMap.emplace_back(EBREAK, 0xfff0707f,
                          &RV32IInstrExecutor::executeEBREAK<InstrSetType>);
    // to be continued


    return InstrMap;
  }

  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, FunctType<InstrSetType>>>
  tryDecode(Register<Instruction::Sz> Instr) const {
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
  RegistersSet<RegSz> Registers;
  RV32IInstrDecoder Decoder;
  std::shared_ptr<RV32IInstrExecutor> Executor;

public:

  RV32IInstrSet() : Registers("X", 31), Executor(RV32IInstrExecutor::getExecutorInstance(std::make_shared<RegistersSet<RegSz>>(Registers))) {
    Registers.addNamedRegister("pc");
    ++Registers.getNamedRegister("pc");
  }

  virtual ~RV32IInstrSet() {};

  Register<PcSz> *getPC() { return &Registers.getNamedRegister("pc"); }
  
  void dump(std::ostream &Stream) const { Stream << "RV32IInstrSet\n"; }

  void print() const { dump(std::cout); }

  template <typename InstrSetType>
  std::optional<std::tuple<Instruction, FunctType<InstrSetType>>>
  tryDecode(Register<Instruction::Sz> Instr,
            const InstrSetType &MainSet) const {
    return Decoder.tryDecode<InstrSetType>(Instr);
  }

  template <typename InstrSetType>
  bool tryExecute(Instruction Instr, FunctType<InstrSetType> Funct,
                  const InstrSetType &MainSet) const {
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
