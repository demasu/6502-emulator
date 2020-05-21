#include "Mapper_000.h"
#include <iostream>

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) {}
Mapper_000::~Mapper_000() {}

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr) {
    // If program is 16kb
    //      CPU address bus             PRG ROM
    //      0x8000 -> 0xBFFF: Map       0x000 -> 0x3FFF
    //      0xC000 -> 0xFFFF: Mirror    0x000->0x3FFF
    //
    // If program is 32kb
    //      CPU address bus             PRG ROM
    //      0x8000 -> 0xFFFF: Map       0x0000 -> 0x7FFF
    auto hex = [](uint32_t n, uint8_t d) {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4) {
            s[i] = "0123456789ABCDEF"[n & 0xF];
        }

        return s;
    };
    //std::cerr << "In the mapper's cpuMapRead" << std::endl;
    //std::cerr << "Address is: [" << hex(addr, 4) << "]" << std::endl;
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        //std::cerr << "Address is between 0x8000 and 0xFFFF" << std::endl;
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        //std::cerr << "Mapped address is: [" << hex(mapped_addr, 4) << "]" << std::endl;
        return true;
    }

    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data) {
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr) {
    // No mapping required for the PPU
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        mapped_addr = addr;
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        if (nCHRBanks == 0) {
            mapped_addr = addr;
            return true;
        }
    }

    return false;
}

void Mapper_000::reset() {}
