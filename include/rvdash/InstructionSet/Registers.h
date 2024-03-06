#ifndef REGISTER_H
#define REGISTER_H

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
#include <initializer_list>
#include <bitset>
#include <optional>
#include <cstring>

#include "Error.h"
#include "magic_enum.hpp"




namespace rvdash {

//--------------------------------------Registers----------------------------------------

enum {
  NoRegister,
  X0,
  X1,
  X2,
  X3,
  X4,
  X5,
  X6,
  X7,
  X8,
  X9,
  X10,
  X11,
  X12,
  X13,
  X14,
  X15,
  X16,
  X17,
  X18,
  X19,
  X20,
  X21,
  X22,
  X23,
  X24,
  X25,
  X26,
  X27,
  X28,
  X29,
  X30,
  X31,
  F0_F,
  F1_F,
  F2_F,
  F3_F,
  F4_F,
  F5_F,
  F6_F,
  F7_F,
  F8_F,
  F9_F,
  F10_F,
  F11_F,
  F12_F,
  F13_F,
  F14_F,
  F15_F,
  F16_F,
  F17_F,
  F18_F,
  F19_F,
  F20_F,
  F21_F,
  F22_F,
  F23_F,
  F24_F,
  F25_F,
  F26_F,
  F27_F,
  F28_F,
  F29_F,
  F30_F,
  F31_F,
  F0_D,
  F1_D,
  F2_D,
  F3_D,
  F4_D,
  F5_D,
  F6_D,
  F7_D,
  F8_D,
  F9_D,
  F10_D,
  F11_D,
  F12_D,
  F13_D,
  F14_D,
  F15_D,
  F16_D,
  F17_D,
  F18_D,
  F19_D,
  F20_D,
  F21_D,
  F22_D,
  F23_D,
  F24_D,
  F25_D,
  F26_D,
  F27_D,
  F28_D,
  F29_D,
  F30_D,
  F31_D,
};

//-----------------------------------RegistersSet----------------------------------------
template <size_t Sz>
using Register = std::bitset<Sz>;
using RegName = unsigned;

template <size_t Sz>
Register<Sz> operator||(Register<Sz> Lhs, Register<Sz> Rhs) {
  return Lhs.has_value() ? Lhs : Rhs;
}

template <size_t Sz>
class RegistersSet {

public:
  RegistersSet(unsigned Count) : OwnRegs(Count, NoRegister) {};

  RegistersSet(RegName FirstReg, unsigned Size) : OwnRegs(Size) {
    Offset = FirstReg;
  };

  Register<Sz> &getRegister(RegName Reg) {
    assert(Reg - Offset < OwnRegs.size());
    return OwnRegs[Reg - Offset];
  }
 
  Register<Sz> getRegister(RegName Reg) const {
    assert(Reg - Offset < OwnRegs.size());
    return OwnRegs[Reg - Offset];
  }
 
  void addNamedRegister(const std::string &Name) {
    NamedRegisters[Name] = 0;
  }

  Register<Sz> &getNamedRegister(const std::string &Name) {
    return NamedRegisters[Name];
  }

  const Register<Sz> &getNamedRegister(const std::string &Name) const {
    return NamedRegisters.at(Name);
  }

private:
  unsigned Offset;
  //std::vector<std::shared_ptr<RegistersSet>> ExternalRegs;
  std::vector<RegName> OwnRegs;
  std::unordered_map<std::string, Register<Sz>> NamedRegisters;

};

template <size_t Sz>
std::bitset<Sz> operator++(Register<Sz>& Reg) {
  Reg = Reg.to_ulong() + Sz;
  return Reg;
}

} // namespace rvdash

#endif // REGISTER_H

