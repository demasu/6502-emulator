#include "PPU.h"

PPU::PPU() {
    palScreen[0x00] = olc::Pixel(84, 84, 84);
    palScreen[0x01] = olc::Pixel(0, 30, 116);
    palScreen[0x02] = olc::Pixel(8, 16, 144);
    palScreen[0x03] = olc::Pixel(48, 0, 136);
    palScreen[0x04] = olc::Pixel(68, 0, 100);
    palScreen[0x05] = olc::Pixel(92, 0, 48);
    palScreen[0x06] = olc::Pixel(84, 4, 0);
    palScreen[0x07] = olc::Pixel(60, 24, 0);
    palScreen[0x08] = olc::Pixel(32, 42, 0);
    palScreen[0x09] = olc::Pixel(8, 58, 0);
    palScreen[0x0A] = olc::Pixel(0, 64, 0);
    palScreen[0x0B] = olc::Pixel(0, 60, 0);
    palScreen[0x0C] = olc::Pixel(0, 50, 60);
    palScreen[0x0D] = olc::Pixel(0, 0, 0);
    palScreen[0x0E] = olc::Pixel(0, 0, 0);
    palScreen[0x0F] = olc::Pixel(0, 0, 0);

    palScreen[0x10] = olc::Pixel(152, 150, 152);
    palScreen[0x11] = olc::Pixel(8, 76, 196);
    palScreen[0x12] = olc::Pixel(48, 50, 236);
    palScreen[0x13] = olc::Pixel(92, 30, 228);
    palScreen[0x14] = olc::Pixel(136, 20, 176);
    palScreen[0x15] = olc::Pixel(160, 20, 100);
    palScreen[0x16] = olc::Pixel(152, 34, 32);
    palScreen[0x17] = olc::Pixel(120, 60, 0);
    palScreen[0x18] = olc::Pixel(84, 90, 0);
    palScreen[0x19] = olc::Pixel(40, 114, 0);
    palScreen[0x1A] = olc::Pixel(8, 124, 0);
    palScreen[0x1B] = olc::Pixel(0, 118, 40);
    palScreen[0x1C] = olc::Pixel(0, 102, 120);
    palScreen[0x1D] = olc::Pixel(0, 0, 0);
    palScreen[0x1E] = olc::Pixel(0, 0, 0);
    palScreen[0x1F] = olc::Pixel(0, 0, 0);

    palScreen[0x20] = olc::Pixel(236, 238, 236);
    palScreen[0x21] = olc::Pixel(76, 154, 236);
    palScreen[0x22] = olc::Pixel(120, 124, 236);
    palScreen[0x23] = olc::Pixel(176, 98, 236);
    palScreen[0x24] = olc::Pixel(228, 84, 236);
    palScreen[0x25] = olc::Pixel(236, 88, 180);
    palScreen[0x26] = olc::Pixel(236, 106, 100);
    palScreen[0x27] = olc::Pixel(212, 136, 32);
    palScreen[0x28] = olc::Pixel(160, 170, 0);
    palScreen[0x29] = olc::Pixel(116, 196, 0);
    palScreen[0x2A] = olc::Pixel(76, 208, 32);
    palScreen[0x2B] = olc::Pixel(56, 204, 108);
    palScreen[0x2C] = olc::Pixel(56, 180, 204);
    palScreen[0x2D] = olc::Pixel(60, 60, 60);
    palScreen[0x2E] = olc::Pixel(0, 0, 0);
    palScreen[0x2F] = olc::Pixel(0, 0, 0);

    palScreen[0x30] = olc::Pixel(236, 238, 236);
    palScreen[0x31] = olc::Pixel(168, 204, 236);
    palScreen[0x32] = olc::Pixel(188, 188, 236);
    palScreen[0x33] = olc::Pixel(212, 178, 236);
    palScreen[0x34] = olc::Pixel(236, 174, 236);
    palScreen[0x35] = olc::Pixel(236, 174, 212);
    palScreen[0x36] = olc::Pixel(236, 180, 176);
    palScreen[0x37] = olc::Pixel(228, 196, 144);
    palScreen[0x38] = olc::Pixel(204, 210, 120);
    palScreen[0x39] = olc::Pixel(180, 222, 120);
    palScreen[0x3A] = olc::Pixel(168, 226, 144);
    palScreen[0x3B] = olc::Pixel(152, 226, 180);
    palScreen[0x3C] = olc::Pixel(160, 214, 228);
    palScreen[0x3D] = olc::Pixel(160, 162, 160);
    palScreen[0x3E] = olc::Pixel(0, 0, 0);
    palScreen[0x3F] = olc::Pixel(0, 0, 0);
}

