#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <algorithm>
#include <typeinfo>
#include <cstdlib>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <vector>
#include <signal.h>
#include <map>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include <bitset>
#include <optional>
#include <cstring>

#include "Error.h"
#include "magic_enum.hpp"




namespace rvdash {

//-----------------------------------RegistersSet----------------------------------------

template <size_t Sz>
class RegistersSet {
public:
  using Register = std::bitset<Sz>;

  RegistersSet(unsigned Count) : Registers(Count, 0) {};
  
  void addNamedRegister(const std::string &Name) {
    NamedRegisters[Name] = 0;
  }

  Register &getNamedRegister(const std::string &Name) {
    return NamedRegisters[Name];
  }
  Register getNamedRegister(const std::string &Name) const {
    return NamedRegisters[Name];
  }
  
  Register operator[](unsigned i) const { 
    assert(i < Registers.size());
    return Registers[i]; 
  }

private:
  std::vector<Register> Registers;
  std::unordered_map<std::string, Register> NamedRegisters;

};

//------------------------------------Instruction----------------------------------------

enum class InstrEncodingType {
  R,
  I,
  S,
  B,
  U,
  J
};


using Operand = unsigned short;


template <size_t Sz>
class Instruction {
public:
  InstrEncodingType Type;
  std::bitset<7 * Sz / 32> Opcode;
  std::bitset<Sz> Instr;

public:
  Instruction() {};
  Instruction(InstrEncodingType T, std::bitset<Sz> Ins) : Type(T), Instr(Ins) {};
  
  virtual void print() const {
    std::cout << "Type: " << magic_enum::enum_name(Type) << ", Opcode: " << Opcode
              << " Instr: " << Instr << "\n";
  }

  virtual inline uint8_t getOpcode() {
    return (Instr & std::bitset<Sz>(0x7f)).to_ulong();
  }

};

//----------------------------------R_Instruction----------------------------------------

template <size_t Sz>
class R_Instruction : public Instruction<Sz> {
  std::bitset<Sz> Instr;
  std::bitset<7 * Sz / 32> Op;
  std::bitset<5 * Sz / 32> Rd;
  std::bitset<3 * Sz / 32> Funct3;
  std::bitset<5 * Sz / 32> Rs1;
  std::bitset<5 * Sz / 32> Rs2;
  std::bitset<7 * Sz / 32> Funct7;

public:
  R_Instruction(std::bitset<Sz> Ins) 
              : Instruction<Sz>(InstrEncodingType::R, Ins), Instr(Ins) {};

  void print() const override {
    std::cout << "Instr: " << Instr << "\n"
              << "      Type: " << magic_enum::enum_name(this->Type) << ", Opcode: " << Op << "\n"
              << "    Funct3: " << Funct3 << ", Funct7: " << Funct7 << ", Rd: " << Rd
              << ", Rs1: " << Rs1 << ", Rs2: " << Rs2 << "\n";
  }

  R_Instruction(std::bitset<7 * Sz / 32> O, std::bitset<3 * Sz / 32> F3, std::bitset<7 * Sz / 32> F7) 
              : Instruction<Sz>(), Op(O), Funct3(F3), Funct7(F7) {};
  R_Instruction(const R_Instruction &R, const std::bitset<Sz> &Ins) 
              : R_Instruction(R) {
    Instr = Ins;
    Rd = extractRd();
    Rs1 = extractRs1();
    Rs2 = extractRs2();
  };

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
};

//----------------------------------I_Instruction----------------------------------------

template <size_t Sz>
class I_Instruction : public Instruction<Sz> {
  std::bitset<Sz> Instr;
  std::bitset<7 * Sz / 32> Op;
  std::bitset<5 * Sz / 32> Rd;
  std::bitset<3 * Sz / 32> Funct3;
  std::bitset<5 * Sz / 32> Rs1;
  std::bitset<12 * Sz / 32> Imm_11_0;

public:
  I_Instruction(std::bitset<Sz> Ins) 
              : Instruction<Sz>(InstrEncodingType::I, Ins), Instr(Ins) {};

  I_Instruction(std::bitset<7 * Sz / 32> O, std::bitset<3 * Sz / 32> F3) 
              : Instruction<Sz>(), Op(O), Funct3(F3) {};
  I_Instruction(const I_Instruction &I, const std::bitset<Sz> &Ins) 
              : I_Instruction(I) {
    Instr = Ins;
    Rd = extractRd();
    Rs1 = extractRs1();
    Imm_11_0 = extractImm_11_0();
  };

