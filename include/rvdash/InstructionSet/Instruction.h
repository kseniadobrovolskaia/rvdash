#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "rvdash/InstructionSet/Registers.h"




namespace rvdash {

enum class Extensions;

//------------------------------------Instruction----------------------------------------
// clang-format off
//
//                RISC_V base instruction formats (InstrEncodingType)
//
//  31   30       25 24  21   20 19      15 14    12 11      8 7      6        0
//   ___________________________________________________________________________
//  |               |           |          |        |                |          |
//  |    Funct7     |    Rs2    |    Rs1   | Funct3 |       Rd       |  Opcode  | R-type
//  |_______________|___________|__________|________|________________|__________|
//   ___________________________________________________________________________
//  |                           |          |        |                |          |
//  |         Imm_11_0          |    Rs1   | Funct3 |       Rd       |  Opcode  | I-type
//  |___________________________|__________|________|________________|__________|
//   ___________________________________________________________________________
//  |               |           |          |        |                |          |
//  |    Imm_11_5   |    Rs2    |    Rs1   | Funct3 |    Imm_4_0     |  Opcode  | S-type
//  |_______________|___________|__________|________|________________|__________|
//   ___________________________________________________________________________
//  |      |        |           |          |        |         |      |          |
//  |Imm_12|Imm_10_5|    Rs2    |    Rs1   | Funct3 | Imm_4_1 |Imm_11|  Opcode  | B-type
//  |______|________|___________|__________|________|_________|______|__________|
//   ___________________________________________________________________________
//  |                                               |                |          |
//  |                   Imm_31_12                   |       Rd       |  Opcode  | U-type
//  |_______________________________________________|________________|__________|
//   ___________________________________________________________________________
//  |      |            |       |                   |                |          |
//  |Imm_20|  Imm_10_1  |Imm_11 |     Imm_19_12     |       Rd       |  Opcode  | J-type
//  |______|____________|_______|___________________|________________|__________|
//
// clang-format on

enum class InstrEncodingType {
  R,
  I,
  S,
  B,
  U,
  J
};

struct Instruction {
  static const short Sz = 32;
  static const short Sz_b = 4;

  std::bitset<Sz> Bits;

  InstrEncodingType Type;
  Extensions Ex;

  Instruction() {};
  Instruction(std::bitset<Sz> Ins, InstrEncodingType T, Extensions Ext)
      : Bits(Ins), Type(T), Ex(Ext){};

  void dump(std::ostream &Stream) const {
    Stream << "Extension: " << magic_enum::enum_name(Ex)
           << ", Type: " << magic_enum::enum_name(Type) << "\n"
           << "Bits: " << Bits << "\n";
  }

  void print() const { dump(std::cout); }

  uint8_t extractInstr() const { return Bits.to_ulong(); }

  uint8_t extractOpcode() const {
    return (Bits & std::bitset<Sz>(0x7f)).to_ulong();
  }

  uint8_t extractFunct3() const {
    return ((Bits >> 12) & std::bitset<Sz>(0x7)).to_ulong();
  }

  uint8_t extractFunct7() const {
    return ((Bits >> 25) & std::bitset<Sz>(0x3f)).to_ulong();
  }

  uint8_t extractRd() const {
    return ((Bits >> 7) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  uint8_t extractRs1() const {
    return ((Bits >> 15) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  uint8_t extractRs2() const {
    return ((Bits >> 20) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  uint8_t extractImm_4_0() const {
    return ((Bits >> 7) & std::bitset<Sz>(0x1f)).to_ulong();
  }

  uint16_t extractImm_11_0() const {
    auto Result = ((Bits >> 20) & std::bitset<Sz>(0xfff)) << 4;
    Result = Result >> 4;
    return Result.to_ulong();
  }

  uint8_t extractImm_4_1() const {
    return ((Bits >> 8) & std::bitset<Sz>(0xf)).to_ulong();
  }

  uint16_t extractImm_10_1() const {
    return ((Bits >> 21) & std::bitset<Sz>(0x3ff)).to_ulong();
  }

  uint8_t extractImm_10_5() const {
    return ((Bits >> 25) & std::bitset<Sz>(0x3f)).to_ulong();
  }

  uint16_t extractImm_11_5() const {
    return ((Bits >> 25) & std::bitset<Sz>(0x3f)).to_ulong();
  }

  uint8_t extractImm_11() const {
    return ((Bits >> 20) & std::bitset<Sz>(0x1)).to_ulong();
  }

  uint8_t extractImm_12() const {
    return ((Bits >> 31) & std::bitset<Sz>(0x1)).to_ulong();
  }

  uint8_t extractImm_19_12() const {
    return ((Bits >> 12) & std::bitset<Sz>(0xff)).to_ulong();
  }

  uint32_t extractImm_31_12() const {
    return ((Bits >> 12) & std::bitset<Sz>(0xfffff)).to_ulong();
  }

  uint8_t extractImm_20() const {
    return ((Bits >> 31) & std::bitset<Sz>(0x1)).to_ulong();
  }

  uint32_t extractImm_B() const {
    std::bitset<4> Imm_4_1 = extractImm_4_1();
    std::bitset<6> Imm_10_5 = extractImm_10_5();
    std::bitset<1> Imm_11 = extractImm_11();
    std::bitset<1> Imm_12 = extractImm_12();
    std::bitset<12> Imm;
    for (auto Idx = 0; Idx < 4; ++Idx)
      Imm[Idx] = Imm_4_1[Idx];
    for (auto Idx = 0; Idx < 6; ++Idx)
      Imm[Idx + 4] = Imm_10_5[Idx];
    Imm[10] = Imm_11[0];
    Imm[11] = Imm_12[0];
    return Imm.to_ulong();
  }

  uint32_t extractImm_J() const {
    std::bitset<10> Imm_10_1 = extractImm_10_1();
    std::bitset<1> Imm_11 = extractImm_11();
    std::bitset<8> Imm_19_12 = extractImm_19_12();
    std::bitset<1> Imm_20 = extractImm_20();
    std::bitset<20> Imm;
    for (auto Idx = 0; Idx < 10; ++Idx)
      Imm[Idx] = Imm_10_1[Idx];
    Imm[10] = Imm_11[0];
    for (auto Idx = 0; Idx < 8; ++Idx)
      Imm[Idx + 11] = Imm_19_12[Idx];
    Imm[19] = Imm_20[0];
    return Imm.to_ulong();
  }
};

bool isSame(Register<Instruction::Sz> Lhs, Register<Instruction::Sz> Rhs,
            std::bitset<Instruction::Sz> Mask);

} // namespace rvdash
#endif // INSTRUCTION_H

