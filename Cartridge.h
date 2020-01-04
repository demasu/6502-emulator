#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "Mapper_000.h"

class Cartridge {
   public:
    Cartridge(const std::string &sFileName);
    ~Cartridge();

    bool ImageValid();

    enum MIRROR {
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI,
    } mirror = HORIZONTAL;

    // Communication with the main bus
    bool cpuRead(uint16_t addr, uint8_t &data);
    bool cpuWrite(uint16_t addr, uint8_t data);

    // Communication with the PPU bus
    bool ppuRead(uint16_t addr, uint8_t &data);
    bool ppuWrite(uint16_t addr, uint8_t data);

    void reset();

   private:
    bool bImageValid  = false;
    uint8_t nMapperID = 0;
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;

    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;

    std::shared_ptr<Mapper> pMapper;
};
