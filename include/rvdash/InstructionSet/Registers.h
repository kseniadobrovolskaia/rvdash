#ifndef REGISTER_H
#define REGISTER_H

#include <algorithm>
#include <bitset>
#include <cstring>
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
  RegistersSet(const char *Name, unsigned Count, std::ostream &File = std::cout)
      : SetName(Name), OwnRegs(Count), LogFile(File) {
    OwnRegs[0] = 0;
  };

  void setRegister(RegName Reg, Register<Sz> NewValue) {
    logChange(Reg, NewValue);
    if (strcmp(SetName, "X") == 0 && Reg == 0)
      return;
    OwnRegs.at(Reg) = NewValue;
  }

  //Register<Sz> &operator[](unsigned RegIdx) { return OwnRegs.at(RegIdx); }

  Register<Sz> operator[](unsigned RegIdx) const { return OwnRegs.at(RegIdx); }
  
  Register<Sz> getRegister(unsigned RegIdx) const { return OwnRegs.at(RegIdx); }

  void addNamedRegister(const std::string &Name) {
    NamedRegisters[Name] = 0;
  }

  void setNamedRegister(const std::string &Name, Register<Sz> NewValue) {
    logChange(Name, NewValue);
    NamedRegisters.at(Name) = NewValue;
  }

  const Register<Sz> &getNamedRegister(const std::string &Name) const {
    return NamedRegisters.at(Name);
  }

  Register<Sz> &getNamedRegister(const std::string &Name) {
    return NamedRegisters.at(Name);
  }

  void logChange(RegName Reg, Register<Sz> NewValue) const {
    LogFile << SetName << std::dec << Reg << " <- 0x" << std::hex
            << NewValue.to_ulong() << "\n";
    LogFile << std::dec;
  }

  void logChange(std::string RegName, Register<Sz> NewValue) const {
    LogFile << RegName << " <- 0x" << std::hex << NewValue.to_ulong() << "\n";
    LogFile << std::dec;
  }

  void dump(std::ostream &Stream) const {
    Stream << "Registers Set:\n";
    if (NamedRegisters.size() != 0) {
      for (const auto &[Name, Reg] : NamedRegisters)
        Stream << Name << " = 0x" << std::hex << Reg.to_ulong() << "\n";
    }
    Stream << "\nRegisters: \n";
    for (auto Idx = 0; Idx < OwnRegs.size(); ++Idx)
      Stream << SetName << std::dec << Idx << " = 0x" << std::hex
             << OwnRegs[Idx].to_ulong() << "\n";
    Stream << std::dec;
  }

  void dump() const { dump(LogFile); }

  void print() const { dump(std::cout); }

  /*void addExternalRegSet(std::shared_ptr<RegistersSet> Set) {
    ExternalRegs.push_back(Set);
  }*/

private:
  //std::vector<std::shared_ptr<RegistersSet>> ExternalRegs;
  const char *SetName;
  std::vector<Register<Sz>> OwnRegs;
  std::unordered_map<std::string, Register<Sz>> NamedRegisters;

  std::ostream &LogFile;
};

template <size_t Sz>
std::bitset<Sz> operator++(Register<Sz>& Reg) {
  Reg = Reg.to_ulong() + Sz;
  return Reg;
}

} // namespace rvdash

#endif // REGISTER_H

