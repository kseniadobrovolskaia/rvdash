#include "Error.h"
#include "rvdash/InstructionSet/RV32I/InstructionSet.h"
#include "rvdash/Memory/Memory.h"

#include <fstream>
#include <iostream>

namespace rvdash {

void failWithError(const std::string &Msg) {
  throw std::logic_error("\n\n" + Msg);
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
                     const std::string &LogFileName) {
  Memory<Sz> Mem;
  std::ofstream LogFile(LogFileName);
  InstrSet<Memory<Sz>, Sz, RV32I::RV32IInstrSet> InstructionSet(Mem, LogFile);
  CPU<Memory<Sz>, decltype(InstructionSet)> Cpu{Mem, InstructionSet};
  Cpu.execute(0 /* pc */, Program);
}

} // namespace rvdash

/**
 * @brief RunTests - This function compiles all assembler files from
 *                   the "Data" directory using riscv toolchains
 *                   (assembler, linker and objcopy).
 *
 *                   Then it runs the resulting binaries on the rvdash
 *                   simulator and the simulation results (traces) are
 *                   written to the "Results" directory.
 *
 *                   Then Google testing begins, which, using a FileCheck,
 *                   finds the necessary execution elements (writes to
 *                   registers or memory) in the trace.
 */
void RunTests() {
  try {
    unsigned CountTests = 11;
    const unsigned AddrSpaceSz = 32;
    for (auto NumTest = 1; NumTest < CountTests; ++NumTest) {
      std::cout << "Start test number " << NumTest << std::endl;
      std::string NameData = "../../Test/rvdashTests/Data/" +
                             std::to_string(NumTest) + "_TestData.bin";
      std::string NameAsm = "../../Test/rvdashTests/Data/" +
                            std::to_string(NumTest) + "_TestData.S";
      std::string NameResults = "../../Test/rvdashTests/Results/" +
                                std::to_string(NumTest) + "_TestResults.txt";

      //
      std::string Cmd1 =
          "riscv64-unknown-linux-gnu-as -march=rv32i -mabi=ilp32 -o tmp.o " +
          NameAsm;
      std::string Cmd2 = "riscv64-unknown-linux-gnu-ld -march=rv32i -m "
                         "elf32lriscv_ilp32 -o tmp.elf tmp.o";
      std::string Cmd3 =
          "riscv64-unknown-linux-gnu-objcopy -O binary tmp.elf " + NameData;

      system(Cmd1.c_str());
      system(Cmd2.c_str());
      system(Cmd3.c_str());

      auto Program = rvdash::putProgramInBuffer(NameData);
      rvdash::generateProcess<AddrSpaceSz>(Program, NameResults);
    }
  } catch (std::exception &Ex) {
    std::cerr << Ex.what() << "\n";
    exit(EXIT_FAILURE);
  }
}
