#include "rvdash/InstructionSet/Instruction.h"

namespace rvdash {

std::ostream &operator<<(std::ostream &Stream, const Instruction &Instr) {
  Stream << "Instruction: \n";
  Instr.print();
  return Stream;
}

bool isSame(Register<Instruction::Sz> Lhs, Register<Instruction::Sz> Rhs,
            std::bitset<Instruction::Sz> Mask) {
  return (Lhs & Mask) == (Rhs & Mask);
}

} // namespace rvdash
