#ifndef REGISTER_H
#define REGISTER_H

#include <algorithm>
#include <bitset>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "Error.h"
#include "magic_enum.hpp"




namespace rvdash {

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
  RegistersSet() {};
  RegistersSet(const char *Name, unsigned Count) : SetName(Name), OwnRegs(Count) {};

  void setRegister(RegName Reg, Register<Sz> NewValue) {
    logChange(Reg, NewValue);
    OwnRegs.at(Reg) = NewValue;
  }

  //Register<Sz> &operator[](unsigned RegIdx) { return OwnRegs.at(RegIdx); }

  Register<Sz> operator[](unsigned RegIdx) const { return OwnRegs.at(RegIdx); }
  
  Register<Sz> getRegister(unsigned RegIdx) const { return OwnRegs.at(RegIdx); }

  void addNamedRegister(const std::string &Name) {
    NamedRegisters[Name] = 0;
  }

  Register<Sz> &getNamedRegister(const std::string &Name) {
    return NamedRegisters.at(Name);
  }

  const Register<Sz> &getNamedRegister(const std::string &Name) const {
    return NamedRegisters.at(Name);
  }

  void logChange(RegName Reg, Register<Sz> NewValue) const {
    std::cout << SetName << Reg << " <- " << NewValue << "\n";
  }

  /*void addExternalRegSet(std::shared_ptr<RegistersSet> Set) {
    ExternalRegs.push_back(Set);
  }*/

private:
  //std::vector<std::shared_ptr<RegistersSet>> ExternalRegs;
  const char *SetName;
  std::vector<Register<Sz>> OwnRegs;
  std::unordered_map<std::string, Register<Sz>> NamedRegisters;

};

template <size_t Sz>
std::bitset<Sz> operator++(Register<Sz>& Reg) {
  Reg = Reg.to_ulong() + Sz;
  return Reg;
}

} // namespace rvdash

#endif // REGISTER_H

