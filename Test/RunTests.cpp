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

void generateProcess(const std::vector<Register<CHAR_BIT>> &Program,
                     const std::string &LogFileName) {
  const unsigned Sz = 32;
  Memory<Sz> Mem;
  std::ofstream LogFile(LogFileName);
  InstrSet<Memory<Sz>, Sz, RV32I::RV32IInstrSet> InstructionSet(Mem, LogFile);
  CPU<Memory<Sz>, decltype(InstructionSet)> Cpu{Mem, InstructionSet};
  Cpu.execute(0 /* pc */, Program);
}

} // namespace rvdash

void RunTests() {
  try {
    for (auto NumTest = 1; NumTest < 11; ++NumTest) {
      std::cout << "Start test number " << NumTest << std::endl;
      std::string NameData = "../../Test/rvdashTests/Data/" +
                             std::to_string(NumTest) + "_TestData.bin";
      std::string NameAs = "../../Test/rvdashTests/Data/" +
                           std::to_string(NumTest) + "_TestData.S";
      std::string NameResults = "../../Test/rvdashTests/Results/" +
                                std::to_string(NumTest) + "_TestResults.txt";

      std::string Cmd1 =
          "riscv64-unknown-linux-gnu-as -march=rv32i -mabi=ilp32 -o tmp.o " +
          NameAs;
      std::string Cmd2 = "riscv64-unknown-linux-gnu-ld -march=rv32i -m "
                         "elf32lriscv_ilp32 -o tmp.elf tmp.o";
      std::string Cmd3 =
          "riscv64-unknown-linux-gnu-objcopy -O binary tmp.elf " + NameData;

      system(Cmd1.c_str());
      system(Cmd2.c_str());
      system(Cmd3.c_str());

      auto Program = rvdash::putProgramInBuffer(NameData);
      rvdash::generateProcess(Program, NameResults);
    }
  } catch (std::exception &Ex) {
    std::cerr << Ex.what() << "\n";
    exit(EXIT_FAILURE);
  }
}