  inline uint8_t extractFunct3() {
    return (Instr >> 12) & 0x7;
  }

  inline int16_t extractImm_11_0() {
    int16_t Result = ((Instr >> 20) & 0xfff) << 4;
    Result = Result >> 4;
    return Result;
  }

  inline uint8_t extractRd() {
    return (Instr >> 7) & 0x1f;
  }

  inline uint8_t extractRs1() {
    return (Instr >> 15) & 0x1f;
  }
};

//----------------------------------S_Instruction----------------------------------------

template <size_t Sz>
class S_Instruction : public Instruction<Sz> {
  std::bitset<Sz> Instr;
  std::bitset<7 * Sz / 32> Op;
  std::bitset<5 * Sz / 32> Imm_4_0;
  std::bitset<3 * Sz / 32> Funct3;
  std::bitset<5 * Sz / 32> Rs1;
  std::bitset<12 * Sz / 32> Imm_11_5;

public:
  S_Instruction(std::bitset<Sz> Ins) 
              : Instruction<Sz>(InstrEncodingType::S, Ins), Instr(Ins) {};

  S_Instruction(std::bitset<7 * Sz / 32> O, std::bitset<3 * Sz / 32> F3) 
              : Instruction<Sz>(), Op(O), Funct3(F3) {};

  S_Instruction(const S_Instruction &S, const std::bitset<Sz> &Ins) 
              : S_Instruction(S) {
    Instr = Ins;
    Imm_4_0 = extractImm_4_0();
    Rs1 = extractRs1();
    Imm_11_5 = extractImm_11_5();
  };

  inline uint8_t extractFunct3() {
    return (Instr >> 12) & 0x7;
  }

  inline int16_t extractImm_11_5() {
    return (Instr >> 25) & 0x3f;
  }

  inline uint8_t extractRs2() {
    return (Instr >> 20) & 0x1f;
  }

  inline uint8_t extractImm_4_0() {
    return (Instr >> 7) & 0x1f;
  }

  inline uint8_t extractRs1() {
    return (Instr >> 15) & 0x1f;
  }
};

//----------------------------------B_Instruction----------------------------------------

template <size_t Sz>
class B_Instruction : public Instruction<Sz> {
  std::bitset<Sz> Instr;
  std::bitset<7 * Sz / 32> Op;
  std::bitset<1 * Sz / 32> Imm_11;
  std::bitset<4 * Sz / 32> Imm_4_1;
  std::bitset<3 * Sz / 32> Funct3;
  std::bitset<5 * Sz / 32> Rs1;
  std::bitset<5 * Sz / 32> Rs2;
  std::bitset<6 * Sz / 32> Imm_10_5;
  std::bitset<1 * Sz / 32> Imm_12;

public:
  B_Instruction(std::bitset<Sz> Ins) 
              : Instruction<Sz>(InstrEncodingType::B, Ins), Instr(Ins) {};

  B_Instruction(std::bitset<7 * Sz / 32> O, std::bitset<3 * Sz / 32> F3) 
              : Instruction<Sz>(), Op(O), Funct3(F3) {};

  B_Instruction(const B_Instruction &B, const std::bitset<Sz> &Ins) 
              : B_Instruction(B) {
    Instr = Ins;
    Imm_11 = extractImm_11();
    Rs1 = extractRs1();
    Rs2 = extractRs2();
    Imm_10_5 = extractImm_10_5();
    Imm_12 = extractImm_12();
  };

  inline uint8_t extractImm_10_5() {
    return (Instr >> 25) & 0x3f;
  }

  inline uint8_t extractImm_12() {
    return (Instr >> 31) & 0x1;
  }

  inline uint8_t extractRs1() {
    return (Instr >> 15) & 0x1f;
  }

  inline uint8_t extractImm_11() {
    return (Instr >> 7) & 0x1;
  }

  inline uint8_t extractImm_4_1() {
    return (Instr >> 8) & 0xf;
  }

  inline uint8_t extractRs2() {
    return (Instr >> 20) & 0x1f;
  }

  inline uint8_t extractFunct3() {
    return (Instr >> 12) & 0x7;
  }

