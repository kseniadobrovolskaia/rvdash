#include "rvdash/InstructionSet/Instruction.h"




namespace rvdash {

std::ostream &operator<<(std::ostream& Stream, const Instruction &Instr) {
    Stream << "Instruction: \n";
    Instr.print();
    return Stream;
}

std::optional<std::shared_ptr<Instruction>> operator||(std::optional<std::shared_ptr<Instruction>> Lhs,
                                                           std::optional<std::shared_ptr<Instruction>> Rhs) {
  if (Lhs.has_value() && Rhs.has_value())
    throw std::logic_error("More than one set of instructions was able to decode one instruction");
  return Lhs.has_value() ? Lhs : Rhs;
}

} // namespace rvdash

