#ifndef REGISTER_H
#define REGISTER_H

#include <algorithm>
#include <assert.h>
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

/**
 * @brief class RegistersSet - class to represent a register file belonging to a
 *                             specific extension. Contains numbered and named
 *                             registers.
 */
template <size_t Sz>
using Register = std::bitset<Sz>;
using RegName = unsigned;

template <size_t Sz>
Register<Sz> operator||(Register<Sz> Lhs, Register<Sz> Rhs) {
  return Lhs.has_value() ? Lhs : Rhs;
}

template <size_t Sz>
class RegistersSet {

  void logChange(RegName Reg, Register<Sz> NewValue,
                 std::ostream &LogFile) const {
    LogFile << SetName << std::dec << Reg << " <- 0x" << std::hex
            << NewValue.to_ulong() << "\n";
    LogFile << std::dec;
  }

  void logChange(std::string RegName, Register<Sz> NewValue,
                 std::ostream &LogFile) const {
    LogFile << RegName << " <- 0x" << std::hex << NewValue.to_ulong() << "\n";
    LogFile << std::dec;
  }

public:
  RegistersSet() {};
  RegistersSet(const char *Name, unsigned Count)
      : SetName(Name), OwnRegs(Count) {
    OwnRegs[0] = 0;
  };

  Register<Sz> &operator[](unsigned RegIdx) { return OwnRegs.at(RegIdx); }
  Register<Sz> operator[](unsigned RegIdx) const { return OwnRegs.at(RegIdx); }

  Register<Sz> getRegister(unsigned RegIdx) const { return OwnRegs.at(RegIdx); }

  void setRegister(RegName Reg, Register<Sz> NewValue) {
    if (strcmp(SetName, "X") == 0 && Reg == 0)
      return;
    OwnRegs.at(Reg) = NewValue;
  }

  void setRegister(RegName Reg, Register<Sz> NewValue, std::ostream &LogFile) {
    setRegister(Reg, NewValue);
    logChange(Reg, NewValue, LogFile);
  }

  void addNamedRegister(const std::string &Name) {
    NamedRegisters[Name] = 0;
  }

  void setNamedRegister(const std::string &Name, Register<Sz> NewValue) {
    NamedRegisters.at(Name) = NewValue;
  }

  void setNamedRegister(const std::string &Name, Register<Sz> NewValue,
                        std::ostream &LogFile) {
    NamedRegisters.at(Name) = NewValue;
    logChange(Name, NewValue, LogFile);
  }

  const Register<Sz> &getNamedRegister(const std::string &Name) const {
    return NamedRegisters.at(Name);
  }

  Register<Sz> &getNamedRegister(const std::string &Name) {
    return NamedRegisters.at(Name);
  }

  void dump(std::ostream &Stream) const {
    Stream << "Registers Set:\n";
    if (NamedRegisters.size() != 0) {
      for (const auto &[Name, Reg] : NamedRegisters)
        Stream << Name << " = 0x" << std::hex << Reg.to_ulong() << "\n";
    }
    Stream << "\nRegisters: \n";
    for (auto Idx = 0; Idx < static_cast<int>(OwnRegs.size()); ++Idx)
      Stream << SetName << std::dec << Idx << " = 0x" << std::hex
             << OwnRegs[Idx].to_ulong() << "\n";
    Stream << std::dec;
  }
  void print() const { dump(std::cout); }

private:
  const char *SetName;
  std::vector<Register<Sz>> OwnRegs;
  std::unordered_map<std::string, Register<Sz>> NamedRegisters;
};

template <size_t Sz>
std::bitset<Sz> operator++(Register<Sz>& Reg) {
  Reg = Reg.to_ulong() + Sz / CHAR_BIT;
  return Reg;
}

} // namespace rvdash

#endif // REGISTER_H

