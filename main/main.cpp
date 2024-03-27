#include "Error.h"
#include "rvdash/InstructionSet/RV32I/InstructionSet.h"
#include "rvdash/Memory/Memory.h"

#include <fstream>
#include <iostream>
#include <iterator>

namespace rvdash {

std::string getProgramName(const int argc, const char *argv[]) {
  if (argc < 2)
    failWithError("There are not programm file name in args");
  return argv[1];
}

std::vector<Register<CHAR_BIT>>
putProgramInBuffer(const std::string &ProgName) {
  std::vector<Register<CHAR_BIT>> Program;
  std::ifstream ProgFile(ProgName);
  if (!ProgFile.is_open())
    failWithError("Can't open file " + ProgName);
  Register<CHAR_BIT> Byte;
  while (ProgFile.read(reinterpret_cast<char *>(&Byte), 1 /* Byte */))
    Program.push_back(Byte);
  return Program;
}

template <size_t Sz>
void generateProcess(const std::vector<Register<CHAR_BIT>> &Program,
                     const char *LogFileName) {
  Memory<Sz> Mem;
  std::ofstream LogFile(LogFileName);
  InstrSet<Memory<Sz>, Sz, RV32I::RV32IInstrSet> InstructionSet(Mem, LogFile);
  CPU<Memory<Sz>, decltype(InstructionSet)> Cpu{Mem, InstructionSet};
  Cpu.execute(/* pc */ 0, Program);
}

} // namespace rvdash

int main(int Argc, char const **Argv) {
  try {
    const unsigned AddrSpaceSz = 32;
    const char *LogFileName = "trace.txt";
    auto Program =
        rvdash::putProgramInBuffer(rvdash::getProgramName(Argc, Argv));
    rvdash::generateProcess<AddrSpaceSz>(Program, LogFileName);
  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}
