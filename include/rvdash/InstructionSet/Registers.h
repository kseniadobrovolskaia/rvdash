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

namespace rvdash {

//-----------------------------------RegistersSet----------------------------------------

/**
 * @brief class RegistersSet - class to represent a register file belonging to a
 *                             specific extension. Contains numbered and named
 *                             registers.
 */
template <size_t Sz> using Register = std::bitset<Sz>;

template <size_t Sz>
Register<Sz> operator||(Register<Sz> Lhs, Register<Sz> Rhs) {
  return Lhs.has_value() ? Lhs : Rhs;
}

template <size_t Sz>
class RegistersSet {
protected:
  virtual void logChange(unsigned RegIdx, Register<Sz> NewValue,
                         std::ostream &LogFile) const {
    LogFile << SetName << std::dec << RegIdx << " <- 0x" << std::hex
            << NewValue.to_ulong() << "\n";
    LogFile << std::dec;
  }

  void logChange(std::string RegName, Register<Sz> NewValue,
                 std::ostream &LogFile) const {
    LogFile << RegName << " <- 0x" << std::hex << NewValue.to_ulong() << "\n";
    LogFile << std::dec;
  }

  virtual const Register<Sz> &operator[](unsigned RegIdx) const {
    return OwnRegs.at(RegIdx);
  }
  virtual Register<Sz> &operator[](unsigned RegIdx) {
    return OwnRegs.at(RegIdx);
  }

public:
  RegistersSet() {};
  RegistersSet(const char *Name, unsigned Count)
      : SetName(Name), OwnRegs(Count){};

  virtual ~RegistersSet(){};

  virtual const Register<Sz> &getRegister(unsigned RegIdx) const {
    return OwnRegs.at(RegIdx);
  }
  virtual Register<Sz> getRegister(unsigned RegIdx) {
    return OwnRegs.at(RegIdx);
  }

  virtual const Register<Sz> &getNamedRegister(const std::string &Name) const {
    return NamedRegisters.at(Name);
  }

  virtual Register<Sz> &getNamedRegister(const std::string &Name) {
    return NamedRegisters.at(Name);
  }

  virtual void setRegister(unsigned RegIdx, const Register<Sz> &NewValue) {
    OwnRegs.at(RegIdx) = NewValue;
  }

  virtual void setRegister(unsigned RegIdx, const Register<Sz> &NewValue,
                           std::ostream &LogFile) {
    setRegister(RegIdx, NewValue);
    logChange(RegIdx, NewValue, LogFile);
  }

  virtual void addNamedRegister(const std::string &Name) {
    NamedRegisters[Name] = 0;
  }

  virtual void setNamedRegister(const std::string &Name,
                                const Register<Sz> &NewValue) {
    NamedRegisters.at(Name) = NewValue;
  }

  virtual void setNamedRegister(const std::string &Name,
                                const Register<Sz> &NewValue,
                                std::ostream &LogFile) {
    NamedRegisters.at(Name) = NewValue;
    logChange(Name, NewValue, LogFile);
  }

  virtual void dump(std::ostream &Stream) const {
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
  virtual void print() const { dump(std::cout); }

protected:
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