PPU::~PPU() {}

olc::Sprite& PPU::GetScreen() {
    return sprScreen;
}

olc::Sprite& PPU::GetPatternTable(uint8_t i, uint8_t palette) {
    // This function draws the CHR ROM for a given pattern table into an olc::Sprite using
    // whatever palette we specified.

    // Loop through all 16x16 tiles
    for (uint16_t nTileY = 0; nTileY < 16; nTileY++) {
        for (uint16_t nTileX = 0; nTileX < 16; nTileX++) {
            // Convert 2D coordinate into 1D offset
            uint16_t nOffset = nTileY * 256 + nTileX * 16;

            // Loop through the 8 rows of 8 pixels
            for (uint16_t row = 0; row < 8; row++) {
                // For each row we need to read both bit planes in order to get the data we need
                uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0x0000);
                uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 0x0008);

                // Now we can get the rest that we need
                for (uint16_t col = 0; col < 8; col++) {
                    uint8_t pixel = (tile_lsb & 0x01) << 1 | (tile_msb & 0x01);
                    tile_lsb >>= 1;
                    tile_msb >>= 1;

                    sprPatternTable[i].SetPixel(nTileX * 8 + (7 - col), nTileY * 8 + row, GetColourFromPaletteRam(palette, pixel));
                }
            }
        }
    }

    return sprPatternTable[i];
}

olc::Pixel& PPU::GetColourFromPaletteRam(uint8_t palette, uint8_t pixel) {
    // Convenience function to get an appropriate color from the palette and pixel index
    // 0x3F00       - Offset into PPU addressable range
    // palette << 2 - Each palette is 4 bytes in size
    // pixel        - Each pixel index is either 0, 1, 2, or 3
    // & 0x3F       - Keeps us in bounds
    return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

olc::Sprite& PPU::GetNameTable(uint8_t i) {
    return sprNameTable[i];
}

uint8_t PPU::cpuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;

    if (readOnly) {
        // Reading from the PPU registers can change their state
        // So we only use this in debugging
        switch (addr) {
            case 0x0000:  // Control
                data = control.reg;
                break;
            case 0x0001:  // Mask
                data = mask.reg;
                break;
            case 0x0002:  // Status
                data = status.reg;
                break;
            case 0x0003:  // OAM address
                break;
            case 0x0004:  // OAM Data
                break;
            case 0x0005:  // Scroll
                break;
            case 0x0006:  // PPU Address
                break;
            case 0x0007:  // PPU Data
                break;
        }
    }
    else {
        switch (addr) {
            case 0x0000:  // Control
                break;
            case 0x0001:  // Mask
                break;
            case 0x0002:  // Status
                data                  = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
                status.vertical_blank = 0;
                address_latch         = 0;
                break;
            case 0x0003:  // OAM address
                break;
            case 0x0004:  // OAM Data
                data = pOAM[oam_addr];
                break;
            case 0x0005:  // Scroll
                break;
            case 0x0006:  // PPU Address
                break;
            case 0x0007:  // PPU Data
                data            = ppu_data_buffer;
                ppu_data_buffer = ppuRead(vram_addr.reg);
                if (vram_addr.reg >= 0x3F00) {
                    data = ppu_data_buffer;
                }
                vram_addr.reg += (control.increment_mode ? 32 : 1);
                break;
        }
    }

    return data;
}

void PPU::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
        case 0x0000:  // Control
            control.reg           = data;
            tram_addr.nametable_x = control.nametable_x;
            tram_addr.nametable_y = control.nametable_y;
            break;
        case 0x0001:  // Mask
            mask.reg = data;
            break;
        case 0x0002:  // Status
            break;
        case 0x0003:  // OAM address
            oam_addr = data;
            break;
        case 0x0004:  // OAM Data
            pOAM[oam_addr] = data;
            break;
        case 0x0005:  // Scroll
            if (address_latch == 0) {
                fine_x             = data & 0x07;
                tram_addr.coarse_x = data >> 3;
                address_latch      = 1;
            }
            else {
                tram_addr.fine_y   = data & 0x07;
                tram_addr.coarse_y = data >> 3;
                address_latch      = 0;
            }
            break;
        case 0x0006:  // PPU Address
            if (address_latch == 0) {
                tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
                address_latch = 1;
            }
            else {
                tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
                vram_addr     = tram_addr;
                address_latch = 0;
            }
            break;
        case 0x0007:  // PPU Data
            ppuWrite(vram_addr.reg, data);
            vram_addr.reg += (control.increment_mode ? 32 : 1);
            break;
    }
}

