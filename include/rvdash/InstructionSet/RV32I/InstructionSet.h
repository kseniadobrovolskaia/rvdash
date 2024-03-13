#ifndef RV32I_INSTRUCTION_SET_H
#define RV32I_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace RV32I {

using FunctTypes =
    std::variant<void (*)(std::shared_ptr<Instruction>), void (*)(int, int)>;

//-----------------------------extern_RV32I::Instructions--------------------------------

extern R_Instruction SUB;
extern R_Instruction ADD;
extern R_Instruction SUB;
extern R_Instruction XOR;
extern R_Instruction OR;
extern R_Instruction AND;
extern R_Instruction SLL;
extern R_Instruction SRL;
extern R_Instruction SRA;
extern R_Instruction SLT;
extern R_Instruction SLTU;

extern I_Instruction ADDI;
extern I_Instruction XORI;
extern I_Instruction ORI;
extern I_Instruction ANDI;
extern I_Instruction SLLI;
extern I_Instruction SRLI;
extern I_Instruction SRAI;
extern I_Instruction SLTI;
extern I_Instruction SLTIU;

extern I_Instruction LB;
extern I_Instruction LH;
extern I_Instruction LW;
extern I_Instruction LBU;
extern I_Instruction LHU;

extern S_Instruction SB;
extern S_Instruction SH;
extern S_Instruction SW;

extern B_Instruction BEQ;
extern B_Instruction BNE;
extern B_Instruction BLT;
extern B_Instruction BGE;
extern B_Instruction BLTU;
extern B_Instruction BGEU;

extern J_Instruction JAL;
extern I_Instruction JALR;

extern U_Instruction LUI;
extern U_Instruction AUIPC;

//--------------------------------RV32IInstrExecutor-------------------------------------

class RV32IInstrExecutor {
  static RegistersSet<32> Registers;

public:
  RV32IInstrExecutor(){};

  static void executeR_Instr(const R_Instruction &Instr, int);
  static void executeI_Instr();
  static void executeS_Instr();
  static void executeB_Instr();
  static void executeU_Instr();
  static void executeJ_Instr();

