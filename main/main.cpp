#include "Error.h"
#include "Memory/Memory.h"
#include "rvdash/CPU.h"
#include "rvdash/InstructionSet/InstructionSet.h"

#include <fstream>
#include <getopt.h>
#include <iostream>
#include <iterator>

#define DEBUG
#undef DEBUG

namespace rvdash {

static std::optional<const char *> LogFilePath;
static std::optional<unsigned long long> RamSize;
static std::optional<unsigned long long> Pc;

// clang-format off
static struct option CmdLineOpts[] = {
    {"help",             no_argument,        0,  'h'},
    {"ram-size",         required_argument,  0,  'r'},
    {"program-counter",  required_argument,  0,  'p'},
    {"trace-output",     required_argument,  0,  't'},
    {0,                  0,                  0,   0}};
// clang-format on

static void printHelp(const char *ProgName, int ErrorCode) {
  std::cerr << "USAGE:     " << ProgName << "   [options]   <binary_file>\n\n";
  std::cerr << "OPTIONS: \n";
  struct option *opt = CmdLineOpts;
  while (opt->name) {
    if (isprint(opt->val))
      std::cerr << "\t   -" << static_cast<char>(opt->val) << "\t --"
                << opt->name << "\n";
    else
      std::cerr << "\t     \t --" << opt->name << "\n";
    opt++;
  }
  exit(ErrorCode);
}

/**
 * @brief parseCmdLine - it parses the command line arguments and returns the
 *                       index for the binary that should be executed.
 */
static int parseCmdLine(int Argc, char **Argv) {
  int NextOpt;
  uint64_t RamSizeMB = 0;
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
      RamSizeMB = atoll(optarg);
      if (RamSizeMB) {
#ifdef DEBUG
        std::cerr << "Setting ram-size to " << RamSizeMB << " MB\n";
#endif
        RamSize = RamSizeMB << 20;
      } else
        failWithError("Invalid ram-size " + std::string(optarg) + " provided");
      break;
    case 'p':
      Pc = atoll(optarg);
#ifdef DEBUG
      std::cerr << "Setting start pc to " << optarg << "\n";
#endif
      break;
    case 'h':
      printHelp(Argv[0], 0);
      break;
    case 't':
      LogFilePath = optarg;
#ifdef DEBUG
      std::cerr << "Using " << LogFilePath.value() << " for trace output\n";
#endif
      break;
    case '?':
      printHelp(Argv[0], 1);
      break;
    }
  }
  if (optind >= Argc)
    failWithError("No binary file in args");
#ifdef DEBUG
  std::cerr << "Binary file " << Argv[optind] << "\n";
#endif
  return optind;
}

std::vector<Register<CHAR_BIT>>
putProgramInBuffer(const std::string &ProgName) {
  std::vector<Register<CHAR_BIT>> Program;
  std::ifstream ProgFile(ProgName);
  if (!ProgFile.is_open())
    failWithError("Can't open file " + ProgName);
  Register<CHAR_BIT> Byte;
  while (ProgFile.read(reinterpret_cast<char *>(&Byte), /* Byte */ 1))
    Program.push_back(Byte);
  return Program;
}

template <size_t Sz>
void generateProcess(const std::vector<Register<CHAR_BIT>> &Program,
                     std::ostream &LogFile) {
  Memory<Sz> Mem;
  if (RamSize.has_value())
    Mem.setRamSize(RamSize.value());
  InstrSet<Memory<Sz>, RV32I::RV32IInstrSet> InstructionSet(Mem, LogFile);
  CPU<Memory<Sz>, decltype(InstructionSet)> Cpu{Mem, InstructionSet};
  Cpu.execute(Pc.value(), Program);
}

} // namespace rvdash

int main(int Argc, char **Argv) {
  try {
    const unsigned AddrSpaceSz = 32;
    auto BinIdx = rvdash::parseCmdLine(Argc, Argv);
    auto Program = rvdash::putProgramInBuffer(Argv[BinIdx]);
    rvdash::Pc = rvdash::Pc.has_value() ? rvdash::Pc.value() : 0;
    if (!rvdash::LogFilePath.has_value()) {
      std::ostream &LogFile = std::cout;
      rvdash::generateProcess<AddrSpaceSz>(Program, LogFile);
    } else {
      std::ofstream LogFile(rvdash::LogFilePath.value());
      rvdash::generateProcess<AddrSpaceSz>(Program, LogFile);
    }
  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}
