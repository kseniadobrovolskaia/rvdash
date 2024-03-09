#include "rvdash/InstructionSet/Instruction.h"




namespace rvdash {

std::ostream &operator<<(std::ostream& Stream, const Instruction &Instr) {
    Stream << "Instruction: \n";
    Instr.print();
    return Stream;
}

} // namespace rvdash

