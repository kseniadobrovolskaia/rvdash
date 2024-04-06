#include "Error.h"
#include "Memory/Memory.h"
#include "rvdash/CPU.h"
#include "rvdash/InstructionSet/InstructionSet.h"

#include <fstream>
#include <getopt.h>

#define DEBUG
#undef DEBUG

namespace rvdash {

static std::optional<const char *> LogFilePath;
static std::optional<unsigned long long> RamStart;
static std::optional<unsigned long long> RamSize;
static std::optional<unsigned long long> Pc;

#define RAM_START 1000
#define RAM_SIZE 1001
// clang-format off
static struct option CmdLineOpts[] = {
    {"help",             no_argument,        0,  'h'        },
    {"ram-start",        required_argument,  0,  RAM_START  },
    {"ram-size",         required_argument,  0,  RAM_SIZE   },
    {"program-counter",  required_argument,  0,  'p'        },
    {"trace-output",     required_argument,  0,  't'        },
    {0,                  0,                  0,   0         }};
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
 * @brief setValue - it helps process incorrect values, which are given
 *                   for unsigned long long argument.
 */
static void setValue(const char *NameValue, const char *GivenValue,
                     std::optional<unsigned long long> &SetVar) {
  // find sign
  while (isspace((unsigned char)*GivenValue))
    GivenValue++;
  char Sign = *GivenValue;
  char *Endptr; //  Store the location where conversion stopped
  unsigned long TryValue = strtoul(GivenValue, &Endptr, /* base */ 10);

  if (GivenValue == Endptr)
    failWithError("Invalid " + std::string(NameValue) + " " +
                  std::string(GivenValue) + " provided");
  else if (*Endptr)
    failWithError("Extra text after " + std::string(NameValue));
  else if (Sign == '-' && TryValue != 0)
    failWithError("Negative " + std::string(NameValue));
  SetVar = TryValue;
#ifdef DEBUG
  std::cerr << "Setting " << NameValue << " to " << TryValue << "\n";
#endif
}

/**
 * @brief parseCmdLine - it parses the command line arguments and returns the
 *                       index for the binary that should be executed.
 */
static int parseCmdLine(int Argc, char **Argv) {
  int NextOpt;
  while (true) {
    NextOpt = getopt_long(Argc, Argv,
                          "h"
                          "p:"
                          "t:",
                          CmdLineOpts, NULL);
    if (NextOpt == -1)
      break;
    switch (NextOpt) {
    case RAM_START:
      setValue("ram-start", optarg, RamStart);
      break;
    case RAM_SIZE:
      setValue("ram-size", optarg, RamSize);
      RamSize.value() <<= 20;
      break;
    case 'p':
      setValue("program counter", optarg, Pc);
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
  if (!RamStart.has_value())
    RamStart = Memory<Sz>::getDefaultRamStart();
  if (!RamSize.has_value())
    RamSize = Memory<Sz>::getDefaultRamSz();
  Memory<Sz> Mem(RamStart.value(), RamSize.value());
  CPU<decltype(Mem), InstrSet<decltype(Mem), RV32I::RV32IInstrSet>> Cpu{
      Mem, LogFile};
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
