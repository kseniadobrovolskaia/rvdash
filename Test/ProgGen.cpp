#include <fstream>
#include <iostream>
#include <bitset>
#include <exception>


void generateInstructions(const char *FileName) {
  std::ofstream File;
  File.open(FileName);
  std::bitset<32> Instr;
 
  // LUI (U-type) Rd = 2, Imm = 5 
  Instr = std::bitset<32>(0b00000000000000000101'00010'0110111);
  File.write(reinterpret_cast<char const*>(&Instr), sizeof(Instr));
  
  // LUI (U-type) Rd = 3, Imm = 10 
  Instr = std::bitset<32>(0b00000000000000001010'00011'0110111);
  File.write(reinterpret_cast<char const*>(&Instr), sizeof(Instr));
  
  // ADD (R-type) Rd = 1, Rs1 = 2, Rs2 = 3
  Instr = std::bitset<32>(0b0000000'00011'00010'000'00001'0110011);
  File.write(reinterpret_cast<char const*>(&Instr), sizeof(Instr));
 
  // LW (I-type) Rd = 2, Imm = 0, Rs1 = 512
  Instr = std::bitset<32>(0b001000000000'00000'010'00010'0000011);
  File.write(reinterpret_cast<char const*>(&Instr), sizeof(Instr));
  
  // LW (I-type) Rd = 3, Imm = 0, Rs1 = 544
  Instr = std::bitset<32>(0b001000100000'00000'010'00011'0000011);
  File.write(reinterpret_cast<char const*>(&Instr), sizeof(Instr));

  // ADD (R-type) Rd = 1, Rs1 = 2, Rs2 = 3
  Instr = std::bitset<32>(0b0000000'00011'00010'000'00001'0110011);
  File.write(reinterpret_cast<char const*>(&Instr), sizeof(Instr));
 
  // EBREAK
  Instr = std::bitset<32>(0b000000000001'00000'000'00000'1110011);
  File.write(reinterpret_cast<char const*>(&Instr), sizeof(Instr));
  File.close();
}


int main(int Argc, char const **Argv) {
  if (Argc < 2)
    throw std::logic_error("What file must be generated?");
  generateInstructions(Argv[1]);
}
