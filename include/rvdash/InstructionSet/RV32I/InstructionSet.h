#ifndef R32I_INSTRUCTION_SET_H
#define R32I_INSTRUCTION_SET_H

#include "rvdash/InstructionSet/InstructionSet.h"




namespace rvdash {
namespace RV32I {


//--------------------------------RV32IInstrDecoder--------------------------------------

class RV32IInstrDecoder {
public:
  RV32IInstrDecoder() {};
  std::optional<std::shared_ptr<rvdash::Instruction>> tryDecode(Register<Instruction::Sz> Instr) const; 
};

//--------------------------------RV32IInstrExecutor-------------------------------------

class RV32IInstrExecutor {

public:
  RV32IInstrExecutor() {};
  void executeI_Instr() const{ std::cout << "execute_I_Instr\n"; };
  void executeS_Instr() const{ std::cout << "execute_S_Instr\n"; };
  void executeB_Instr() const{ std::cout << "execute_B_Instr\n"; };
  void executeU_Instr() const{ std::cout << "execute_U_Instr\n"; };
  void executeJ_Instr() const{ std::cout << "execute_J_Instr\n"; };

  void execute(std::shared_ptr<Instruction> Instr) const;


};

//----------------------------------RV32IInstrSet----------------------------------------

class RV32IInstrSet {

public:
  using PcSz_t = const size_t;
  static PcSz_t PcSz = 32;
  static const size_t RegSz = 32;

protected:
  RV32IInstrDecoder Decoder;
  RV32IInstrExecutor Executor;
  RegistersSet<RegSz> Registers;

public:
  RV32IInstrSet() : Registers(X0, X31 - X0) {
    Registers.addNamedRegister("pc");
    ++Registers.getNamedRegister("pc");
  }

  virtual ~RV32IInstrSet() {};

  Register<PcSz> *getPC() { return &Registers.getNamedRegister("pc"); }

  void print() const { std::cout << "RV32IInstrSet\n"; }

  std::optional<std::shared_ptr<Instruction>> tryDecode(Register<Instruction::Sz> Instr) const {
    return Decoder.tryDecode(Instr);
  }

  bool tryExecute(std::shared_ptr<Instruction> Instr) const {
    if (Instr->getExtension() != Extensions::RV32I)
      return true;
    Executor.execute(Instr); 
    return false;
  }
  

};


std::ostream& operator<<(std::ostream& stream, const typename RV32I::RV32IInstrSet& counter);

} // namespace RV32I
} // namespace rvdash

#endif// R32I_INSTRUCTION_SET_H

