#include "Error.h"
#include "rvdash/InstructionSet/M/InstructionSet.h"
#include "rvdash/InstructionSet/RV32I/InstructionSet.h"
#include "rvdash/Memory/Memory.h"

#include <fstream>
#include <iostream>
#include <iterator>

namespace rvdash {

std::string getExtensions(const int argc, const char *argv[]) {
  if (argc < 3)
    return {};
  return argv[1];
}

std::string getProgramName(const int argc, const char *argv[]) {
  if (argc < 2)
    failWithError("There are not programm file name in args");
  if (argc == 3)
    return argv[2];
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

auto getSelectedInstructionSets(std::string ExStr) {
  const auto &ExEnumNames = magic_enum::enum_names<Extensions>();
  std::vector<std::string> ExNames;
  for (const auto &Ex : ExEnumNames)
    ExNames.emplace_back(Ex);
  std::vector<std::string> SelectedExs;
  std::for_each(ExNames.begin(), ExNames.end(),
                [&ExStr, &SelectedExs](const auto &Ex) {
                  auto Pos = ExStr.find(Ex);
                  if (Pos != ExStr.npos) {
                    SelectedExs.emplace_back(Ex);
                    ExStr.erase(Pos, Ex.size());
                  }
                  if (ExStr.find(Ex) != ExStr.npos)
                    failWithError("Extension " + Ex + " parsed several times");
                });
  if (!ExStr.empty())
    failWithError("Unsupported extensions selected " + ExStr);
  return SelectedExs;
}

template <size_t Sz>
void generateProcess(const std::string ExStr,
                     const std::vector<Register<CHAR_BIT>> &Program) {
  auto Extensions = getSelectedInstructionSets(ExStr);

  Memory<Sz> Mem;
  InstrSet<Memory<Sz>, Sz, RV32I::RV32IInstrSet, M::MInstrSet> InstructionSet(Mem);
  CPU<Memory<Sz>, decltype(InstructionSet)> Cpu{Mem, InstructionSet};
  Cpu.execute(0 /* pc */, Program);
  Cpu.print();
}

} // namespace rvdash


int main(int Argc, char const **Argv)
{
  try {
    const unsigned SizePC = 32;
    auto Program =
        rvdash::putProgramInBuffer(rvdash::getProgramName(Argc, Argv));
    rvdash::generateProcess<SizePC>(rvdash::getExtensions(Argc, Argv), Program);
  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