uint8_t PPU::ppuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    if (cart->ppuRead(addr, data)) {
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // If cartridge can't map it, have a location ready here
        data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF) {
        addr &= 0x0FFF;

        if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) {
                data = tblName[0][addr & 0x03FF];
            }
            if (addr >= 0x0400 && addr <= 0x07FF) {
                data = tblName[1][addr & 0x03FF];
            }
            if (addr >= 0x0800 && addr <= 0x0BFF) {
                data = tblName[0][addr & 0x03FF];
            }
            if (addr >= 0x0C00 && addr <= 0x0FFF) {
                data = tblName[1][addr & 0x03FF];
            }
        }
        else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) {
                data = tblName[0][addr & 0x03FF];
            }
            if (addr >= 0x0400 && addr <= 0x07FF) {
                data = tblName[0][addr & 0x03FF];
            }
            if (addr >= 0x0800 && addr <= 0x0BFF) {
                data = tblName[1][addr & 0x03FF];
            }
            if (addr >= 0x0C00 && addr <= 0x0FFF) {
                data = tblName[1][addr & 0x03FF];
            }
        }
    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F;
        if (addr == 0x0010) {
            addr = 0x0000;
        }
        if (addr == 0x0014) {
            addr = 0x0004;
        }
        if (addr == 0x0018) {
            addr = 0x0008;
        }
        if (addr == 0x001C) {
            addr = 0x000C;
        }
        data = tblPalette[addr] & (mask.grayscale ? 0x30 : 0x3F);
    }

    return data;
}

void PPU::ppuWrite(uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;

    if (cart->ppuWrite(addr, data)) {
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF) {
        addr &= 0x0FFF;
        if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) {
                tblName[0][addr & 0x03FF] = data;
            }
            if (addr >= 0x0400 && addr <= 0x07FF) {
                tblName[1][addr & 0x03FF] = data;
            }
            if (addr >= 0x0800 && addr <= 0x0BFF) {
                tblName[0][addr & 0x03FF] = data;
            }
            if (addr >= 0x0C00 && addr <= 0x0FFF) {
                tblName[1][addr & 0x03FF] = data;
            }
        }
        else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) {
                tblName[0][addr & 0x03FF] = data;
            }
            if (addr >= 0x0400 && addr <= 0x07FF) {
                tblName[0][addr & 0x03FF] = data;
            }
            if (addr >= 0x0800 && addr <= 0x0BFF) {
                tblName[1][addr & 0x03FF] = data;
            }
            if (addr >= 0x0C00 && addr <= 0x0FFF) {
                tblName[1][addr & 0x03FF] = data;
            }
        }
    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F;
        if (addr == 0x0010) {
            addr = 0x0000;
        }
        if (addr == 0x0014) {
            addr = 0x0004;
        }
        if (addr == 0x0018) {
            addr = 0x0008;
        }
        if (addr == 0x001C) {
            addr = 0x000C;
        }
        tblPalette[addr] = data;
    }
}

void PPU::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cart = cartridge;
}