  void execute(std::shared_ptr<Instruction> Instr, FunctTypes Func) const {
    std::cout << "RV32I execute :";
    Instr->print();
    std::cout << "\n";

    switch (Instr->getType()) {
    case (InstrEncodingType::R):
      std::get<0>(Func)(Instr);
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

  static void executeADD(std::shared_ptr<Instruction> Instr);
  static void executeSUB(std::shared_ptr<Instruction> Instr);
  static void executeXOR(std::shared_ptr<Instruction> Instr);
  static void executeOR(std::shared_ptr<Instruction> Instr);
  static void executeAND(std::shared_ptr<Instruction> Instr);
  static void executeSLL(std::shared_ptr<Instruction> Instr);
  static void executeSRL(std::shared_ptr<Instruction> Instr);
  static void executeSRA(std::shared_ptr<Instruction> Instr);
  static void executeSLT(std::shared_ptr<Instruction> Instr);
  static void executeSLTU(std::shared_ptr<Instruction> Instr);
};

//--------------------------------RV32IInstrDecoder--------------------------------------

class RV32IInstrDecoder {
public:
  RV32IInstrDecoder(){};
  std::optional<std::tuple<std::shared_ptr<rvdash::Instruction>, FunctTypes>>
  tryDecode(Register<Instruction::Sz> Instr) const {
    auto Opcode = rvdash::Instruction::extractOpcode(Instr);
    auto Funct3 = rvdash::Instruction::extractFunct3(Instr);
    auto Funct7 = rvdash::Instruction::extractFunct7(Instr);

    // R_Instruction
    if (Opcode == 0b011'0011) {
      if (Funct7 == 0b010'0000) {
        switch (Funct3) {
        case (0b000):
          return std::tuple{std::make_shared<rvdash::R_Instruction>(SUB, Instr),
                            RV32IInstrExecutor::executeSUB};
        case (0b101):
          return std::tuple{std::make_shared<rvdash::R_Instruction>(SRA, Instr),
                            &RV32IInstrExecutor::executeSRA};
        default:
          return std::nullopt;
        }
      }
      if (Funct7 != 0b000'0000)
        return std::nullopt;
      switch (Funct3) {
      case (0b000):
        return std::tuple{std::make_shared<rvdash::R_Instruction>(ADD, Instr),
                          &RV32IInstrExecutor::executeADD};
      case (0b100):
        return std::tuple{std::make_shared<rvdash::R_Instruction>(XOR, Instr),
                          &RV32IInstrExecutor::executeXOR};
      case (0b110):
        return std::tuple{std::make_shared<rvdash::R_Instruction>(OR, Instr),
                          &RV32IInstrExecutor::executeOR};
      case (0b111):
        return std::tuple{std::make_shared<rvdash::R_Instruction>(AND, Instr),
                          &RV32IInstrExecutor::executeAND};
      case (0b001):
        return std::tuple{std::make_shared<rvdash::R_Instruction>(SLL, Instr),
                          &RV32IInstrExecutor::executeSLL};
      case (0b101):
        return std::tuple{std::make_shared<rvdash::R_Instruction>(SRL, Instr),
                          &RV32IInstrExecutor::executeSRL};
      case (0b010):
        return std::tuple{std::make_shared<rvdash::R_Instruction>(SLT, Instr),
                          &RV32IInstrExecutor::executeSLT};
      case (0b011):
        return std::tuple{std::make_shared<rvdash::R_Instruction>(SLTU, Instr),
                          &RV32IInstrExecutor::executeSLTU};
      default:
        return std::nullopt;
      }
    }
    // I_Instruction without ECALL, EBREAK, JALR and loads
    else if (Opcode == 0b001'0011) {
      switch (Funct3) {
      case (0b000):
        return std::tuple{std::make_shared<rvdash::I_Instruction>(ADDI, Instr),
                          &RV32IInstrExecutor::executeADD};
      case (0b100):
        return std::tuple{std::make_shared<rvdash::I_Instruction>(XORI, Instr),
                          &RV32IInstrExecutor::executeADD};
      case (0b110):
        return std::tuple{std::make_shared<rvdash::I_Instruction>(ORI, Instr),
                          &RV32IInstrExecutor::executeADD};
      case (0b111):
        return std::tuple{std::make_shared<rvdash::I_Instruction>(ANDI, Instr),
                          &RV32IInstrExecutor::executeADD};
      case (0b001): {
        auto Imm = rvdash::Instruction::extractImm_11_0(Instr);
        switch (Imm) {
        case (0b0):
          return std::tuple{
              std::make_shared<rvdash::I_Instruction>(SLLI, Instr),
              &RV32IInstrExecutor::executeADD};
        default:
          return std::nullopt;
        }
      }
      case (0b101): {
        auto Imm = rvdash::Instruction::extractImm_11_0(Instr);
        switch (Imm) {
        case (0b0):
          return std::tuple{
              std::make_shared<rvdash::I_Instruction>(SRLI, Instr),
              &RV32IInstrExecutor::executeADD};
        case (0b0000'0010'0000):
          return std::tuple{
              std::make_shared<rvdash::I_Instruction>(SRAI, Instr),
              &RV32IInstrExecutor::executeADD};
        default:
          return std::nullopt;
        }
      }
      case (0b010):
        return std::tuple{std::make_shared<rvdash::I_Instruction>(SLTI, Instr),
                          &RV32IInstrExecutor::executeADD};
      case (0b011):
        return std::tuple{std::make_shared<rvdash::I_Instruction>(SLTIU, Instr),
                          &RV32IInstrExecutor::executeADD};
      default:
        return std::nullopt;
      }
    }
    // Loads (I_Instruction)
    else if (Opcode == 0b000'0011) {
      switch (Funct3) {
      case 0b000:
        return std::tuple{std::make_shared<rvdash::I_Instruction>(LB, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b001:
        return std::tuple{std::make_shared<rvdash::I_Instruction>(LH, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b010:
        return std::tuple{std::make_shared<rvdash::I_Instruction>(LW, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b100:
        return std::tuple{std::make_shared<rvdash::I_Instruction>(LBU, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b101:
        return std::tuple{std::make_shared<rvdash::I_Instruction>(LHU, Instr),
                          &RV32IInstrExecutor::executeADD};
      default:
        return std::nullopt;
      }
    }
    // B_Instruction
    else if (Opcode == 0b110'0011) {
      switch (Funct3) {
      case 0b000:
        return std::tuple{std::make_shared<rvdash::B_Instruction>(BEQ, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b001:
        return std::tuple{std::make_shared<rvdash::B_Instruction>(BNE, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b100:
        return std::tuple{std::make_shared<rvdash::B_Instruction>(BLT, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b101:
        return std::tuple{std::make_shared<rvdash::B_Instruction>(BGE, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b110:
        return std::tuple{std::make_shared<rvdash::B_Instruction>(BLTU, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b111:
        return std::tuple{std::make_shared<rvdash::B_Instruction>(BGEU, Instr),
                          &RV32IInstrExecutor::executeADD};
      default:
        return std::nullopt;
      }
    }
    // S_Instruction
    else if (Opcode == 0b010'0011) {
      switch (Funct3) {
      case 0b000:
        return std::tuple{std::make_shared<rvdash::S_Instruction>(SB, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b001:
        return std::tuple{std::make_shared<rvdash::S_Instruction>(SH, Instr),
                          &RV32IInstrExecutor::executeADD};
      case 0b010:
        return std::tuple{std::make_shared<rvdash::S_Instruction>(SW, Instr),
                          &RV32IInstrExecutor::executeADD};
      default:
        return std::nullopt;
      }
    }
    // LUI (U_Instruction)
    else if (Opcode == 0b011'0111) {
      return std::tuple{std::make_shared<rvdash::U_Instruction>(LUI, Instr),
                        &RV32IInstrExecutor::executeADD};
    }
    // AUIPC (U_Instruction)
    else if (Opcode == 0b001'0111) {
      return std::tuple{std::make_shared<rvdash::U_Instruction>(AUIPC, Instr),
                        &RV32IInstrExecutor::executeADD};
    }
    // J_Instruction
    else if (Opcode == 0b110'1111) {
      return std::tuple{std::make_shared<rvdash::J_Instruction>(JAL, Instr),
                        &RV32IInstrExecutor::executeADD};
    }
    // JALR (I_Instruction)
    else if (Opcode == 0b110'0111) {
      switch (Funct3) {
      case (0b000):
        return std::tuple{std::make_shared<rvdash::I_Instruction>(JALR, Instr),
                          &RV32IInstrExecutor::executeADD};
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
          return std::tuple{std::make_shared<rvdash::I_Instruction>(ECALL,
    Instr), &RV32IInstrExecutor::executeADD}; case (0b1): return
    std::tuple{std::make_shared<rvdash::I_Instruction>(EBREAK, Instr),
    &RV32IInstrExecutor::executeADD}; default: return std::nullopt;
      }
    }*/

    return std::nullopt;
  }
};

//----------------------------------RV32IInstrSet----------------------------------------

class RV32IInstrSet {
public:
  static const size_t PcSz = 32;
  static const size_t RegSz = 32;

protected:
  RV32IInstrDecoder Decoder;
  RV32IInstrExecutor Executor;
  RegistersSet<RegSz> Registers;

public:
  using FunctTypes = RV32I::FunctTypes;

  RV32IInstrSet() : Registers(X0, X31 - X0) {
    Registers.addNamedRegister("pc");
    ++Registers.getNamedRegister("pc");
  }

  virtual ~RV32IInstrSet() {};

  Register<PcSz> *getPC() { return &Registers.getNamedRegister("pc"); }

  void dump(std::ostream &Stream) const { Stream << "RV32IInstrSet\n"; }

  void print() const { dump(std::cout); }

  std::optional<std::tuple<std::shared_ptr<Instruction>, FunctTypes>>
  tryDecode(Register<Instruction::Sz> Instr) const {
    return Decoder.tryDecode(Instr);
  }

  template <typename Variant>
  bool tryExecute(std::shared_ptr<Instruction> Instr, Variant Functs) const {
    if (Instr->getExtension() != Extensions::RV32I)
      return true;
    Executor.execute(Instr, std::get<FunctTypes>(Functs));
    return false;
  }
};

std::ostream &operator<<(std::ostream &Stream,
                         const typename RV32I::RV32IInstrSet &Set);

} // namespace RV32I
} // namespace rvdash

#endif // RV32I_INSTRUCTION_SET_H
