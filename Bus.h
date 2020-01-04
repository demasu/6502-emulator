#pragma once
#include <array>
#include <cstdint>

#include "6502.h"
#include "Cartridge.h"
#include "PPU.h"

class Bus {
   public:
    Bus();
    ~Bus();

    // Devices on main bus
    cpu6502 cpu;
    PPU ppu;
    std::shared_ptr<Cartridge> cart;
    uint8_t cpuRam[2048];
    uint8_t controller[2];

    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset();
    void clock();

   private:
    // Count of how many clocks have passed
    uint32_t nSystemClockCounter = 0;

    // Controller state
    uint8_t controller_state[2];
};
