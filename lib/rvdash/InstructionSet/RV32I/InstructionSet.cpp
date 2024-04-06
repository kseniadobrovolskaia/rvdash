#include "rvdash/InstructionSet/RV32I/InstructionSet.h"

namespace rvdash {
namespace RV32I {

std::shared_ptr<RV32IRegistersSet> RV32IInstrExecutor::Registers;

} // namespace RV32I

std::ostream &operator<<(std::ostream &Stream,
                         const typename RV32I::RV32IInstrSet &Set) {
  Set.dump(Stream);
  return Stream;
}

} // namespace rvdash
