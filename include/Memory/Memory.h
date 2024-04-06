#ifndef MEMORY_H
#define MEMORY_H

#include <bitset>
#include <climits>
#include <iostream>
#include <set>
#include <vector>

#include "Error.h"

namespace rvdash {

//------------------------------------Page-----------------------------------------------

/**
 * @brief struct Page - structure not used independently,
 *                      it is a component of virtual memory.
 */
template <unsigned PageSz> struct Page {
  unsigned long long FirstAddr;
  mutable std::bitset<PageSz> Space;

  Page(unsigned long long First) : FirstAddr(First) { Space = 0; };

  unsigned size() const { return PageSz; }
  const std::bitset<PageSz> &getSpace() const { return Space; }
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

  auto operator[](unsigned long long Idx) const {
    if ((Idx < FirstAddr) || (Idx > FirstAddr + PageSz))
      failWithError("Accessing addresses not belonging to this page");
    return Space[Idx - FirstAddr];
  }

  template <typename BitsIt>
  void setPageBits(unsigned long long Begin, unsigned long long End,
                   BitsIt It) const {
    if (Begin % CHAR_BIT != 0 || End % CHAR_BIT != 0)
      failWithError("Misaligned on byte store in page");
    if ((Begin < FirstAddr) || (End > FirstAddr + PageSz))
      failWithError("Storing addresses not belonging to this page");
    for (auto Idx = Begin - FirstAddr; Idx < End - FirstAddr; ++Idx, ++It)
      Space[Idx] = *It;
  }

  void dump(std::ostream &Stream) const {
    Stream << "First address: 0x" << std::hex << FirstAddr / CHAR_BIT
           << std::dec << "\n";
    Stream << "Bits: \n";
    Stream << Space << "\n";
  }

  void print() const { dump(std::cout); }
};

template <unsigned PageSz>
bool operator<(Page<PageSz> &Lhs, Page<PageSz> &Rhs) {
  return Lhs.FirstAddr < Rhs.FirstAddr;
}

template <unsigned PageSz>
bool operator<(const Page<PageSz> &Lhs, const Page<PageSz> &Rhs) {
  return Lhs.FirstAddr < Rhs.FirstAddr;
}

// This is done for iterators in the std::set in Memory
template <typename Pg>
concept HasFirstAddrPtr = requires(Pg P) {
  P->FirstAddr;
};

template <typename T>
requires HasFirstAddrPtr<T>
bool operator<(const T Lhs, const T Rhs) {
  return Lhs->FirstAddr < Rhs->FirstAddr;
}

template <unsigned PageSz>
std::ostream &operator<<(std::ostream &Stream, const Page<PageSz> &Pg) {
  Stream << Pg.getSpace();
  return Stream;
}

//-------------------------------------Memory--------------------------------------------

/**
 * @brief class Memory - class for representing virtual memory
 *                       consisting of pages allocated when memory is accessed.
 *                       Supports AddrSz-bit address space with byte addressing.
 *                       All memory is random access (for now).
 *
 *                       All public methods use an Addr and Size in bytes,
 *                       while private methods use bits.
 */
template <unsigned AddrSz, unsigned PageSz = 4096 * CHAR_BIT /* 4 KB */>
class Memory {

  unsigned long long RamStart /* bits */;
  unsigned long long RamSize /* bits */;
  std::set<Page<PageSz>> Pages;

public:
  Memory(unsigned long long RamStrt = 0 /* bytes */,
         unsigned long long RamSz = 1ull << 20 /* 1 MB */)
      : RamStart(RamStrt * CHAR_BIT), RamSize(RamSz * CHAR_BIT) {
    if ((RamStart + RamSize) >= (1ull << AddrSz))
      failWithError("RAM addresses exceeds addr space size " +
                    std::to_string(1ull << AddrSz));
  };

  constexpr static unsigned short getAddrSz() { return AddrSz; }
  constexpr static unsigned long long getPageSz() { return PageSz; }
  constexpr static unsigned long long getDefaultRamStart() { return 0; }
  constexpr static unsigned long long getDefaultRamSz() { return 1ull << 20; }
  const std::set<Page<PageSz>> &getPages() const { return Pages; }

  template <typename RegisterType>
  void load(unsigned long long Addr, unsigned long long Size,
            RegisterType &Reg) {
    if (Size > Reg.size())
      failWithError("Size of load exceeds register size");
    Addr *= CHAR_BIT;
    Size *= CHAR_BIT;
    validate(Addr, Size);
    auto Bits = getBits(Addr, Size);
    for (auto Idx = 0; Idx < Size; ++Idx)
      Reg[Idx] = Bits[Idx];
  }

  template <typename RegisterType>
  void load(unsigned long long Addr, unsigned long long Size, RegisterType &Reg,
            std::ostream &LogFile) {
    load(Addr, Size, Reg);
    logGet(Addr * CHAR_BIT, Size * CHAR_BIT, Reg, LogFile);
  }

  std::vector<bool> load(unsigned long long Addr, unsigned long long Size) {
    Addr *= CHAR_BIT;
    Size *= CHAR_BIT;
    validate(Addr, Size);
    return getBits(Addr, Size);
  }

