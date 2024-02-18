#include "RVdash/InstructionSet/RV32I/InstructionSet.h"
#include "Error.h"




namespace rvdash {

enum class Extentions {
  RV32I,
  F,
  V,
  C,
  A
};

auto getSelectedInstructionSets(std::string ExStr) {
  const auto &ExEnumNames = magic_enum::enum_names<Extentions>();
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
      failWithError("Расширение " + Ex + " встретилось несколько раз");
      
  });
  
  if (!ExStr.empty())
    failWithError("Выбраны неподдерживаемые расширения " + ExStr);
    
  return SelectedExs;
}

void generateProcess(const std::string ExStr) {
  auto Extentions = getSelectedInstructionSets(ExStr);
  std::cout << "Selected Exs:\n";
  for (const auto &Ex : Extentions)
      std::cout << Ex << "\n";

  RV32I::InstrDecoder Decoder;
  auto Instr = Decoder.tryDecode(0b0000'0001'0110'1010'1000'1010'1011'0011);
  assert(Instr.has_value());
  Instr.value()->print();

}

} // namespace rvdash


int main(int Argc, char const **Argv)
{
  try {
    if (Argc < 2)
      rvdash::failWithError("Не выбраны расширения");
    rvdash::generateProcess(Argv[1]);
  }
  catch(std::exception & ex)
  {
    std::cout << ex.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

