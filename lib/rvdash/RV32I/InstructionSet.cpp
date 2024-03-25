#include "rvdash/InstructionSet/RV32I/InstructionSet.h"

namespace rvdash {
namespace RV32I {

//--------------------------------RV32IInstrDecoder--------------------------------------

RV32IInstrDecoder &RV32IInstrDecoder::getDecoderInstance() {
  static RV32IInstrDecoder SingleDecoder;
  return SingleDecoder;
}

//--------------------------------RV32IInstrExecutor-------------------------------------

std::shared_ptr<RegistersSet<RegSz>> RV32IInstrExecutor::Registers;

RV32IInstrExecutor &RV32IInstrExecutor::getExecutorInstance(
    std::shared_ptr<RegistersSet<RegSz>> Regs) {
  static RV32IInstrExecutor SingleExecutor(Regs);
  return SingleExecutor;
}

} // namespace RV32I
} // namespace rvdash
