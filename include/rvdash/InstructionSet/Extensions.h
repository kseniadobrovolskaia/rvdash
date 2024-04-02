#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include "rvdash/InstructionSet/Instruction.h"

namespace rvdash {

//------------------------------------Extensions-----------------------------------------

enum class Extensions {
  RV32I,
};

//---------------------------------ExecuteFuncType---------------------------------------

template <typename InstrSetType>
using ExecuteFuncType = void (*)(Instruction, InstrSetType &Set);

} // namespace rvdash

#endif // EXTENSIONS_H