  template <typename RegisterType>
  void store(unsigned long long Addr, unsigned long long Size,
             const RegisterType &Reg) {
    if (Size > Reg.size())
      failWithError("Size of store exceeds register size");
    Addr *= CHAR_BIT;
    Size *= CHAR_BIT;
    validate(Addr, Size);
    std::vector<bool> Bits;
    Bits.reserve(Reg.size());
    for (auto Idx = 0; Idx < Size; ++Idx)
      Bits.push_back(Reg[Idx]);
    setBits(Addr, Bits);
  }

  template <typename RegisterType>
  void store(unsigned long long Addr, unsigned long long Size,
             const RegisterType &Reg, std::ostream &LogFile) {
    store(Addr, Size, Reg);
    logChange(Addr * CHAR_BIT, Size * CHAR_BIT, Reg, LogFile);
  }

  void store(unsigned long long Addr, unsigned long long Size) {
    Addr *= CHAR_BIT;
    Size *= CHAR_BIT;
    validate(Addr, Size);
    std::vector<bool> Bits(Size, 1);
    setBits(Addr, Bits);
  }

  bool isAllocated(unsigned long long Addr, unsigned long long Sz) const {
    Addr *= CHAR_BIT;
    long long Size = Sz * CHAR_BIT;
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
    Stream << "Memory (AddrSz = " << AddrSz << ", PageSz = " << PageSz
           << "):\n";
    Stream << "Count pages: " << Pages.size() << "\n\n";
    for (const auto &Pg : Pages) {
      Stream << "Page:\n";
      Pg.dump(Stream);
      Stream << "\n\n";
    }
  }
  void print() const { dump(std::cout); }

private:
  std::vector<bool> getBits(unsigned long long Addr,
                            unsigned long long Size) const {
    const auto First = getFirstPage(Addr);
    std::vector<bool> Result;
    Result.reserve(Size);
    for (auto PageIt = First; Size > 0; PageIt++) {
      auto LoadBegin = Addr;
      auto UsedLoad = PageSz - (LoadBegin - (*PageIt).FirstAddr);
      auto LoadSz = UsedLoad < Size ? UsedLoad : Size;
      auto LoadEnd = LoadBegin + LoadSz;
      auto Bits = (*PageIt).getPageBits(LoadBegin, LoadEnd);
      std::copy(Bits.begin(), Bits.end(), std::back_inserter(Result));
      Addr += LoadSz;
      Size -= LoadSz;
    }
    return Result;
  }

  auto getFirstPage(unsigned long long Addr) const {
    auto StartAddr = Addr / PageSz * PageSz;
    return Pages.find(StartAddr);
  }

  void setBits(unsigned long long Addr, const std::vector<bool> &Bits) const {
    auto First = getFirstPage(Addr);
    int Size = Bits.size();
    auto Curr = 0;
    for (auto PageIt = First; Size > 0; PageIt++) {
      auto StoreBegin = Addr;
      auto UsedStore = PageSz - (StoreBegin - (*PageIt).FirstAddr);
      auto StoreSz = UsedStore < Size ? UsedStore : Size;
      auto StoreEnd = StoreBegin + StoreSz;
      (*PageIt).setPageBits(StoreBegin, StoreEnd, Bits.begin() + Curr);
      Addr += StoreSz;
      Curr += StoreSz;
      Size -= StoreSz;
    }
  }

  template <typename RegisterType>
  void printBits(unsigned long long Size, const RegisterType &Bits,
                 std::ostream &LogFile) const {
    LogFile << " 0x" << std::hex << Bits.to_ulong() << std::dec << "\n";
  }

  template <typename RegisterType>
  void logChange(unsigned long long Addr, unsigned long long Size,
                 const RegisterType &Bits, std::ostream &LogFile) const {
    LogFile << "Changed memory bytes [" << std::hex << "0x" << Addr / CHAR_BIT
            << ", "
            << "0x" << (Addr + Size) / CHAR_BIT << "] <-" << std::dec;
    printBits(Size, Bits, LogFile);
  }

  template <typename RegisterType>
  void logGet(unsigned long long Addr, unsigned long long Size,
              const RegisterType &Bits, std::ostream &LogFile) const {
    LogFile << "Read memory bytes [" << std::hex << "0x" << Addr / CHAR_BIT
            << ", "
            << "0x" << (Addr + Size) / CHAR_BIT << "] ->" << std::dec;
    printBits(Size, Bits, LogFile);
  }

  void validate(unsigned long long Addr, unsigned long long Size) {
    if (Addr >= (RamStart + RamSize) || Addr < RamStart)
      failWithError("Invalid memory access, address " + std::to_string(Addr) +
                    " not available. Avalable addresses: [" +
                    std::to_string(RamStart / CHAR_BIT) + ", " +
                    std::to_string((RamStart + RamSize) / CHAR_BIT) + "]");
    if (Addr >= (1ull << AddrSz))
      failWithError("Address exceeds addr space size " +
                    std::to_string(1ull << AddrSz));
    if (!isAllocated(Addr, Size))
      allocate(Addr, Size);
  }

  void allocate(unsigned long long Addr, unsigned long long Size) {
    auto StartAddr = Addr / PageSz * PageSz;
    Pages.insert(Page<PageSz>(StartAddr));

    auto Allocated = StartAddr + PageSz - Addr;
    if (Allocated < Size)
      allocate(StartAddr + PageSz, Size - Allocated);
  }
};

} // namespace rvdash
#endif // MEMORY_H
