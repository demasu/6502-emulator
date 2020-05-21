#include <iostream>
#include <sstream>

#include "6502.h"
#include "Bus.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Demo_6502 : public olc::PixelGameEngine {
   public:
    Demo_6502() {
        sAppName = "6502 Demonstration";
    }

   private:
    Bus nes;
    std::shared_ptr<Cartridge> cart;
    bool bEmulationRun  = false;
    float fResidualTime = 0.0f;
    uint8_t nSelectedPalette = 0x00;
    std::map<uint16_t, std::string> mapAsm;

    std::string hex(uint32_t n, uint8_t d) {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4) {
            s[i] = "0123456789ABCDEF"[n & 0xF];
        }

        return s;
    };

    void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
        int nRamX = x, nRamY = y;
        for (int row = 0; row < nRows; row++) {
            std::string sOffset = "$" + hex(nAddr, 4) + ":";
            for (int col = 0; col < nColumns; col++) {
                sOffset += " " + hex(nes.cpuRead(nAddr, true), 2);
                nAddr += 1;
            }
            DrawString(nRamX, nRamY, sOffset);
            nRamY += 10;
        }
    }

    void DrawCpu(int x, int y) {
        std::string status = "STATUS: ";
        DrawString(x, y, status, olc::WHITE);
        DrawString(x + 64, y, "N", nes.cpu.status & cpu6502::N ? olc::GREEN : olc::RED);
        DrawString(x + 80, y, "V", nes.cpu.status & cpu6502::V ? olc::GREEN : olc::RED);
        DrawString(x + 96, y, "-", nes.cpu.status & cpu6502::U ? olc::GREEN : olc::RED);
        DrawString(x + 112, y, "B", nes.cpu.status & cpu6502::B ? olc::GREEN : olc::RED);
        DrawString(x + 128, y, "D", nes.cpu.status & cpu6502::D ? olc::GREEN : olc::RED);
        DrawString(x + 144, y, "I", nes.cpu.status & cpu6502::I ? olc::GREEN : olc::RED);
        DrawString(x + 160, y, "Z", nes.cpu.status & cpu6502::Z ? olc::GREEN : olc::RED);
        DrawString(x + 178, y, "C", nes.cpu.status & cpu6502::C ? olc::GREEN : olc::RED);

        DrawString(x, y + 10, "PC:            $" + hex(nes.cpu.pc, 4));
        DrawString(x, y + 20, "A:             $" + hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
        DrawString(x, y + 30, "X:             $" + hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
        DrawString(x, y + 40, "Y:             $" + hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
        DrawString(x, y + 50, "Stack Pointer: $" + hex(nes.cpu.sptr, 4));
    }

    void DrawCode(int x, int y, int nLines) {
        auto it_a  = mapAsm.find(nes.cpu.pc);
        int nLineY = (nLines >> 1) * 10 + y;
        if (it_a != mapAsm.end()) {
            DrawString(x, nLineY, (*it_a).second, olc::CYAN);
            while (nLineY < (nLines * 10) + y) {
                nLineY += 10;
                if (++it_a != mapAsm.end()) {
                    DrawString(x, nLineY, (*it_a).second);
                }
            }
        }

        it_a   = mapAsm.find(nes.cpu.pc);
        //std::cerr << "it_a.first is: [" << std::to_string(it_a->first) << "]" << std::endl;
        nLineY = (nLines >> 1) * 10 + y;
        //std::cerr << "Checking if it_a is at the end" << std::endl;
        if (it_a != mapAsm.end() && it_a != mapAsm.begin()) {
            //std::cerr << "It is not" << std::endl;
            //std::cerr << "Is nLineY greater than y?" << std::endl;
            while (nLineY > y) {
                //std::cerr << "It is" << std::endl;
                nLineY -= 10;
                //std::cerr << "Decrementing it_a and checking if it is at the end" << std::endl;
                if (--it_a != mapAsm.end()) {
                    //std::cerr << "It is not" << std::endl;
                    //std::cerr << "it_a.first is: [" << std::to_string(it_a->first) << "]" << std::endl;
                    DrawString(x, nLineY, (*it_a).second);
                }
            }
        }
    }

    bool OnUserCreate() {
        // Load the cartridge
        cart = std::make_shared<Cartridge>("nestest.nes");
        //cart = std::make_shared<Cartridge>("wat.nes");
        // cart = std::make_shared<Cartridge>("test_rom2.nes");
        //cart = std::make_shared<Cartridge>("chr_rom_test.nes");
        //cart = std::make_shared<Cartridge>("test_nesasm.nes");
        //cart = std::make_shared<Cartridge>("test_rom_nesasm.nes");
        //cart = std::make_shared<Cartridge>("test_rom.bin");
        if (!cart->ImageValid()) {
            return false;
        }

        // Insert into the NES
        nes.insertCartridge(cart);

        auto hex = [](uint32_t n, uint8_t d) {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4) {
                s[i] = "0123456789ABCDEF"[n & 0xF];
            }

            return s;
        };
        std::cerr << "Value at 0xFFFC is: [" << hex(nes.cpuRead(0xFFFC, true), 2) << "]" << std::endl;
        std::cerr << "Value at 0xFFFD is: [" << hex(nes.cpuRead(0xFFFD, true), 2) << "]" << std::endl;
        //return false;

        // Extract disassembly
        mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

        // Reset
        nes.cpu.reset();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) {
        Clear(olc::DARK_BLUE);
        nes.controller[0] = 0x00;
        nes.controller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;     // A Button
        nes.controller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;     // B Button
        nes.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00;     // Select
        nes.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00;     // Start
        nes.controller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
        nes.controller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;

        if (bEmulationRun) {
            if (fResidualTime > 0.0f) {
                fResidualTime -= fElapsedTime;
            }
            else {
                fResidualTime += (1.0f / 60.0f) - fElapsedTime;
                do {
                    nes.clock();
                } while (!nes.ppu.frame_complete);
                nes.ppu.frame_complete = false;
            }
        }
        else {
            // One clock at a time
            if (GetKey(olc::Key::C).bPressed) {
                do {
                    nes.clock();
                } while (!nes.cpu.complete());

                // Since the CPU runs slower, there may be more complete cycles to use up
                do {
                    nes.clock();
                } while (nes.cpu.complete());
            }

            // Emulate one entire frame
            if (GetKey(olc::Key::F).bPressed) {
                // Clock enough for an entire frame
                do {
                    nes.clock();
                } while (!nes.ppu.frame_complete);

                // Use residual clock cycles to complete the current instruction
                do {
                    nes.clock();
                } while (nes.cpu.complete());

                // Reset frame completion flag
                nes.ppu.frame_complete = false;
            }

            if (GetKey(olc::Key::U).bPressed) {
                do {
                    nes.clock();
                } while (nes.ppu.GetScanLine() <= 240);

                do {
                    nes.clock();
                } while (nes.cpu.complete());
            }

            if (GetKey(olc::Key::Y).bPressed) {
                int newScanLine = nes.ppu.GetScanLine() + 1;
                std::cerr << "newScanLine is: [" << std::to_string(newScanLine) << "]" << std::endl;
                if ( newScanLine <= 260 ) {
                    std::cerr << "Clocking until new scanline" << std::endl;
                    do {
                        nes.clock();
                    } while (nes.ppu.GetScanLine() < newScanLine);

                    std::cerr << "Clocking to finish out unused clock cycles" << std::endl;
                    do {
                        nes.clock();
                    } while (nes.cpu.complete());
                }
            }

            if ( GetKey(olc::Key::B).bPressed) {
                nes.cpu.pc = 0xFB78;
            }

            if ( GetKey(olc::Key::E).bPressed) {
                std::cerr << "Program counter is at: " << hex(nes.cpu.pc, 4) << std::endl;
                std::cerr << "Data at PC location is: " << hex(nes.cpuRead(nes.cpu.pc, true), 2) << std::endl;
            }
        }

        if (GetKey(olc::Key::SPACE).bPressed) {
            bEmulationRun = !bEmulationRun;
        }

        if (GetKey(olc::Key::R).bPressed) {
            nes.reset();
        }

        if (GetKey(olc::Key::P).bPressed) {
            (++nSelectedPalette) &= 0x07;
        }

        // Draw Ram Page 0x00
        DrawCpu(516, 2);
        DrawCode(516, 72, 26);
        DrawString(516, 402, "Scanline: " + std::to_string(nes.ppu.GetScanLine()));
        DrawString(516, 412, "Cycle: " + std::to_string(nes.ppu.GetCycle()));
        //DrawRam(2, 2, 0x0000, 16, 16);
        //DrawRam(2, 182, 0x8000, 16, 16);

        //// Draw OAM contents
        //for ( int i = 0; i < 26; i++ ) {
        //    std::string s = hex(i, 2) + ": (" + std::to_string(nes.ppu.pOAM[i * 4 + 3]) + ", " + std::to_string(nes.ppu.pOAM[i * 4 + 0]) + ") " + "ID: " + hex(nes.ppu.pOAM[i * 4 + 1], 2) + " AT: " + hex(nes.ppu.pOAM[1 * 4 + 2], 2);
        //    DrawString(516, 72 + i * 10, s);
        //}

        //// Draw palettes and pattern tables
        //const int nSwatchSize = 6;
        //for ( int p = 0; p < 8; p++ ) {
        //    for ( int s = 0; s < 4; s++ ) {
        //        FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340, nSwatchSize, nSwatchSize, nes.ppu.GetColourFromPaletteRam(p, s));
        //    }
        //}

        //// Draw selection reticule around selected palette
        //DrawRect(516 + nSelectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);

        //// Generate pattern tables
        //DrawSprite(516, 348, &nes.ppu.GetPatternTable(0, nSelectedPalette));
        //DrawSprite(648, 348, &nes.ppu.GetPatternTable(1, nSelectedPalette));

        DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);

        return true;
    }
};

int main() {
    Demo_6502 demo;
    demo.Construct(780, 480, 2, 2);
    demo.Start();

    return 0;
}
