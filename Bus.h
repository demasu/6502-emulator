#pragma once
#include <array>
#include <cstdint>

#include "GenericBus.h"
#include "6502.h"
#include "Cartridge.h"
#include "PPU.h"

class Bus : public GenericBus {
   public:
    Bus();
    ~Bus();

    // Devices on main bus
    cpu6502 cpu;
    PPU ppu;
    std::shared_ptr<Cartridge> cart;
    uint8_t cpuRam[2048];
    uint8_t controller[2];

    void cpuWrite(uint16_t addr, uint8_t data) override;
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false) override;

    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset() override;
    void clock() override;

   private:
    // Count of how many clocks have passed
    uint32_t nSystemClockCounter = 0;

    // Controller state
    uint8_t controller_state[2];

    // A simple form of Direct Memory Access to swiftly transfer data from CPU bus memory to
    // the OAM memory.
    uint8_t dma_page = 0x00;
    uint8_t dma_addr = 0x00;
    uint8_t dma_data = 0x00;

    // DMA transfers need to be timed accurately. It takes 512 cycles to transfer the 256 bytes,
    // however the CPU needs to be on an even clock cycle to do it.
    bool dma_dummy = true;

    // Flag to indicate that DMA transfer is happening since the CPU gets suspended during it
    bool dma_transfer = false;
};
