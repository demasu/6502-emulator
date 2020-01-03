#pragma once
#include <cstdint>
#include <memory>

#include "olcPixelGameEngine.h"

#include "Cartridge.h"

class PPU {
   public:
    PPU();
    ~PPU();

    // Debugging utilities
    olc::Sprite& GetScreen();
    olc::Sprite& GetNameTable(uint8_t i);
    olc::Sprite& GetPatternTable(uint8_t i);
    bool frame_complete = false;

    // Communications with the main bus
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    // Communications with PPU bus
    uint8_t ppuRead(uint16_t addr, bool reaOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

    // Interface
    void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();

   private:
    uint8_t tblName[2][1024];
    uint8_t tblPattern[2][4096];
    uint8_t tblPalette[32];

    olc::Pixel palScreen[0x40];
    olc::Sprite sprScreen          = olc::Sprite(256, 240);
    olc::Sprite sprNameTable[2]    = {olc::Sprite(256, 240), olc::Sprite(256, 240)};
    olc::Sprite sprPatternTable[2] = {olc::Sprite(128, 128), olc::Sprite(128, 128)};

    int16_t scanline = 0;
    int16_t cycle    = 0;

    // Cartridge
    std::shared_ptr<Cartridge> cart;
};