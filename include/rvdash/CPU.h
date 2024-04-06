#ifndef CPU_H
#define CPU_H

#include "rvdash/InstructionSet/Instruction.h"

#define DEBUG
#undef DEBUG

namespace rvdash {

//-------------------------------------CPU-----------------------------------------------

/**
 * @brief class CPU - a class that controls the simulation process from writing
 *                    a program into virtual memory to executing instructions
 *                    (this is the model hart).
 */
template <typename MemoryType, typename InstrSetType> class CPU {

private:
  MemoryType &VirtualMemory;
  InstrSetType ExtSet;

  std::ostream &LogFile;

public:
  CPU(MemoryType &Mem, std::ostream &LogFile = std::cout,
      bool IsForTests = false)
      : VirtualMemory(Mem), ExtSet(Mem, LogFile, IsForTests), LogFile(LogFile) {
  }

  void dump() const { dump(LogFile); }
  void dump(std::ostream &Stream) const {
    Stream << "\nCPU:\n\n--------------------------------------------\n1. ";
    ExtSet.dump(Stream);
    std::ofstream File("Mem.dump");
    VirtualMemory.dump(File);
    Stream << "2. Virtual memory dump in file "
              "Mem.dump\n--------------------------------------------\n";
  }
  void print() const { dump(std::cout); }

  void
  storeProgramInVirtualMemory(const std::vector<Register<CHAR_BIT>> &Program) {
    unsigned long long NumStore = 0;
    for (const auto &Byte : Program) {
      VirtualMemory.store(NumStore, /* Size */ 1, Byte);
      NumStore++;
    }
  }

  void storeByte(uint64_t Addr, const Register<CHAR_BIT> &Byte) const {
    VirtualMemory.store(Addr, /* Size */ 1, Byte);
  }

  Register<CHAR_BIT> loadByte(uint64_t Addr) const {
    Register<CHAR_BIT> Byte;
    VirtualMemory.load(Addr, /* Size */ 1, Byte);
    return Byte;
  }

  void execute(unsigned long long Pc,
               const std::vector<Register<CHAR_BIT>> &Program) {
    if (Pc % Instruction::Sz_b != 0)
      failWithError("Pc start address is not aligned to 4 bytes");

    storeProgramInVirtualMemory(Program);
#ifdef DEBUG
    std::ofstream File("Mem_debug.dump");
    VirtualMemory.dump(File);
#endif
    LogFile << "====================Simulation started====================\n";
    ExtSet.executeProgram(Pc);
    LogFile << "===================Simulation completed===================\n";
  }

  void step() { ExtSet.step(); }
  void increasePC() const { ExtSet.increasePC(); }
  Register<InstrSetType::AddrSz> readPC() const { return ExtSet.readPC(); }
  void setPC(unsigned long long PcValue) const { ExtSet.setPC(PcValue); }
  uint64_t readXReg(unsigned Reg) const { return ExtSet.readXReg(Reg); }
  void setXReg(unsigned Reg, uint64_t NewValue) const {
    ExtSet.setXReg(Reg, NewValue);
  }
};

} // namespace rvdash
#endif // CPU_H
