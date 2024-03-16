#ifndef MEMORY_H
#define MEMORY_H

#include <bitset>
#include <iostream>
#include <set>
#include <vector>

#include "Error.h"

namespace rvdash {

//------------------------------------Page-----------------------------------------------

template <unsigned PageSz> struct Page {
  unsigned long long FirstAddr;
  mutable std::bitset<PageSz> Space;

  Page(unsigned long long First) : FirstAddr(First) {
    Space = 0;
  };

  unsigned size() const { return PageSz; }

  std::vector<bool> getPageBits(unsigned long long Begin,
                                unsigned long long End) const {
    if ((Begin < FirstAddr) || (End > FirstAddr + PageSz))
      failWithError("Loading addresses not belonging to this page");
    std::vector<bool> Result;
    Result.reserve(End - Begin);
    for (auto Idx = Begin - FirstAddr; Idx < End - FirstAddr; ++Idx)
      Result.push_back(Space[Idx]);
    return Result;
  }

  template <typename BitsIt>
  void setPageBits(unsigned long long Begin, unsigned long long End,
                   BitsIt It) const {
    if ((Begin < FirstAddr) || (End > FirstAddr + PageSz))
      failWithError("Storing addresses not belonging to this page");
    for (auto Idx = Begin - FirstAddr; Idx < End - FirstAddr; ++Idx, ++It)
      Space[Idx] = *It;
  }

  const std::bitset<PageSz> &getSpace() const { return Space; }

  void dump(std::ostream &Stream) const {
    Stream << "First address: " << FirstAddr << "\n";
    Stream << "Bits: \n";
    Stream << Space << "\n";
  }

  void print() const { dump(std::cout); }

  auto operator[](unsigned long long Idx) const {
    if ((Idx < FirstAddr) || (Idx > FirstAddr + PageSz))
      failWithError("Accessing addresses not belonging to this page");
    return Space[Idx - FirstAddr];
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

template <unsigned PageSz>
bool operator<(std::_Rb_tree_iterator<rvdash::Page<PageSz>> &Lhs,
               std::_Rb_tree_iterator<rvdash::Page<PageSz>> &Rhs) {
  return (*Lhs).FirstAddr < (*Rhs).FirstAddr;
}

template <unsigned PageSz>
std::ostream &operator<<(std::ostream &Stream, const Page<PageSz> &Pg) {
  Stream << Pg.getSpace();
  return Stream;
}

//-------------------------------------Memory--------------------------------------------

template <unsigned AddrSz, unsigned PageSz = /* 4 kB / 100 */ 320,
          unsigned long long SpaceSz = 1ull << (AddrSz - 8)>
class Memory {
  std::set<Page<PageSz>> Pages;

  std::vector<bool> getBits(unsigned long long Addr,
                            unsigned long long Size) const {
    const auto First = getFirstPage(Addr);
    std::vector<bool> Result;
    Result.reserve(Size);
    for (auto PageIt = First; Size > 0; PageIt++) {
      unsigned long long LoadBegin = Addr;
      unsigned long long UsedLoad = PageSz - (LoadBegin - (*PageIt).FirstAddr);
      unsigned long long LoadSz = UsedLoad < Size ? UsedLoad : Size;
      unsigned long long LoadEnd = LoadBegin + LoadSz;
      auto Bits = (*PageIt).getPageBits(LoadBegin, LoadEnd);
      std::copy(Bits.begin(), Bits.end(), std::back_inserter(Result));
      Addr += LoadSz;
      Size -= LoadSz;
    }
    return Result;
  }

  auto getFirstPage(unsigned long long Addr) const {
    unsigned long long StartAddr = Addr / PageSz * PageSz;
    return Pages.find(StartAddr);
  }

  void setBits(unsigned long long Addr, const std::vector<bool> &Bits) const {
    auto First = getFirstPage(Addr);
    unsigned long long Size = Bits.size();
    unsigned long long Curr = 0;
    for (auto PageIt = First; Size > 0; PageIt++) {
      unsigned long long StoreBegin = Addr;
      unsigned long long UsedStore =
          PageSz - (StoreBegin - (*PageIt).FirstAddr);
      unsigned long long StoreSz = UsedStore < Size ? UsedStore : Size;
      unsigned long long StoreEnd = StoreBegin + StoreSz;
      (*PageIt).setPageBits(StoreBegin, StoreEnd, Bits.begin() + Curr);
      Addr += StoreSz;
      Curr += StoreSz;
      Size -= StoreSz;
    }
  }

  void validate(unsigned long long Addr, unsigned long long Size) {
    if (Addr >= SpaceSz)
      failWithError("Address exceeds address space size");
    if (!isAllocated(Addr, Size))
      allocate(Addr, Size);
  }

  void allocate(unsigned long long Addr, unsigned long long Size) {
    unsigned long long StartAddr = Addr / PageSz * PageSz;
    Pages.insert(Page<PageSz>(StartAddr));

    unsigned long long Allocated = StartAddr + PageSz - Addr;
    if (Allocated < Size)
      allocate(StartAddr + PageSz, Size - Allocated);
  }

public:
  Memory() {};
  ~Memory() {};

  constexpr static unsigned short getAddrSpaceSz() { return AddrSz; }
  constexpr static unsigned short getRealSpaceSz() { return SpaceSz; }
  constexpr static unsigned short getPageSz() { return PageSz; }

  const std::set<Page<PageSz>> &getPages() const { return Pages; }

  template <typename RegisterType>
  void load(unsigned long long Addr, unsigned long long Size,
            RegisterType &Reg) {
    if (Size > Reg.size())
      failWithError("Size of load exceeds register size");
    validate(Addr, Size);
    auto Bits = getBits(Addr, Size);
    for (unsigned long long Idx = 0; Idx < Size; Idx++)
      Reg[Idx] = Bits[Idx];
  }

  std::vector<bool> load(unsigned long long Addr, unsigned long long Size) {
    validate(Addr, Size);
    return getBits(Addr, Size);
  }

  template <typename RegisterType>
  void store(unsigned long long Addr, unsigned long long Size,
             const RegisterType &Reg) {
    if (Size > Reg.size())
      failWithError("Size of store exceeds register size");
    validate(Addr, Size);
    std::vector<bool> Bits;
    Bits.reserve(Reg.size());
    for (unsigned long long Idx = 0; Idx < Size; ++Idx)
      Bits.push_back(Reg[Idx]);
    setBits(Addr, Bits);
  }

  void store(unsigned long long Addr, unsigned long long Size) {
    validate(Addr, Size);
    setBits(Addr, std::vector<bool>(Size, 1));
  }

  bool isAllocated(unsigned long long Addr, unsigned long long Sz) const {
    long long Size = Sz;
    for (auto Page = Pages.cbegin(); Page < Pages.cend() && Size > 0; ++Page) {
      auto First = (*Page).FirstAddr;
      if ((First <= Addr) && (Addr < (First + PageSz))) {
        Size -= (First + PageSz - Addr);
        Addr = First + PageSz;
      }
    }
    return Size <= 0;
  };

  void dump(std::ostream &Stream) const {
    Stream << "Memory (AddrSz = " << AddrSz << ", SpaceSz = " << SpaceSz
           << ", PageSz = " << PageSz << "):\n";
    Stream << "Count pages: " << Pages.size() << "\n\n";
    for (const auto &Pg : Pages) {
      Stream << "Page:\n";
      Pg.dump(Stream);
      Stream << "\n\n";
    }
  }

  void print() const { dump(std::cout); }
};

template <unsigned AddrSz, unsigned PageSz, unsigned long long SpaceSz>
std::ostream &operator<<(std::ostream &Stream,
                         const Memory<AddrSz, SpaceSz, PageSz> &Mem) {
  for (const auto &Pg : Mem.getPages())
    Stream << Pg;
  return Stream;
}

} // namespace rvdash

#endif // MEMORY_H

