#include "rvdash/InstructionSet/RV32I/InstructionSet.h"
#include "rvdash/InstructionSet/M/InstructionSet.h"
#include "Error.h"




namespace rvdash {

std::string getExtensions(const int argc, const char *argv[]) {
  if (argc < 2)
    throw std::logic_error("There are not Extensions in args");
  return argv[1];
}

std::string getProgramName(const int argc, const char *argv[]) {
  if (argc < 3)
    throw std::logic_error("There are not programm file name in args");
  return argv[2];
}

template <size_t Sz>
std::vector<Register<Sz>> putProgramInBuffer(const std::string &ProgName) {
  std::vector<Register<Sz>> Program;
  std::ifstream ProgFile(ProgName);
  if (!ProgFile.is_open())
    throw std::logic_error("Can't open file " + ProgName);
  
  Register<Sz> Command;
  while (ProgFile.read(reinterpret_cast<char *>(&Command), sizeof(Command)))
    Program.push_back(Command);
  ProgFile.close();
  return Program;
}

auto getSelectedInstructionSets(std::string ExStr) {
  const auto &ExEnumNames = magic_enum::enum_names<Extensions>();
  std::vector<std::string> ExNames;
  for (const auto &Ex : ExEnumNames)
    ExNames.emplace_back(Ex);
  std::vector<std::string> SelectedExs;
  std::for_each(ExNames.begin(), ExNames.end(), [&ExStr, &SelectedExs](const auto &Ex) 
  { 
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
void generateProcess(const std::string ExStr, const std::vector<Register<Sz>> &Program) {
  auto Extensions = getSelectedInstructionSets(ExStr);
  std::cout << "Selected Exs:\n";
  for (const auto &Ex : Extensions)
      std::cout << Ex << "\n";
  std::cout << "\n\n";
  Memory<Sz> Mem;
  CPU<Sz, RV32I::RV32IInstrSet, M::MInstrSet> Cpu{InstrSet<Sz, RV32I::RV32IInstrSet, M::MInstrSet>(Mem)};
  Cpu.print();

  Cpu.execute(Program);
}

} // namespace rvdash


int main(int Argc, char const **Argv)
{
  try {
    const unsigned SizePC = 32; 
    auto Program = rvdash::putProgramInBuffer<SizePC>(rvdash::getProgramName(Argc, Argv));
    auto A = rvdash::getExtensions(Argc, Argv);
    5;
    rvdash::generateProcess(A, Program);
  }
  catch(std::exception & ex)
  {
    std::cout << ex.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

