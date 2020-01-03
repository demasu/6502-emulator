#pragma once
#include <cstdint>

class Mapper {
   public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper();

    // Transform CPU bus address into PRG ROM offset
    virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr)  = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

    // Transform PPU bus address into CHR ROM offset
    virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr)  = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

   protected:
    // A lot of mappers need this information, so they're stored here
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
};