#include "rvdash/InstructionSet/RV32I/InstructionSet.h"




namespace rvdash {
namespace RV32I {

//--------------------------------RV32IInstrExecutor-------------------------------------

std::shared_ptr<RV32IInstrExecutor> RV32IInstrExecutor::SingleExecutor;
std::shared_ptr<RegistersSet<32>> RV32IInstrExecutor::Registers;

std::shared_ptr<RV32IInstrExecutor> RV32IInstrExecutor::getExecutorInstance(
    std::shared_ptr<RegistersSet<32>> Regs) {
  if (SingleExecutor == nullptr)
    SingleExecutor = std::make_shared<RV32IInstrExecutor>(Regs);
  return SingleExecutor;
}

} // namespace RV32I
} // namespace rvdash

