#include "RunTests.h"
#include "Memory/Memory.h"
#include "rvdash/CPU.h"
#include "rvdash/InstructionSet/RV32I/InstructionSet.h"

#include <fstream>
#include <iostream>

namespace rvdash {

void failWithError(const std::string &Msg) {
  throw std::logic_error("\n" + Msg + "\n");
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
                     std::ostream &ResultFile) {
  Memory<Sz> Mem;
  InstrSet<Memory<Sz>, Sz, RV32I::RV32IInstrSet> InstructionSet(Mem,
                                                                ResultFile);
  CPU<Memory<Sz>, decltype(InstructionSet)> Cpu{Mem, InstructionSet};
  Cpu.execute(0 /* pc */, Program);
}

} // namespace rvdash

/**
 * @brief compileAsm - this function compiles assembler file NameAsm from
 *                     the "Data" directory using riscv toolchains
 *                     (assembler, linker and objcopy).
 *                     If any program is executed incorrectly,
 *                     it ends with an error.
 */
static void compileAsm(const std::string NameAsm, const std::string NameData) {
  auto Cmd1 =
      "riscv64-unknown-linux-gnu-as -march=rv32i -mabi=ilp32 -o tmp.o " +
      NameAsm;
  decltype(Cmd1) Cmd2 = "riscv64-unknown-linux-gnu-ld -march=rv32i -m "
                        "elf32lriscv_ilp32 -o tmp.elf tmp.o";
  auto Cmd3 = "riscv64-unknown-linux-gnu-objcopy -O binary tmp.elf " + NameData;

  if (system(Cmd1.c_str()) != 0)
    rvdash::failWithError("Error during assembly");
  if (system(Cmd2.c_str()) != 0)
    rvdash::failWithError("Error during linking");
  if (system(Cmd3.c_str()) != 0)
    rvdash::failWithError("Error during objcopy operation");
}

/**
 * @brief compileOneTest - it compile assembly from one test and the
 *                         compilation results (errors) are
 *                         written to the "LogFile".
 */
void compileOneTest(const std::string CurrTestDir, unsigned NumTest,
                    std::ostream &LogFile) {
  auto NameData = getNameData(NumTest, CurrTestDir);
  auto NameAsm = getNameAsm(NumTest, CurrTestDir);
  try {
    compileAsm(NameAsm, NameData);
  } catch (std::exception &Ex) {
    std::string ErrMess = "\nError in compilation " + std::to_string(NumTest) +
                          " test"
                          " from " +
                          CurrTestDir + ":\n" + Ex.what();
    LogFile << ErrMess;
  }
}

/**
 * @brief runOneTest - it run binary on the rvdash model and the
 *                     simulation results (traces) are
 *                     written to the "Results" directory.
 */
void runOneTest(const std::string NameData, std::ostream &ResultFile) {
  const unsigned AddrSpaceSz = 32;
  try {
    auto Program = rvdash::putProgramInBuffer(NameData);
    rvdash::generateProcess<AddrSpaceSz>(Program, ResultFile);
  } catch (std::exception &Ex) {
    std::string ErrMess = Ex.what();
    ResultFile << ErrMess;
  }
}
