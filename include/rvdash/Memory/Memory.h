#ifndef MEMORY_H
#define MEMORY_H

#include "rvdash/InstructionSet/Registers.h"
#include <set>

namespace rvdash {

template <unsigned PageSz> struct Page {
  unsigned long long FirstAddr;
  std::bitset<PageSz> Space;

  Page(unsigned long long First) : FirstAddr(First){};

  unsigned size() const { return PageSz; }
  std::vector<bool> getFrom(unsigned long long Addr) const {
    return std::vector<bool>(Space[Addr - FirstAddr], Space[PageSz - 1]);
  }
};

template <unsigned PageSz>
bool operator<(Page<PageSz> &Lhs, Page<PageSz> &Rhs) {
  return Lhs.FirstAddr < Rhs.FirstAddr;
}

template <unsigned PageSz>
bool operator<(const Page<PageSz> &Lhs, const Page<PageSz> &Rhs) {
  return Lhs.FirstAddr < Rhs.FirstAddr;
}

template <unsigned PageSz>
bool operator<(const std::_Rb_tree_const_iterator<rvdash::Page<PageSz>> &Lhs,
               const std::_Rb_tree_const_iterator<rvdash::Page<PageSz>> &Rhs) {
  return (*Lhs).FirstAddr < (*Rhs).FirstAddr;
}

template <unsigned AddrSz, unsigned long long SpaceSz = 1ull << (AddrSz - 8),
          unsigned PageSz = /* 4 kB */ 32000>
class Memory {
  std::set<Page<PageSz>> Pages;

  std::vector<bool> getBits(unsigned long long Addr, int Size) const {
    std::vector<bool> Result;

    for (auto Page = Pages.begin(); Size > 0 && Page < Pages.end(); ++Page) {
      if ((*Page).FirstAddr <= Addr && Addr < (*Page).FirstAddr + PageSz) {
        auto Bits = (*Page).getFrom(Addr);
        Result.insert(Result.end(), Bits.begin(), Bits.end());
        Size -= (*Page).FirstAddr + PageSz - Addr;
        Addr = (*Page).FirstAddr + PageSz;
      }
    }
    Result.resize(Size);
    return Result;
  }

  void validate(unsigned long long Addr, unsigned long long Size) {
    if (Addr >= SpaceSz)
      throw std::logic_error("Address exceeds address space size");
    if (!isAllocated(0, Addr, Size))
      allocate(Addr, Size);
  }

  void allocate(unsigned long long Addr, unsigned long long Size) {
    Pages.insert(Page<PageSz>(Addr / PageSz));
    if (Size > PageSz)
      allocate(Addr + PageSz, Size - PageSz);
  }

public:
  Memory() {};
  ~Memory() {};

  constexpr static unsigned short getAddrSpaceSz() { return AddrSz; }
  constexpr static unsigned short getRealSpaceSz() { return SpaceSz; }
  constexpr static unsigned short getPageSz() { return PageSz; }

  template <typename RegisterType>
  void load(unsigned long long Addr, unsigned long long Size,
            RegisterType Reg) {
    if (Size > Reg.size())
      throw std::logic_error("Size of load exceeds register size");
    validate(Addr, Size);

    Reg = getBits(Addr, Size);
  }

  std::vector<bool> load(unsigned long long Addr, unsigned long long Size) {
    validate(Addr, Size);
    std::cout << "load\n";
    return getBits(Addr, Size);
  }

  template <typename RegisterType>
  void store(unsigned long long Addr, unsigned long long Size,
             RegisterType Reg) {
    if (Size > Reg.size())
      throw std::logic_error("Size of store exceeds register size");
    validate(Addr, Size);

    throw std::logic_error("Store not implemented yet");
  }

  void store(unsigned long long Addr, unsigned long long Size) {
    validate(Addr, Size);

    throw std::logic_error("Store not implemented yet");
  }

  bool isAllocated(unsigned FirstPageForSearch, unsigned long long Addr,
                   unsigned long long Size) const {
    if (FirstPageForSearch >= Pages.size())
      return false;

    for (auto Page = Pages.cbegin(); Page<Pages.cend(), Size> 0; ++Page) {
      if ((*Page).FirstAddr <= Addr && Addr < (*Page).FirstAddr + PageSz) {
        Size -= (*Page).FirstAddr + PageSz - Addr;
        Addr = (*Page).FirstAddr + PageSz;
      }
    }

    return Size <= 0;
  };
};

} // namespace rvdash

#endif // MEMORY_H