  inline int16_t extractImm() {
    int16_t Result;
    Result = (Instr & 0x80000000) >> 19;
    Result |= (Instr & 0x80) << 4;
    Result |= (Instr & 0x7e000000) >> 20;
    Result |= (Instr & 0xf00) >> 7;
    Result = Result << 3;
    Result = Result >> 3;
    return Result;
  }
};

//----------------------------------U_Instruction----------------------------------------

template <size_t Sz>
class U_Instruction : public Instruction<Sz> {
  std::bitset<Sz> Instr;
  std::bitset<7 * Sz / 32> Op;
  std::bitset<5 * Sz / 32> Rd;
  std::bitset<20 * Sz / 32> Imm_31_12;

public:
  U_Instruction(std::bitset<Sz> Ins) 
              : Instruction<Sz>(InstrEncodingType::U, Ins) {
    if (((Ins >> 7) & std::bitset<Sz>(0x1ffffff)).any())
      Op = Ins.to_ulong();
    else
      Instr = Ins;
  };

  U_Instruction(const U_Instruction &U, const std::bitset<Sz> &Ins) 
              : U_Instruction(U) {
    Instr = Ins;
    Rd = extractRd();
    Imm_31_12 = extractImm_31_12();
  };

  inline int32_t extractImm_31_12() {
    return (Instr >> 12) & 0xfffff;
  }

  inline uint8_t extractRd() {
    return (Instr >> 7) & 0x1f;
  }
};

//----------------------------------J_Instruction----------------------------------------

template <size_t Sz>
class J_Instruction : public Instruction<Sz> {
  std::bitset<Sz> Instr;
  std::bitset<7 * Sz / 32> Op;
  std::bitset<5 * Sz / 32> Rd;
  std::bitset<8 * Sz / 32> Imm_19_12;
  std::bitset<1 * Sz / 32> Imm_11;
  std::bitset<10 * Sz / 32> Imm_10_1;
  std::bitset<1 * Sz / 32> Imm_20;

public:
  J_Instruction(std::bitset<Sz> Ins) 
              : Instruction<Sz>(InstrEncodingType::J, Ins) {
    if (((Ins >> 7) & std::bitset<Sz>(0x1ffffff)).any())
      Op = Ins.to_ulong();
    else
      Instr = Ins;
  };

  J_Instruction(const J_Instruction &J, const std::bitset<Sz> &Ins) 
              : J_Instruction(J) {
    Instr = Ins;
    Rd = extractRd();
    Imm_19_12 = extractImm_19_12();
    Imm_11 = extractImm_11();
    Imm_10_1 = extractImm_10_1();
    Imm_20 = extractImm_20();
  };

  inline uint8_t extractImm_19_12() {
    return (Instr >> 12) & 0xff;
  }

  inline uint8_t extractRd() {
    return (Instr >> 7) & 0x1f;
  }

  inline uint8_t extractImm_11() {
    return (Instr >> 20) & 0x1;
  }

  inline uint8_t extractImm_10_1() {
    return (Instr >> 21) & 0x3ff;
  }

  inline uint8_t extractImm_20() {
    return (Instr >> 31) & 0x1;
  }
};

//----------------------------------InstrDecoder-----------------------------------------

const size_t AddrSpaceSz = 32;

class InstrDecoder {

public:
  using Register = std::bitset<AddrSpaceSz>;
  InstrDecoder() {};
  virtual std::optional<std::shared_ptr<Instruction<AddrSpaceSz>>> tryDecode(Register Instr) const = 0;
};

//----------------------------------InstrExecutor----------------------------------------

class InstrExecutor {

public:
  using Register = std::bitset<AddrSpaceSz>;
  InstrExecutor() {};
  virtual void execute(const Instruction<AddrSpaceSz> &Instr) const = 0;
};

//------------------------------------InstrSet-------------------------------------------

class InstrSet {
protected:
  std::shared_ptr<InstrDecoder> Decoder;
  std::shared_ptr<InstrExecutor> Executor;
  std::optional<std::shared_ptr<RegistersSet<AddrSpaceSz>>> Registers;

public:
  using Register = std::bitset<AddrSpaceSz>;
  InstrSet() {};
  virtual ~InstrSet() = 0;
};

} // namespace rvdash

#endif // INSTRUCTION_SET_H

