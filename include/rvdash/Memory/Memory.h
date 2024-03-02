#ifndef MEMORY_H
#define MEMORY_H

#include "rvdash/InstructionSet/Registers.h"
#include <cmath>




namespace rvdash {

template <size_t AddrSz, unsigned long long SpaceSz = 1ull << (AddrSz - 8)>
class Memory {
  std::bitset<SpaceSz> Space;

public:
  Memory() {};
  ~Memory() {};

  unsigned short getAddrSpaceSz() const { return AddrSz; }
  unsigned short getRealSpaceSz() const { return SpaceSz; }

  template <size_t RegSz>
  void load(unsigned long long Addr, unsigned long long Size, Register<RegSz> Reg) const {
    for (auto Bit = Addr; Bit < Addr + Size; Bit++)
      Reg[Bit - Addr] = Space[Bit];
  }

  template <size_t RegSz>
  void store(unsigned long long Addr, unsigned long long Size, Register<RegSz> Reg) {
    Space[Addr] = Reg[0];
    for (auto Bit = Addr; Bit < Addr + Size; Bit++)
      Space[Bit] = Reg[Bit - Addr];
  }


};


} // namespace rvdash

#endif // MEMORY_H

