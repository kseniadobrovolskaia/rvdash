#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "rvdash/InstructionSet/Registers.h"




namespace rvdash {

enum class Extensions;

//------------------------------------Instruction----------------------------------------

enum class InstrEncodingType {
  R,
  I,
  S,
  B,
  U,
  J
};

class Instruction {
public:
  static const short Sz = 32;

protected:
  InstrEncodingType Type;
  Extensions Ex;
  
  std::bitset<Sz> Instr;
  std::bitset<7> Op;

public:
  Instruction() {};
  Instruction(InstrEncodingType T, Extensions Ext) : Type(T) {};
  Instruction(InstrEncodingType T, Extensions Ext, std::bitset<7> O) : Type(T), Ex(Ext), Op(O) {};
  Instruction(InstrEncodingType T, Extensions Ext, std::bitset<Sz> Ins) : Type(T), Ex(Ext), Instr(Ins) {};

  Extensions getExtension() const { return Ex; }

  virtual void print() const {
    std::cout << "Extension: " << magic_enum::enum_name(Ex)
              << ", Type: " << magic_enum::enum_name(Type) << "\n"; 
  }
  
  InstrEncodingType getType() const { return Type; }

  inline uint8_t getOpcode() const {
    return (Instr & std::bitset<Sz>(0x7f)).to_ulong();
  }
  
  virtual inline uint8_t getInstr() const {
    return Instr.to_ulong();
  }

  virtual void extractAllFields() {
    Op = getOpcode();
  }

  static inline uint8_t extractOpcode(std::bitset<Sz> Ins) {
    return (Ins & std::bitset<Sz>(0x7f)).to_ulong();
  }

  static inline uint8_t extractFunct3(std::bitset<Sz> Ins) {
    return ((Ins >> 12) & std::bitset<Sz>(0x7)).to_ulong();
  }

  static inline uint8_t extractFunct7(std::bitset<Sz> Ins) {
    return ((Ins >> 25) & std::bitset<Sz>(0x3f)).to_ulong();
  }

