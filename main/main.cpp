#include "Error.h"
#include "rvdash/InstructionSet/RV32I/InstructionSet.h"
#include "rvdash/Memory/Memory.h"

#include <fstream>
#include <getopt.h>
#include <iostream>
#include <iterator>

namespace rvdash {

static std::optional<const char *> LogFilePath;
static unsigned long long RvRamSize = 1ull << 20;
static unsigned long long Pc;

static struct option CmdLineOpts[] = {
    {"help", no_argument, 0, 'h'},
    {"ram-size", required_argument, 0, 'r'},
    {"program-counter", required_argument, 0, 'p'},
    {"trace-output", required_argument, 0, 't'},
    {0, 0, 0, 0}};

static void printHelp(const char *Argv0, int ErrorCode) {
  std::cerr << "USAGE:   " << Argv0 << " [options] <elf_file>\n\n";
  std::cerr << "OPTIONS: \n";
  struct option *opt = CmdLineOpts;
  while (opt->name) {
    if (isprint(opt->val))
      std::cerr << "\t -" << static_cast<char>(opt->val) << "\t --" << opt->name
                << "\n";
    else
      std::cerr << "\t   \t --" << opt->name << "\n";
    opt++;
  }
  exit(ErrorCode);
}

/**
 * @brief parseCmdLine - it parses the command line arguments and returns the
 *                      index for the binary that should be executed.
 */
static int parseCmdLine(int Argc, char **Argv) {
  int NextOpt;
  uint64_t RamSize = 0;
  while (true) {
    NextOpt = getopt_long(Argc, Argv,
                          "h"
                          "r:"
                          "p:"
                          "t:",
                          CmdLineOpts, NULL);
    if (NextOpt == -1)
      break;
    switch (NextOpt) {
    case 'r':
      RamSize = atol(optarg);
      if (RamSize) {
        std::cerr << "Setting ram-size to " << RamSize << " Mb\n";
        RvRamSize = RamSize << 20;
      } else
        failWithError("Invalid ram-size " + std::string(optarg) + " provided");
      break;
    case 'p':
      Pc = atol(optarg);
      std::cerr << "Setting start pc to " << Pc << "\n";
      break;
    case 'h':
      printHelp(Argv[0], 0);
      break;
    case 't':
      LogFilePath = optarg;
      std::cerr << "Using " << LogFilePath.value() << " for trace output\n";
      break;
    case '?':
      printHelp(Argv[0], 1);
      break;
    }
  }
  if (optind >= Argc)
    failWithError("No binary file in args");
  std::cerr << "Binary file " << Argv[optind] << "\n";
  return optind;
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
  Memory<Sz> Mem(RvRamSize);
  std::ofstream LogFile(LogFileName);
  InstrSet<Memory<Sz>, Sz, RV32I::RV32IInstrSet> InstructionSet(Mem, LogFile);
  CPU<Memory<Sz>, decltype(InstructionSet)> Cpu{Mem, InstructionSet};
  Cpu.execute(Pc, Program);
}

} // namespace rvdash

int main(int Argc, char **Argv) {
  try {
    const unsigned AddrSpaceSz = 32;
    auto ElfIdx = rvdash::parseCmdLine(Argc, Argv);
    auto Program = rvdash::putProgramInBuffer(Argv[ElfIdx]);
    if (!rvdash::LogFilePath.has_value())
      rvdash::LogFilePath = "trace.txt";
    rvdash::generateProcess<AddrSpaceSz>(Program, rvdash::LogFilePath.value());
  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}
