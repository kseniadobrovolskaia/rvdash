#ifndef CPU_H
#define CPU_H

#include "rvdash/InstructionSet/InstructionSet.h"

#define DEBUG
#undef DEBUG

namespace rvdash {

//-------------------------------------CPU-----------------------------------------------

/**
 * @brief class CPU - a class that controls the simulation process from writing
 *                    a program into virtual memory to executing instructions
 *                    (this is the model hart).
 */
template <typename MemoryType, typename InstrSetType,
          size_t AddrSz = MemoryType::getAddrSpaceSz()>
class CPU {

private:
  MemoryType &VirtualMemory;
  InstrSetType &ExtSet;

  std::ostream &LogFile;

public:
  CPU(MemoryType &Mem, InstrSetType &E)
      : VirtualMemory(Mem), ExtSet(E), LogFile(E.LogFile) {
    if (AddrSz > MemoryType::getAddrSpaceSz())
      failWithError("The requested address space size exceeds the size that "
                    "virtual memory has");
  }

  void dump() const { dump(LogFile); }
  void dump(std::ostream &Stream) const {
    Stream << "\nCPU:\n";
    ExtSet.dump(Stream);
    std::ofstream File("Mem.dump");
    VirtualMemory.dump(File);
    Stream << "Virtual memory dump in file Mem.dump\n";
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

  void step() const { ExtSet.step(); }
  void increasePC() const { ExtSet.increasePC(); }
  void setPC(unsigned long long PcValue) const { ExtSet.setPC(PcValue); }
};

} // namespace rvdash
#endif // CPU_H