  static inline uint8_t extractRs2(std::bitset<Sz> Ins) {
    return ((Ins >> 20) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  static inline uint8_t extractRd(std::bitset<Sz> Ins) {
    return ((Ins >> 7) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  static inline uint8_t extractRs1(std::bitset<Sz> Ins) {
    return ((Ins >> 15) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  static inline int16_t extractImm_11_0(std::bitset<Sz> Ins) {
    auto Result = ((Ins >> 20) & std::bitset<Sz>(0xfff)) << 4;
    Result = Result >> 4;
    return Result.to_ulong();
  }
};


std::ostream &operator<<(std::ostream& Stream, const Instruction &Instr);

std::optional<std::shared_ptr<Instruction>> operator||(std::optional<std::shared_ptr<Instruction>> Lhs,
                                                           std::optional<std::shared_ptr<Instruction>> Rhs);

//----------------------------------R_Instruction----------------------------------------


class R_Instruction : public Instruction {
  std::bitset<5> Rd;
  std::bitset<3> Funct3;
  std::bitset<5> Rs1;
  std::bitset<5> Rs2;
  std::bitset<7> Funct7;

public:
  R_Instruction(std::bitset<Sz> Ins, Extensions Ext) 
              : Instruction(InstrEncodingType::R, Ext, Ins) {
    extractAllFields();
  };

  R_Instruction(std::bitset<7> O, std::bitset<3> F3, std::bitset<7> F7, Extensions Ext) 
              : Instruction(InstrEncodingType::R, Ext, O), Funct3(F3), Funct7(F7) {};

  R_Instruction(const R_Instruction &R, const std::bitset<Sz> &Ins) 
              : R_Instruction(R) {
    Instr = Ins;
    Rd = extractRd();
    Rs1 = extractRs1();
    Rs2 = extractRs2();
  };

  void print() const override {
    Instruction::print();
    std::cout << "Instr: " << Instr << "\n"
              << "Opcode: " << Op << ", Funct3: " << Funct3 << ", Funct7: " << Funct7 
              << "\n    Rd: " << Rd << ", Rs1: " << Rs1 << ", Rs2: " << Rs2 << "\n";
  }

  inline uint8_t extractFunct3() {
    return ((Instr >> 12) & std::bitset<Sz>(0x7)).to_ulong();
  }

  inline uint8_t extractFunct7() {
    return ((Instr >> 25) & std::bitset<Sz>(0x3f)).to_ulong();
  }

  inline uint8_t extractRs2() {
    return ((Instr >> 20) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  inline uint8_t extractRd() {
    return ((Instr >> 7) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  inline uint8_t extractRs1() {
    return ((Instr >> 15) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  void extractAllFields() override {
    Op = getOpcode();
    Rd = extractRd();
    Funct3 = extractFunct3();
    Rs1 = extractRs1();
    Rs2 = extractRs2();
    Funct7 = extractFunct7();
  };

};

//----------------------------------I_Instruction----------------------------------------


class I_Instruction : public Instruction {
  std::bitset<5> Rd;
  std::bitset<3> Funct3;
  std::bitset<5> Rs1;
  std::bitset<12> Imm_11_0;

public:
  I_Instruction(std::bitset<Sz> Ins, Extensions Ext) 
              : Instruction(InstrEncodingType::I, Ext, Ins) {
    extractAllFields();
  };

  I_Instruction(std::bitset<7> O, std::bitset<3> F3, Extensions Ext) 
              : Instruction(InstrEncodingType::I, Ext, O), Funct3(F3) {};

  I_Instruction(std::bitset<7> O, std::bitset<3> F3, std::bitset<12> Imm_110,
                Extensions Ext) 
              : Instruction(InstrEncodingType::I, Ext, O), Funct3(F3), Imm_11_0(Imm_110) {};

  I_Instruction(const I_Instruction &I, const std::bitset<Sz> &Ins) 
              : I_Instruction(I) {
    Instr = Ins;
    Rd = extractRd();
    Rs1 = extractRs1();
    Imm_11_0 = extractImm_11_0();
  };

  void print() const override {
    Instruction::print();
    std::cout << "Instr: " << Instr << "\n"
              << ", Opcode: " << Op << "\n"
              << "    Funct3: " << Funct3 << ", Rs1: " << Rs1
              << ", Imm_11_0: " << Imm_11_0 << "\n";
  }

  inline uint8_t extractFunct3() {
    return ((Instr >> 12) & std::bitset<Sz>(0x7)).to_ulong();
  }

  inline uint8_t extractRd() {
    return ((Instr >> 7) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  inline uint8_t extractRs1() {
    return ((Instr >> 15) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  inline int16_t extractImm_11_0() {
    auto Result = ((Instr >> 20) & std::bitset<Sz>(0xfff)) << 4;
    Result = Result >> 4;
    return Result.to_ulong();
  }

  void extractAllFields() override {
    Op = getOpcode();
    Rd = extractRd();
    Funct3 = extractFunct3();
    Rs1 = extractRs1();
    Imm_11_0 = extractImm_11_0();
  };

};

//----------------------------------S_Instruction----------------------------------------


class S_Instruction : public Instruction {
  std::bitset<5> Imm_4_0;
  std::bitset<3> Funct3;
  std::bitset<5> Rs1;
  std::bitset<12> Imm_11_5;

public:
  S_Instruction(std::bitset<Sz> Ins, Extensions Ext) 
              : Instruction(InstrEncodingType::S, Ext, Ins) {
    extractAllFields();
  };

  S_Instruction(std::bitset<7> O, std::bitset<3> F3, Extensions Ext) 
              : Instruction(InstrEncodingType::S, Ext, O), Funct3(F3) {};

  S_Instruction(const S_Instruction &S, const std::bitset<Sz> &Ins) 
              : S_Instruction(S) {
    Instr = Ins;
    Imm_4_0 = extractImm_4_0();
    Rs1 = extractRs1();
    Imm_11_5 = extractImm_11_5();
  };

  void print() const override {
    Instruction::print();
    std::cout << "Instr: " << Instr << "\n"
              << ", Opcode: " << Op << "\n" << "    Imm_4_0: " << Imm_4_0
              << "Funct3: " << Funct3 << ", Rs1: " << Rs1 << "\n"
              << "    Imm_11_5: " << Imm_11_5 << "\n";
  }
 
  inline uint8_t extractFunct3() {
    return ((Instr >> 12) & std::bitset<Sz>(0x7)).to_ulong();
  }

  inline uint8_t extractRs2() {
    return ((Instr >> 20) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  inline uint8_t extractRs1() {
    return ((Instr >> 15) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  inline int16_t extractImm_11_5() {
    return ((Instr >> 25) & std::bitset<Sz>(0x3f)).to_ulong();
  }

  inline uint8_t extractImm_4_0() {
    return ((Instr >> 7) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  void extractAllFields() override {
    Op = getOpcode();
    Imm_4_0 = extractImm_4_0();
    Funct3 = extractFunct3();
    Rs1 = extractRs1();
    Imm_11_5 = extractImm_11_5();
  };

};

//----------------------------------B_Instruction----------------------------------------


class B_Instruction : public Instruction {
  std::bitset<1> Imm_11;
  std::bitset<4> Imm_4_1;
  std::bitset<3> Funct3;
  std::bitset<5> Rs1;
  std::bitset<5> Rs2;
  std::bitset<6> Imm_10_5;
  std::bitset<1> Imm_12;

public:
  B_Instruction(std::bitset<Sz> Ins, Extensions Ext) 
              : Instruction(InstrEncodingType::B, Ext, Ins) {
    extractAllFields();
  };

  B_Instruction(std::bitset<7> O, std::bitset<3> F3, Extensions Ext) 
              : Instruction(InstrEncodingType::B, Ext, O), Funct3(F3) {};

  B_Instruction(const B_Instruction &B, const std::bitset<Sz> &Ins) 
              : B_Instruction(B) {
    Instr = Ins;
    Imm_11 = extractImm_11();
    Imm_4_1 = extractImm_4_1();
    Rs1 = extractRs1();
    Rs2 = extractRs2();
    Imm_10_5 = extractImm_10_5();
    Imm_12 = extractImm_12();
  };

  void print() const override {
    Instruction::print();
    std::cout << "Instr: " << Instr << "\n"
              << ", Opcode: " << Op << "\n" << "    Imm_11: " << Imm_11 
              << ", Imm_4_1:" << Imm_4_1 << "    Funct3: " << Funct3 
              << ", Rs1: " << Rs1 << ", Rs2: " << Rs2 << "\n"
              << "    Imm_10_5:" << Imm_10_5 << ", Imm_12: " << Imm_12 << "\n";
  }

  inline uint8_t extractFunct3() {
    return ((Instr >> 12) & std::bitset<Sz>(0x7)).to_ulong();
  }

  inline uint8_t extractRs2() {
    return ((Instr >> 20) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  inline uint8_t extractRs1() {
    return ((Instr >> 15) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  inline uint8_t extractImm_10_5() {
    return ((Instr >> 25) & std::bitset<Sz>(0x3f)).to_ulong();
  }

  inline uint8_t extractImm_12() {
    return ((Instr >> 31) & std::bitset<Sz>(0x1)).to_ulong();
  }

  inline uint8_t extractImm_11() {
    return ((Instr >> 7) & std::bitset<Sz>(0x1)).to_ulong();
  }

  inline uint8_t extractImm_4_1() {
    return ((Instr >> 8) & std::bitset<Sz>(0xf)).to_ulong();
  }

  void extractAllFields() override {
    Op = getOpcode();
    Imm_11 = extractImm_11();
    Imm_4_1 = extractImm_4_1();
    Funct3 = extractFunct3();
    Rs1 = extractRs1();
    Rs2 = extractRs2();
    Imm_10_5 = extractImm_10_5();
    Imm_12 = extractImm_12();
  };

  inline int16_t extractImm() {
	  std::bitset<Sz> Result;
    Result = (Instr & std::bitset<Sz>(0x80000000)) >> 19;
    Result |= (Instr & std::bitset<Sz>(0x80)) << 4;
    Result |= (Instr & std::bitset<Sz>(0x7e000000)) >> 20;
    Result |= (Instr & std::bitset<Sz>(0xf00)) >> 7;
    Result = Result << 3;
    Result = Result >> 3;
    return Result.to_ulong();
  }
};

//----------------------------------U_Instruction----------------------------------------


class U_Instruction : public Instruction {
  std::bitset<5> Rd;
  std::bitset<20> Imm_31_12;

public:
  U_Instruction(std::bitset<Sz> Ins, Extensions Ext) 
              : Instruction(InstrEncodingType::U, Ext) {
    if (((Ins >> 7) & std::bitset<Sz>(0x1ffffff)).any()) {
      Op = Ins.to_ulong();
      return;
    }
    Instr = Ins;
    extractAllFields();
  };

  U_Instruction(const U_Instruction &U, const std::bitset<Sz> &Ins) 
              : U_Instruction(U) {
    Instr = Ins;
    Rd = extractRd();
    Imm_31_12 = extractImm_31_12();
  };

  void print() const override {
    Instruction::print();
    std::cout << "Instr: " << Instr << "\n"
              << ", Opcode: " << Op << "\n" << "    Rd: " << Rd
              << ", Imm_31_12: " << Imm_31_12 << "\n";
  }

  inline int32_t extractImm_31_12() {
    return ((Instr >> 12) & std::bitset<Sz>(0xfffff)).to_ulong();
  }

  inline uint8_t extractRd() {
    return ((Instr >> 7) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  void extractAllFields() override {
    Op = getOpcode();
    Rd = extractRd();
    Imm_31_12 = extractImm_31_12();
  };

};

//----------------------------------J_Instruction----------------------------------------


class J_Instruction : public Instruction {
  std::bitset<5> Rd;
  std::bitset<8> Imm_19_12;
  std::bitset<1> Imm_11;
  std::bitset<10> Imm_10_1;
  std::bitset<1> Imm_20;

public:
  J_Instruction(std::bitset<Sz> Ins, Extensions Ext) 
              : Instruction(InstrEncodingType::J, Ext) {
    if (((Ins >> 7) & std::bitset<Sz>(0x1ffffff)).any()) {
      Op = Ins.to_ulong();
      return;
    }
    Instr = Ins;
    extractAllFields();
  };

  J_Instruction(const J_Instruction &J, const std::bitset<Sz> &Ins) 
              : J_Instruction(J) {
    Instr = Ins;
    extractAllFields();
  };

  void print() const override {
    Instruction::print();
    std::cout << "Instr: " << Instr << "\n"
              << ", Opcode: " << Op << "\n" << "    Rd: " << Rd
              << ", Imm_19_12: " << Imm_19_12 << ", Imm_11: " << Imm_11 << "\n"
              << "    Imm_10_1: " << Imm_10_1 << " Imm_20: " << Imm_20 << "\n";
  }

  inline uint8_t extractImm_19_12() {
    return ((Instr >> 12) & std::bitset<Sz>(0xff)).to_ulong();
  }

  inline uint8_t extractRd() {
    return ((Instr >> 7) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  inline uint8_t extractImm_11() {
    return ((Instr >> 20) & std::bitset<Sz>(0x1)).to_ulong();
  }

  inline uint8_t extractImm_10_1() {
    return ((Instr >> 21) & std::bitset<Sz>(0x3ff)).to_ulong();
  }

  inline uint8_t extractImm_20() {
    return ((Instr >> 31) & std::bitset<Sz>(0x1)).to_ulong();
  }

  void extractAllFields() override {
    Op = getOpcode();
    Rd = extractRd();
    Imm_19_12 = extractImm_19_12();
    Imm_11 = extractImm_11();
    Imm_10_1 = extractImm_10_1();
    Imm_20 = extractImm_20();
  };

};

} // namespace rvdash

#endif // INSTRUCTION_H

