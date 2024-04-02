#include "rvdash/InstructionSet/RV32I/InstructionSet.h"

namespace rvdash {
namespace RV32I {

//--------------------------------RV32IInstrDecoder--------------------------------------

RV32IInstrDecoder &RV32IInstrDecoder::getDecoderInstance() {
  static RV32IInstrDecoder SingleDecoder;
  return SingleDecoder;
}

//--------------------------------RV32IInstrExecutor-------------------------------------

std::shared_ptr<RV32IRegistersSet> RV32IInstrExecutor::Registers;

RV32IInstrExecutor &RV32IInstrExecutor::getExecutorInstance(
    std::shared_ptr<RV32IRegistersSet> Regs) {
  static RV32IInstrExecutor SingleExecutor(Regs);
  RV32IInstrExecutor::Registers = Regs;
  return SingleExecutor;
}

} // namespace RV32I

std::ostream &operator<<(std::ostream &Stream,
                         const typename RV32I::RV32IInstrSet &Set) {
  Set.dump(Stream);
  return Stream;
}

} // namespace rvdash