void PPU::clock() {
    auto IncrementScrollX = [&]() {
        if (mask.render_background || mask.render_sprites) {
            if (vram_addr.coarse_x == 31) {
                vram_addr.coarse_x    = 0;
                vram_addr.nametable_x = ~vram_addr.nametable_x;
            }
            else {
                vram_addr.coarse_x++;
            }
        }
    };

    auto IncrementScrollY = [&]() {
        if (mask.render_background || mask.render_sprites) {
            if (vram_addr.fine_y < 7) {
                vram_addr.fine_y++;
            }
            else {
                vram_addr.fine_y = 0;

                if (vram_addr.coarse_y == 29) {
                    vram_addr.coarse_y    = 0;
                    vram_addr.nametable_y = ~vram_addr.nametable_y;
                }
                else if (vram_addr.coarse_y == 31) {
                    vram_addr.coarse_y = 0;
                }
                else {
                    vram_addr.coarse_y++;
                }
            }
        }
    };

    auto TransferAddressX = [&]() {
        if (mask.render_background || mask.render_sprites) {
            vram_addr.nametable_x = tram_addr.nametable_x;
            vram_addr.coarse_x    = tram_addr.coarse_x;
        }
    };

    auto TransferAddressY = [&]() {
        if (mask.render_background || mask.render_sprites) {
            vram_addr.fine_y      = tram_addr.fine_y;
            vram_addr.nametable_y = tram_addr.nametable_y;
            vram_addr.coarse_y    = tram_addr.coarse_y;
        }
    };

    auto LoadBackgroundShifters = [&]() {
        bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
        bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

        bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
        bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
    };

    auto UpdateShifters = [&]() {
        if (mask.render_background) {
            bg_shifter_pattern_lo <<= 1;
            bg_shifter_pattern_hi <<= 1;

            bg_shifter_attrib_lo <<= 1;
            bg_shifter_attrib_hi <<= 1;
        }

        if (mask.render_sprites && cycle >= 1 && cycle < 258) {
            for (int i = 0; i < sprite_count; i++) {
                if (spriteScanline[i].x > 0) {
                    spriteScanline[i].x--;
                }
                else {
                    sprite_shifter_pattern_lo[i] <<= 1;
                    sprite_shifter_pattern_hi[i] <<= 1;
                }
            }
        }
    };

    if (scanline >= -1 && scanline < 240) {
        // Background rendering
        if (scanline == 0 && cycle == 0) {
            cycle = 1;
        }
        if (scanline == -1 && cycle == 1) {
            // Effectively the start of a new frame, so clear some flags
            status.vertical_blank  = 0;
            status.sprite_overflow = 0;
            status.sprite_zero_hit = 0;

            // Clear shifters
            for (int i = 0; i < 8; i++) {
                sprite_shifter_pattern_lo[i] = 0;
                sprite_shifter_pattern_hi[i] = 0;
            }
        }

        if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {
            UpdateShifters();

            switch ((cycle - 1) % 8) {
                case 0:
                    LoadBackgroundShifters();
                    bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
                    break;
                case 2:
                    bg_next_tile_attrib = ppuRead(0x23C0 | (vram_addr.nametable_y << 11) | (vram_addr.nametable_x << 10) | ((vram_addr.coarse_y >> 2) << 3) | (vram_addr.coarse_x >> 2));
                    if (vram_addr.coarse_y & 0x02) {
                        bg_next_tile_attrib >>= 4;
                    }
                    if (vram_addr.coarse_x & 0x02) {
                        bg_next_tile_attrib >>= 2;
                    }
                    bg_next_tile_attrib &= 0x03;
                    break;
                case 4:
                    bg_next_tile_lsb = ppuRead((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + (vram_addr.fine_y) + 0);
                    break;
                case 6:
                    bg_next_tile_msb = ppuRead((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + (vram_addr.fine_y) + 8);
                    break;
                case 7:
                    IncrementScrollX();
                    break;
            }
        }

        if (cycle == 256) {
            IncrementScrollY();
        }

        if (cycle == 257) {
            LoadBackgroundShifters();
            TransferAddressX();
        }

        if (cycle == 338 || cycle == 340) {
            bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
        }

        if (scanline == -1 && cycle >= 280 && cycle < 305) {
            TransferAddressY();
        }

        // Foreground rendering
        //TODO: See if you can update this to be more accurate to the actual NES
        if (cycle == 257 && scanline >= 0) {
            // End of visible scanline, so we need to determine the sprites for the next scanline

            // Clear the sprite memory so we can get new ones to render
            std::memset(spriteScanline, 0xFF, 8 * sizeof(sObjectAttributeEntry));

            // Store the number of sprites we've seen. NES can only display 8 per line max
            sprite_count = 0;

            // Clear out residual information in shifters
            for (uint8_t i = 0; i < 8; i++) {
                sprite_shifter_pattern_lo[i] = 0;
                sprite_shifter_pattern_hi[i] = 0;
            }

            // Evaluate which sprites are visible. Need to iterate through the OAM until we've found 8 sprites that
            // are on the scanline. We count to 9 so we can set the sprite overflow flag
            uint8_t nOAMEntry = 0;

            // New set of sprites. Sprite zero may not be in the new set so clear the flag
            bSpriteZeroHitPossible = false;

            while (nOAMEntry < 64 && sprite_count < 9) {
                int16_t diff = ((int16_t)scanline - (int16_t)OAM[nOAMEntry].y);

                // If the difference is positive, then the scanline is at least the same height as the sprite
                // so we need to check if it resides in the sprite vertically depending on the sprite height mode

                if (diff >= 0 && diff < (control.sprite_size ? 16 : 8)) {
                    // Sprite is visible, so copy the attribute entry over to the sprite cache
                    if (sprite_count < 8) {
                        // Set the flag for a possible sprite zero hit if the sprite is sprite zero
                        if (nOAMEntry == 0) {
                            bSpriteZeroHitPossible = true;
                        }

                        memcpy(&spriteScanline[sprite_count], &OAM[nOAMEntry], sizeof(sObjectAttributeEntry));
                        sprite_count++;
                    }
                }

                nOAMEntry++;
            }

            status.sprite_overflow = (sprite_count > 8);
        }

        if (cycle == 340) {
            // We're at the very end of the scanline. Prepare the sprite shifters with the selected sprites
            for (uint8_t i = 0; i < sprite_count; i++) {
                // Need to extract the 8-bit row patterns of the sprite with the correct vertical offset.
                uint8_t sprite_pattern_bits_lo;
                uint8_t sprite_pattern_bits_hi;
                uint16_t sprite_pattern_addr_lo;
                uint16_t sprite_pattern_addr_hi;

                // Determine the memory addresses that contain the byte of pattern data.
                if (!control.sprite_size) {
                    // 8x8 sprite mode
                    if (!(spriteScanline[i].attribute & 0x80)) {
                        // Sprite is normal
                        // Patterntable - cell - row
                        sprite_pattern_addr_lo = (control.pattern_sprite << 12) | (spriteScanline[i].id << 4) | (scanline - spriteScanline[i].y);
                    }
                    else {
                        // Sprite is upside down
                        sprite_pattern_addr_lo = (control.pattern_sprite << 12) | (spriteScanline[i].id << 4) | (7 - (scanline - spriteScanline[i].y));
                    }
                }
                else {
                    // 8x16 sprite mode
                    if (!(spriteScanline[i].attribute & 0x80)) {
                        // Sprite is normal
                        if (scanline - spriteScanline[i].y < 8) {
                            // Reading the top half of the tile
                            sprite_pattern_addr_lo = ((spriteScanline[i].id & 0x01) << 12) | ((spriteScanline[i].id & 0xFE) << 4) | ((scanline - spriteScanline[i].y) & 0x07);
                        }
                        else {
                            // Reading the bottom half of the tile
                            sprite_pattern_addr_lo = ((spriteScanline[i].id & 0x01) << 12) | (((spriteScanline[i].id & 0xFE) + 1) << 4) | ((scanline - spriteScanline[i].y) & 0x07);
                        }
                    }
                    else {
                        // Sprite is upside down
                        if (scanline - spriteScanline[i].y < 8) {
                            // Reading the top half of the tile
                            sprite_pattern_addr_lo = ((spriteScanline[i].id & 0x01) << 12) | (((spriteScanline[i].id & 0xFE) + 1) << 4) | (7 - (scanline - spriteScanline[i].y) & 0x07);
                        }
                        else {
                            sprite_pattern_addr_lo = ((spriteScanline[i].id & 0x01) << 12) | ((spriteScanline[i].id & 0xFE) << 4) | (7 - (scanline - spriteScanline[i].y) & 0x07);
                        }
                    }
                }

                // Hi bit plane is always offset by 8 bytes from lo bit plane
                sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;

                // Now we can read the sprites
                sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo);
                sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi);

                // If the sprite is flipped horizontally (right becomes left) we need to flip the pattern bites
                if (spriteScanline[i].attribute & 0x40) {
                    // Lambda function taken from https://stackoverflow.com/a/2602885
                    auto flipbyte = [](uint8_t b) {
                        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
                        b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
                        b = (b & 0xAA) >> 1 | (b & 0x55) << 1;

                        return b;
                    };

                    // Flip patterns
                    sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
                    sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
                }

                // Load pattern into sprite shift registers
                sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
                sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
            }
        }
    }

    if (scanline == 240) {
    }

    if (scanline >= 241 && scanline < 261) {
        if (scanline == 241 && cycle == 1) {
            status.vertical_blank = 1;
            if (control.enable_nmi) {
                nmi = true;
            }
        }
    }

    // Background
    uint8_t bg_pixel   = 0x00;
    uint8_t bg_palette = 0x00;

    if (mask.render_background) {
        uint16_t bit_mux = 0x8000 >> fine_x;
        uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
        uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;

        bg_pixel = (p1_pixel << 1) | p0_pixel;

        uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
        uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
        bg_palette      = (bg_pal1 << 1) | bg_pal0;
    }

    // Foreground
    uint8_t fg_pixel    = 0x00;
    uint8_t fg_palette  = 0x00;
    uint8_t fg_priority = 0x00;

    if (mask.render_sprites) {
        bSpriteZeroBeingRendered = false;
        for (uint8_t i = 0; i < sprite_count; i++) {
            // Scanline has "hit" the sprite
            if (spriteScanline[i].x == 0) {
                // Get the pixel value
                uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
                uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
                fg_pixel            = (fg_pixel_hi << 1) | fg_pixel_lo;

                // Get the bottom two bits from the palette
                fg_palette  = (spriteScanline[i].attribute & 0x03) + 0x04;
                fg_priority = (spriteScanline[i].attribute & 0x20) == 0;

                // If pixel is not transparent we render it
                if (fg_pixel != 0) {
                    if (i == 0) {
                        // This is sprite zero
                        bSpriteZeroBeingRendered = true;
                    }

                    break;
                }
            }
        }
    }

    // Combine the background and foreground pixels. Sprites can go behind non-transparent background tiles
    uint8_t pixel   = 0x00;
    uint8_t palette = 0x00;

    if (bg_pixel == 0 && fg_pixel == 0) {
        // Both pixels are transparent, draw the background color
        pixel   = 0x00;
        palette = 0x00;
    }
    else if (bg_pixel == 0 && fg_pixel > 0) {
        // Background is transparent, forground is visible
        pixel   = fg_pixel;
        palette = fg_palette;
    }
    else if (bg_pixel > 0 && fg_pixel == 0) {
        // Background is visible, foreground is transparent
        pixel   = bg_pixel;
        palette = bg_palette;
    }
    else if (bg_pixel > 0 && fg_pixel > 0) {
        // Both are visible
        if (fg_priority) {
            // Foreground has priority
            pixel   = fg_pixel;
            palette = fg_palette;
        }
        else {
            // Background has priority
            pixel   = bg_pixel;
            palette = bg_palette;
        }

        // Sprite zero hit detection
        if (bSpriteZeroHitPossible && bSpriteZeroBeingRendered) {
            if (mask.render_background & mask.render_sprites) {
                if (~(mask.render_background_left | mask.render_sprites_left)) {
                    if (cycle >= 9 && cycle < 258) {
                        status.sprite_zero_hit = 1;
                    }
                }
                else {
                    if (cycle >= 1 && cycle < 258) {
                        status.sprite_zero_hit = 1;
                    }
                }
            }
        }
    }

    sprScreen.SetPixel(cycle - 1, scanline, GetColourFromPaletteRam(palette, pixel));

    cycle++;
    if (cycle >= 341) {
        cycle = 0;
        scanline++;
        if (scanline >= 261) {
            scanline       = -1;
            frame_complete = true;
        }
    }
}

void PPU::reset() {
    fine_x                = 0x00;
    address_latch         = 0x00;
    ppu_data_buffer       = 0x00;
    scanline              = 0;
    cycle                 = 0;
    bg_next_tile_id       = 0x00;
    bg_next_tile_attrib   = 0x00;
    bg_next_tile_lsb      = 0x00;
    bg_next_tile_msb      = 0x00;
    bg_shifter_pattern_lo = 0x0000;
    bg_shifter_pattern_hi = 0x0000;
    bg_shifter_attrib_lo  = 0x0000;
    bg_shifter_attrib_hi  = 0x0000;
    status.reg            = 0x00;
    mask.reg              = 0x00;
    control.reg           = 0x00;
    vram_addr.reg         = 0x0000;
    tram_addr.reg         = 0x0000;
}

int PPU::GetScanLine() {
    return scanline;
}

int PPU::GetCycle() {
    return cycle;
}
