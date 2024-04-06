#include "Memory/Memory.h"
#include "SnippyRVdash/RVM.h"
#include "rvdash/CPU.h"
#include "rvdash/InstructionSet/InstructionSet.h"

#include <fstream>
#include <iostream>
#include <sstream>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

using namespace rvdash;

//---------------------------------SnippyRVdash------------------------------------------

/**
 * @brief class SnippyRVdash - this class is the model for Snippy.
 *                             It helps to forward all requests
 *                             from interface functions to the
 *                             rvdash library.
 */
class SnippyRVdash {

  std::optional<std::ofstream> LogFile;

  Memory<32> Mem;
  CPU<decltype(Mem), InstrSet<decltype(Mem), RV32I::RV32IInstrSet>> Cpu;

public:
  SnippyRVdash(const char *LogFilePath, unsigned long long RamSt,
               unsigned long long RamSz)
      : LogFile(LogFilePath), Mem(RamSt, RamSz), Cpu(Mem, LogFile.value()) {
    if (!LogFile.value().is_open())
      failWithError("Can't open log file " + std::string(LogFilePath));
    LogFile.value() << "====================rvdash start====================\n";
  }
  SnippyRVdash(unsigned long long RamSt, unsigned long long RamSz)
      : Mem(RamSt, RamSz), Cpu(Mem) {
    std::cout << "====================rvdash start====================\n";
  }
  ~SnippyRVdash() {
    if (LogFile.has_value()) {
      LogFile.value()
          << "===================rvdash complete==================\n";
      LogFile.value().close();
    } else
      std::cout << "===================rvdash complete==================\n";
  }

  void storeByte(uint64_t Addr, const Register<CHAR_BIT> &Byte) {
    Cpu.storeByte(Addr, Byte);
  }

  char loadByte(uint64_t Addr) const { return Cpu.loadByte(Addr).to_ulong(); }

  RVMRegT readXReg(RVMXReg Reg) const {
    return Cpu.readXReg(static_cast<unsigned>(Reg));
  }

  void setXReg(RVMXReg Reg, RVMRegT NewValue) const {
    return Cpu.setXReg(static_cast<unsigned>(Reg), NewValue);
  }

  void dumpMem() const {
    std::ofstream File("Mem.dump");
    Mem.dump(File);
  }

  void step() { Cpu.step(); }
  void increasePC() const { Cpu.increasePC(); }
  uint64_t readPC() const { return Cpu.readPC().to_ullong(); }
  void setPC(unsigned long long PcValue) const { Cpu.setPC(PcValue); }
  void print() const { Cpu.print(); }
};

//-------------------------------------RVMState------------------------------------------

/**
 * @brief struct RVMState - the main structure that connects the state of the
 *                          model and the snippy. It is declared in the
 *                          snippy and defined here. It is the argument to
 *                          all interface functions so that snippy can access
 *                          the model.
 */
struct RVMState {
  RVMConfig Config;
  std::unique_ptr<SnippyRVdash> Model;
};

//---------------------Implementation_of_rvm_interface_functions-------------------------

RVMState *rvm_modelCreate(const RVMConfig *Config) {
  std::ofstream LogFile(Config->LogFilePath);
  RVMState *State;
  // This is a temporary solution.
  // The model does not yet support ROM memory, but it will be available soon.
  unsigned long long RamStart, RamSize;
  if (Config->RomStart > Config->RamStart) {
    RamStart = Config->RamStart;
    RamSize = Config->RomStart + Config->RomSize;
  } else {
    RamStart = Config->RomStart;
    RamSize = Config->RamStart + Config->RamSize;
  }
  if (strlen(Config->LogFilePath) != 0)
    State = new RVMState(*Config, std::make_unique<SnippyRVdash>(
                                      Config->LogFilePath, RamStart, RamSize));
  else
    State = new RVMState(*Config,
                         std::make_unique<SnippyRVdash>(RamStart, RamSize));
  return State;
}

void rvm_modelDestroy(RVMState *State) { State->Model->~SnippyRVdash(); }

const RVMConfig *rvm_getModelConfig(const RVMState *State) {
  if (State == nullptr)
    failWithError("A config is required, but it is not there");
  return &State->Config;
}

int rvm_executeInstr(RVMState *State) {
  State->Model->step();
  State->Model->increasePC();
  return 0;
}

void rvm_readMem(const RVMState *State, uint64_t Addr, size_t Count,
                 char *Data) {
  auto &Model = State->Model;
  for (auto Idx = 0; Idx < Count; ++Idx)
    Data[Idx] = Model->loadByte(Addr + Idx);
}

void rvm_writeMem(RVMState *State, uint64_t Addr, size_t Count,
                  const char *Data) {
  auto &Model = State->Model;
  for (auto Idx = 0; Idx < Count; ++Idx)
    Model->storeByte(Addr + Idx, Data[Idx]);
}

uint64_t rvm_readPC(const RVMState *State) { return State->Model->readPC(); }
void rvm_setPC(RVMState *State, uint64_t NewPC) { State->Model->setPC(NewPC); }

RVMRegT rvm_readXReg(const RVMState *State, RVMXReg Reg) {
  return State->Model->readXReg(Reg);
}

void rvm_setXReg(RVMState *State, RVMXReg Reg, RVMRegT Value) {
  State->Model->setXReg(Reg, Value);
}

// There are no CSR registers in the model yet,
// but to show that everything is fine, zero is returned here
RVMRegT rvm_readCSRReg(const RVMState *State, unsigned Reg) { return 0; }
void rvm_setCSRReg(RVMState *State, unsigned Reg, RVMRegT Value) {}

int rvm_queryCallbackSupportPresent() { return 0; }
void rvm_logMessage(const char *Message) { std::cout << Message; }

// These functions are not implemented because the model does not support these
// registers

RVMRegT rvm_readFReg(const RVMState *State, RVMFReg Reg) {
  failWithError("SnippyRVdash does not yet support float registers");
  return 1;
}

void rvm_setFReg(RVMState *State, RVMFReg Reg, RVMRegT Value) {
  failWithError("SnippyRVdash does not yet support float registers");
}

int rvm_readVReg(const RVMState *State, RVMVReg Reg, char *Data,
                 size_t MaxSize) {
  failWithError("SnippyRVdash does not yet support vector registers");
  return 1;
}

int rvm_setVReg(RVMState *State, RVMVReg Reg, const char *Data,
                size_t DataSize) {
  failWithError("SnippyRVdash does not yet support vector registers");
  return 1;
}

#ifdef __cplusplus
}
#endif // __cplusplus
