// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 26/12/2021.
//

#include "ppuRP2C02.h"
#include <bitset>

ppuRP2C02::ppuRP2C02() {
    // Here are all available colors
    // They are all hard coded
    colors[0x00] = olc::Pixel(84, 84, 84);
    colors[0x01] = olc::Pixel(0, 30, 116);
    colors[0x02] = olc::Pixel(8, 16, 144);
    colors[0x03] = olc::Pixel(48, 0, 136);
    colors[0x04] = olc::Pixel(68, 0, 100);
    colors[0x05] = olc::Pixel(92, 0, 48);
    colors[0x06] = olc::Pixel(84, 4, 0);
    colors[0x07] = olc::Pixel(60, 24, 0);
    colors[0x08] = olc::Pixel(32, 42, 0);
    colors[0x09] = olc::Pixel(8, 58, 0);
    colors[0x0A] = olc::Pixel(0, 64, 0);
    colors[0x0B] = olc::Pixel(0, 60, 0);
    colors[0x0C] = olc::Pixel(0, 50, 60);
    colors[0x0D] = olc::Pixel(0, 0, 0);
    colors[0x0E] = olc::Pixel(0, 0, 0);
    colors[0x0F] = olc::Pixel(0, 0, 0);

    colors[0x10] = olc::Pixel(152, 150, 152);
    colors[0x11] = olc::Pixel(8, 76, 196);
    colors[0x12] = olc::Pixel(48, 50, 236);
    colors[0x13] = olc::Pixel(92, 30, 228);
    colors[0x14] = olc::Pixel(136, 20, 176);
    colors[0x15] = olc::Pixel(160, 20, 100);
    colors[0x16] = olc::Pixel(152, 34, 32);
    colors[0x17] = olc::Pixel(120, 60, 0);
    colors[0x18] = olc::Pixel(84, 90, 0);
    colors[0x19] = olc::Pixel(40, 114, 0);
    colors[0x1A] = olc::Pixel(8, 124, 0);
    colors[0x1B] = olc::Pixel(0, 118, 40);
    colors[0x1C] = olc::Pixel(0, 102, 120);
    colors[0x1D] = olc::Pixel(0, 0, 0);
    colors[0x1E] = olc::Pixel(0, 0, 0);
    colors[0x1F] = olc::Pixel(0, 0, 0);

    colors[0x20] = olc::Pixel(236, 238, 236);
    colors[0x21] = olc::Pixel(76, 154, 236);
    colors[0x22] = olc::Pixel(120, 124, 236);
    colors[0x23] = olc::Pixel(176, 98, 236);
    colors[0x24] = olc::Pixel(228, 84, 236);
    colors[0x25] = olc::Pixel(236, 88, 180);
    colors[0x26] = olc::Pixel(236, 106, 100);
    colors[0x27] = olc::Pixel(212, 136, 32);
    colors[0x28] = olc::Pixel(160, 170, 0);
    colors[0x29] = olc::Pixel(116, 196, 0);
    colors[0x2A] = olc::Pixel(76, 208, 32);
    colors[0x2B] = olc::Pixel(56, 204, 108);
    colors[0x2C] = olc::Pixel(56, 180, 204);
    colors[0x2D] = olc::Pixel(60, 60, 60);
    colors[0x2E] = olc::Pixel(0, 0, 0);
    colors[0x2F] = olc::Pixel(0, 0, 0);

    colors[0x30] = olc::Pixel(236, 238, 236);
    colors[0x31]= olc::Pixel(168, 204, 236);
    colors[0x32] = olc::Pixel(188, 188, 236);
    colors[0x33] = olc::Pixel(212, 178, 236);
    colors[0x34] = olc::Pixel(236, 174, 236);
    colors[0x35] = olc::Pixel(236, 174, 212);
    colors[0x36] = olc::Pixel(236, 180, 176);
    colors[0x37] = olc::Pixel(228, 196, 144);
    colors[0x38] = olc::Pixel(204, 210, 120);
    colors[0x39] = olc::Pixel(180, 222, 120);
    colors[0x3A] = olc::Pixel(168, 226, 144);
    colors[0x3B] = olc::Pixel(152, 226, 180);
    colors[0x3C] = olc::Pixel(160, 214, 228);
    colors[0x3D] = olc::Pixel(160, 162, 160);
    colors[0x3E] = olc::Pixel(0, 0, 0);
    colors[0x3F] = olc::Pixel(0, 0, 0);
}

ppuRP2C02::~ppuRP2C02() {

}

void ppuRP2C02::connectGamepak(const std::shared_ptr<Gamepak>& gamepak) {
    this->gamepak = gamepak;
}

void ppuRP2C02::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
        case 0x0000: // Control
            ppuCTRL.rawData = data;
            tRegister.nametableSelect1 = ppuCTRL.nametableAddr1;
            tRegister.nametableSelect2 = ppuCTRL.nametableAddr2;
            break;
        case 0x0001: // Mask
            ppuMask.rawData = data;
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            oamAddr = data;
            break;
        case 0x0004: // OAM Data
            oam[oamAddr] = data;
            break;
        case 0x0005: // Scroll
            if (addressLatch == 0x00) {
                fineX = data & 0x07;
                tRegister.coarseX = data >> 3;
                addressLatch = 0x01;
            } else {
                tRegister.fineY = data & 0x07;
                tRegister.coarseY = data >> 3;
                addressLatch = 0x00;
            }
            break;
        case 0x0006: // Address
            if (addressLatch == 0x00) {
                tRegister.rawData = (uint16_t)((data & 0x3F) << 8)
                        | (tRegister.rawData & 0x00FF);
                addressLatch = 0x01;
            } else {
                tRegister.rawData = (tRegister.rawData & 0xFF00)
                        | data;
                vRegister = tRegister;
                addressLatch = 0x00;
            }
            break;
        case 0x0007: // Data
            ppuWrite(vRegister.rawData, data);
            vRegister.rawData += (ppuCTRL.vramAddrIncrement ? 32 : 1);
            break;
    }
}

uint8_t ppuRP2C02::cpuRead(uint16_t addr) {
    uint8_t data = 0x00;
    switch (addr) {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            data = (ppuStatus.rawData & 0xE0) | (readBuffer & 0x1F);
            ppuStatus.verticalBlank = 0;
            addressLatch = 0x00;
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            data = oam[oamAddr];
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // Address
            break;
        case 0x0007: // Data
            // According to nesdev, when reading from the ppuData register, we wants to have a read buffer. But, reading
            // palette data is direct, no need of a read buffer.
            data = readBuffer;
            readBuffer = ppuRead(vRegister.rawData);

            if (vRegister.rawData >= 0x3F00) data = readBuffer;
            vRegister.rawData += (ppuCTRL.vramAddrIncrement ? 32 : 1);
            break;
    }

    return data;
}

void ppuRP2C02::ppuWrite(uint16_t addr, uint8_t data) {
    // The Cartridge will intercept all the address that the Mapper accept
    if (gamepak->ppuWrite(addr, data)) {

    } else if (addr >= 0x0000 && addr <= 0x1FFF) { // Pattern table locations
        // Pattern tables are managed by the getPatternTable function
    } else if (addr >= 0x2000 && addr <= 0x3EFF) { // Nametable locations
        if (gamepak->mirror == Gamepak::MIRRORING::VERTICAL)
        {
            // Vertical
            if (addr >= 0x2000 && addr <= 0x23FF)
                ciram[0][addr & 0x03FF] = data;
            if (addr >= 0x2400 && addr <= 0x27FF)
                ciram[1][addr & 0x03FF] = data;
            if (addr >= 0x2800 && addr <= 0x2BFF)
                ciram[0][addr & 0x03FF] = data;
            if (addr >= 0x2C00 && addr <= 0x2FFF)
                ciram[1][addr & 0x03FF] = data;
        }
        else if (gamepak->mirror == Gamepak::MIRRORING::HORIZONTAL)
        {
            // Horizontal
            if (addr >= 0x2000 && addr <= 0x23FF)
                ciram[0][addr & 0x03FF] = data;
            if (addr >= 0x2400 && addr <= 0x27FF)
                ciram[0][addr & 0x03FF] = data;
            if (addr >= 0x2800 && addr <= 0x2BFF)
                ciram[1][addr & 0x03FF] = data;
            if (addr >= 0x2C00 && addr <= 0x2FFF)
                ciram[1][addr & 0x03FF] = data;
        }
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) { // Color palette locations
        addr &= 0x001F;
        // This are hardcoded values that are specifically mirrored. See : https://wiki.nesdev.org/w/index.php?title=PPU_palettes#Memory_Map
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        paletteRAM[addr] = data;
    }
}

uint8_t ppuRP2C02::ppuRead(uint16_t addr) {
    // The following locations are all listed here : https://wiki.nesdev.org/w/index.php?title=PPU_memory_map
    uint8_t data = 0;
    if (gamepak->ppuRead(addr, data)) {

    } else if (addr >= 0x0000 && addr <= 0x1FFF) { // Pattern table locations
        /*
         * This is actually simple :
         * There is 2 pattern tables, 0 and 1
         * The first one is located between 0x0000 and 0x0FFF
         * The second one is located between 0x1000 and 0x1FFF
         * So, to select the correct pattern table we are just keeping the first hexadecimal number
         * Then, we keep the 3 next numbers to select the pixel.
         */
        //data = patternTables[addr >> 12].GetPixel(addr & 0x0FFF);
    } else if (addr >= 0x2000 && addr <= 0x3EFF) { // Nametable locations
        if (gamepak->mirror == Gamepak::MIRRORING::VERTICAL)
        {
            // Vertical
            if (addr >= 0x2000 && addr <= 0x23FF)
                data = ciram[0][addr & 0x03FF];
            if (addr >= 0x2400 && addr <= 0x27FF)
                data = ciram[1][addr & 0x03FF];
            if (addr >= 0x2800 && addr <= 0x2BFF)
                data = ciram[0][addr & 0x03FF];
            if (addr >= 0x2C00 && addr <= 0x2FFF)
                data = ciram[1][addr & 0x03FF];
        }
        else if (gamepak->mirror == Gamepak::MIRRORING::HORIZONTAL)
        {
            // Horizontal
            if (addr >= 0x2000 && addr <= 0x23FF)
                data = ciram[0][addr & 0x03FF];
            if (addr >= 0x2400 && addr <= 0x27FF)
                data = ciram[0][addr & 0x03FF];
            if (addr >= 0x2800 && addr <= 0x2BFF)
                data = ciram[1][addr & 0x03FF];
            if (addr >= 0x2C00 && addr <= 0x2FFF)
                data = ciram[1][addr & 0x03FF];
        }
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) { // Color palette locations
        addr &= 0x001F;
        // This are hardcoded values that are specifically mirrored. See : https://wiki.nesdev.org/w/index.php?title=PPU_palettes#Memory_Map
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        data = paletteRAM[addr];
    }

    return data;
}

olc::Sprite &ppuRP2C02::getScreen() {
    return screenOut;
}

olc::Sprite &ppuRP2C02::getNametable(uint8_t i) {
    return nametables[i];
}

olc::Sprite &ppuRP2C02::getPatternTable(uint8_t i, uint8_t palette) {
    // The pattern table is divided into 2 256 tiles planes. Each tile is 16 bytes made of two planes.
    // So, one tile plane is 4096 bytes long (4KB or 0x0FFF)
    uint8_t num = 0x00;
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            // The idea here is to make a 1D offset from x and y
            // One table is 256 bytes long
            uint16_t offset = x * 256 + y * 16;

            for (int row = 0; row < 8; row++) {
                uint8_t tileLSB = ppuRead(i * 0x1000 + offset + row); // We are selecting the tile plane and adding the offset
                uint8_t tileMSB = ppuRead(i * 0x1000 + offset + row + 8);
                for (int col = 0; col < 8; col++) {
                    uint8_t pixel = (tileLSB & 0x01) + (tileMSB & 0x01);
                    tileLSB >>= 1;
                    tileMSB >>= 1;

                    patternTables[i].SetPixel(y * 8 + (7 - col), x * 8 + row, getColor(palette, pixel));
                }
            }
        }
    }
    return patternTables[i];
}

olc::Pixel& ppuRP2C02::getColor(uint8_t palette, uint8_t pixel) {
    //printf("%X\n", (0x3F00 + (palette << 2) + pixel) & 0x3F);
    return colors[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

void ppuRP2C02::clock() {

    // ####################################################################################################################
    // ####################################################################################################################
    //                                               BACKGROUND RENDERING
    // ####################################################################################################################
    // ####################################################################################################################

    auto incrementScrollX = [&]() {
        if (ppuMask.showBackground || ppuMask.showSprites) {
            if (vRegister.coarseX == 31) {
                vRegister.coarseX = 0;
                vRegister.nametableSelect1 = ~vRegister.nametableSelect1;
            } else {
                vRegister.coarseX++;
            }
        }
    };

    auto incrementScrollY = [&]() {
        if (ppuMask.showBackground || ppuMask.showSprites) {
            if (vRegister.fineY < 7) {
                vRegister.fineY++;
            } else {
                vRegister.fineY = 0;
                if (vRegister.coarseY == 29) {
                    vRegister.coarseY = 0;
                    vRegister.nametableSelect2 = ~vRegister.nametableSelect2;
                } else if (vRegister.coarseY == 31) {
                    vRegister.coarseY = 0;
                } else {
                    vRegister.coarseY++;
                }
            }
        }
    };

    auto transferAddressX = [&]() {
        if (ppuMask.showBackground || ppuMask.showSprites) {
            vRegister.nametableSelect1 = tRegister.nametableSelect1;
            vRegister.coarseX = tRegister.coarseX;
        }
    };

    auto transferAddressY = [&]() {
        if (ppuMask.showBackground || ppuMask.showSprites) {
            vRegister.fineY = tRegister.fineY;
            vRegister.nametableSelect2 = tRegister.nametableSelect2;
            vRegister.coarseY = tRegister.coarseY;
        }
    };

    auto loadBackgroundShifters = [&]() {
        bgShifterPatternLO = (bgShifterPatternLO & 0xFF00) | bgNextTileLSB;
        bgShifterPatternHI = (bgShifterPatternHI & 0xFF00) | bgNextTileMSB;

        bgShifterAttributeLO = (bgShifterAttributeLO & 0xFF00) | ((bgNextTileAttribute & 0b01) ? 0xFF : 0x00);
        bgShifterAttributeHI = (bgShifterAttributeHI & 0xFF00) | ((bgNextTileAttribute & 0b10) ? 0xFF : 0x00);
    };

    auto updateBackgroundShifters = [&]() {
        if (ppuMask.showBackground) {
            bgShifterPatternLO <<= 1;
            bgShifterPatternHI <<= 1;

            bgShifterAttributeLO <<= 1;
            bgShifterAttributeHI <<= 1;
        }
    };

    auto updateSpritesShifters = [&]() {
        if (ppuMask.showSprites && cycle >= 1 && cycle < 258) {
            for (int i = 0; i < spriteNb; i++) {
                if (spriteScanline[i].posX > 0) {
                    spriteScanline[i].posX--;
                } else {
                    spritePatternShifterLo[i] <<= 1;
                    spritePatternShifterHi[i] <<= 1;
                }
            }
        }
    };

    if (scanline >= -1 && scanline < 240) {

        if (scanline == 0 && cycle == 0) {
            cycle = 1;
        }

        if (scanline == -1 && cycle == 1) {
            ppuStatus.verticalBlank = 0;
            ppuStatus.spriteOverflow = 0;
            ppuStatus.spriteZeroHit = 0;

            for (int i = 0; i < 8; i++) {
                spritePatternShifterLo[i] = 0x00;
                spritePatternShifterHi[i] = 0x00;
            }
        }

        if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {

            updateBackgroundShifters();
            updateSpritesShifters();

            switch ((cycle - 1) % 8) {
                case 0:
                    loadBackgroundShifters();
                    bgNextTileID = ppuRead(0x2000 | (vRegister.rawData & 0x0FFF));
                    break;
                case 2:
                    bgNextTileAttribute = ppuRead(0x23C0 | (vRegister.nametableSelect2 << 11)
                            | (vRegister.nametableSelect1 << 10)
                            | ((vRegister.coarseY >> 2) << 3)
                            | (vRegister.coarseX >> 2));
                    if (vRegister.coarseY & 0x02) bgNextTileAttribute >>= 4;
                    if (vRegister.coarseX & 0x02) bgNextTileAttribute >>= 2;
                    bgNextTileAttribute &= 0x03;
                    break;
                case 4:
                    bgNextTileLSB = ppuRead((ppuCTRL.backgroundPatternTableAddr << 12) + ((uint16_t)bgNextTileID << 4) + (vRegister.fineY));
                    break;
                case 6:
                    bgNextTileMSB = ppuRead((ppuCTRL.backgroundPatternTableAddr << 12) + ((uint16_t)bgNextTileID << 4) + (vRegister.fineY) + 8);
                    break;
                case 7:
                    incrementScrollX();
                    break;
            }
        }

        if (cycle == 256) {
            incrementScrollY();
        }

        if (cycle == 257) {
            loadBackgroundShifters();
            transferAddressX();
        }

        if (cycle == 338 || cycle == 340)
        {
            bgNextTileID = ppuRead(0x2000 | (vRegister.rawData & 0x0FFF));
        }

        if (scanline == -1 && cycle >= 280 && cycle < 305)
        {
            // End of vertical blank period so reset the Y address ready for rendering
            transferAddressY();
        }

        // ####################################################################################################################
        // ####################################################################################################################
        //                                               SPRITES RENDERING
        // ####################################################################################################################
        // ####################################################################################################################
        // FIXME: Cycle accurate implementation for sprites.
        if (scanline >= -1 && scanline < 238) {
            if (cycle == 257) {
                std::memset(spriteScanline, 0xFF, 8 * sizeof(objectAttributeMemoryEntry));
                spriteNb = 0x00;
                currentOAMEntry = 0x00;
                spriteZeroPossible = false;
                while (currentOAMEntry < 64 && spriteNb < 9) {
                    // The goal here is to find all the sprites that will render on the next scanline.
                    // To do that, we do the difference between the scanline, and the OAM entry y position.
                    // Then we know that if the result value is contained between 0 and 7 (or 0 and 15, depending on the sprite height),
                    // the sprite is contained in the next scanline.
                    int16_t diff = scanline - OAM[currentOAMEntry].posY;
                    if (diff >= 0 && diff < (ppuCTRL.spriteSize ? 16 : 8)) {
                        if (spriteNb < 8) {

                            if (currentOAMEntry == 0x00) {
                                spriteZeroPossible = true;
                            }

                            memcpy(&spriteScanline[spriteNb], & OAM[currentOAMEntry], sizeof(objectAttributeMemoryEntry));
                            spriteNb++; // Another sprite is in the next scanline
                        } else {
                            ppuStatus.spriteOverflow = 1; // If there is already 8 sprites into the secondary OAM, it means that there is a sprite overflow.
                        }
                    }
                    currentOAMEntry++;
                }
            }

            if (cycle == 340) {
                for (uint8_t i = 0; i < spriteNb; i++) {
                    uint16_t spritePatternTableAddrLo = 0x0000;
                    uint16_t spritePatternTableAddrHi = 0x0000;
                    uint8_t spritePatternTableDataLo = 0x00;
                    uint8_t spritePatternTableDataHi = 0x00;

                    if (!ppuCTRL.spriteSize) {
                        // 8x8 sprite
                        if (!(spriteScanline[i].attribute & 0x80)) {
                            // The sprite is not flipped vertically
                            // This part is a little complex. So to get the pattern table address :
                            // 1. Get the sprite pattern address contained in the PPU Control register
                            // 2. Shift it 12 bits to the left. (If spritePatternTableAddrLo == 0 the address will be: 0x0000
                            // else if spritePatternTableAddrLo == 1 the address will be 0x1000.
                            // 3. Get where the sprite is and shift this value to the left by 4 bits.
                            // 4. Get the row we are interested in.
                            spritePatternTableAddrLo =
                                    (ppuCTRL.spritePatternTableAddr << 12)
                                    | (spriteScanline[i].tileIndex << 4)
                                    | (scanline - spriteScanline[i].posY);
                        } else {
                            // The sprite is flipped vertically
                            // Same, but the sprite is vertically flipped, so we just invert all the bits.
                            // To do that we know that a sprite has 8 rows (0-7)
                            // So if we want the 8th (actually 7th) row, but you flip the draw vertically, you actually want the 1st (0th actually) row.
                            // We just do 7 minus the row.
                            spritePatternTableAddrLo =
                                    (ppuCTRL.spritePatternTableAddr << 12)
                                    | (spriteScanline[i].tileIndex << 4)
                                    | (7 - (scanline - spriteScanline[i].posY));
                        }
                    } else {
                        // 8x16 sprite
                        if (!(spriteScanline[i].attribute & 0x80)) {
                            // The sprite is not flipped vertically
                            if (scanline - spriteScanline[i].posY < 8) {
                                spritePatternTableAddrLo =
                                        ((spriteScanline[i].tileIndex & 0x01) << 12)
                                        | ((spriteScanline[i].tileIndex & 0xFE) << 4)
                                        | ((scanline - spriteScanline[i].posY) & 0x07);
                            } else {
                                spritePatternTableAddrLo =
                                        ((spriteScanline[i].tileIndex & 0x01) << 12)
                                        | (((spriteScanline[i].tileIndex & 0xFE) + 1) << 4)
                                        | ((scanline - spriteScanline[i].posY) & 0x07);
                            }
                        } else {
                            // The sprite is flipped vertically
                            if (scanline - spriteScanline[i].posY < 8) {
                                spritePatternTableAddrLo =
                                        ((spriteScanline[i].tileIndex & 0x01) << 12)
                                        | (((spriteScanline[i].tileIndex & 0xFE) + 1) << 4)
                                        | (7 - (scanline - spriteScanline[i].posY) & 0x07);
                            } else {
                                spritePatternTableAddrLo =
                                        ((spriteScanline[i].tileIndex & 0x01) << 12)
                                        | ((spriteScanline[i].tileIndex & 0xFE) << 4)
                                        | (7 - (scanline - spriteScanline[i].posY) & 0x07);
                            }
                        }
                    }
                    spritePatternTableAddrHi = spritePatternTableAddrLo + 8;
                    spritePatternTableDataLo = ppuRead(spritePatternTableAddrLo);
                    spritePatternTableDataHi = ppuRead(spritePatternTableAddrHi);

                   if (spriteScanline[i].attribute & 0x40)
                    {
                        auto reverseBytes = [](uint8_t b)
                        {
                            b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
                            b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
                            b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
                            return b;
                        };

                        // Flip Patterns Horizontally
                        spritePatternTableDataLo = reverseBytes(spritePatternTableDataLo);
                        spritePatternTableDataHi = reverseBytes(spritePatternTableDataHi);
                    }

                    spritePatternShifterLo[i] = spritePatternTableDataLo;
                    spritePatternShifterHi[i] = spritePatternTableDataHi;
                }
            }
        }
    }



    if (scanline >= 241 && scanline < 261)
    {
        if (scanline == 241 && cycle == 1)
        {
            // Effectively end of frame, so set vertical blank flag
            ppuStatus.verticalBlank = 1;

            if (ppuCTRL.generateNMI)
                nmi = true;
        }
    }

    uint8_t bgPixel = 0x00;
    uint8_t bgPalette = 0x00;

    if (ppuMask.showBackground) {
        uint16_t bit = 0x8000 >> fineX;

        uint8_t pixel0 = (bgShifterPatternLO & bit) > 0;
        uint8_t pixel1 = (bgShifterPatternHI & bit) > 0;

        bgPixel = (pixel1 << 1) | pixel0;

        uint8_t palette0 = (bgShifterAttributeLO & bit) > 0;
        uint8_t palette1 = (bgShifterAttributeHI & bit) > 0;

        bgPalette = (palette1 << 1) | palette0;
    }

    uint8_t spritePixel = 0x00;
    uint8_t spritePalette = 0x00;
    uint8_t spritePriority = 0x00;

    if (ppuMask.showSprites) {
        spriteZeroRendered = false;
        for (int i = 0; i < spriteNb; i++) {
            if (spriteScanline[i].posX == 0) {
                uint8_t spritePixelLo = (spritePatternShifterLo[i] & 0x80) > 0;
                uint8_t spritePixelHi = (spritePatternShifterHi[i] & 0x80) > 0;
                spritePixel = (spritePixelHi << 1) | spritePixelLo;

                spritePalette = (spriteScanline[i].attribute & 0x03) + 0x04;
                spritePriority = (spriteScanline[i].attribute & 0x20) == 0;

                if (spritePixel != 0)
                {
                    if (i == 0) {
                        spriteZeroRendered = true;
                    }
                    break;
                }
            }
        }
    }

    uint8_t pixel = 0x00;
    uint8_t palette = 0x00;

    if (bgPixel == 0 && spritePixel == 0)
    {
        // The background pixel is transparent
        // The foreground pixel is transparent
        // No winner, draw "background" colour
        pixel = 0x00;
        palette = 0x00;
    }
    else if (bgPixel == 0 && spritePixel > 0)
    {
        // The background pixel is transparent
        // The foreground pixel is visible
        // Foreground wins!
        pixel = spritePixel;
        palette = spritePalette;
    }
    else if (bgPixel > 0 && spritePixel == 0)
    {
        // The background pixel is visible
        // The foreground pixel is transparent
        // Background wins!
        pixel = bgPixel;
        palette = bgPalette;
    }
    else if (bgPixel > 0 && spritePixel > 0)
    {
        // The background pixel is visible
        // The foreground pixel is visible
        // Hmmm...
        if (spritePriority)
        {
            // Foreground cheats its way to victory!
            pixel = spritePixel;
            palette = spritePalette;
        }
        else
        {
            // Background is considered more important!
            pixel = bgPixel;
            palette = bgPalette;
        }

        // Sprite Zero Hit detection
        if (spriteZeroPossible && spriteZeroRendered)
        {
            // Sprite zero is a collision between foreground and background
            // so they must both be enabled
            if (ppuMask.showBackground & ppuMask.showSprites)
            {
                // The left edge of the screen has specific switches to control
                // its appearance. This is used to smooth inconsistencies when
                // scrolling (since sprites x coord must be >= 0)
                if (~(ppuMask.showBackgroundLeft | ppuMask.showSpritesLeft))
                {
                    if (cycle >= 9 && cycle < 258)
                    {
                        ppuStatus.spriteZeroHit = 1;
                    }
                }
                else
                {
                    if (cycle >= 1 && cycle < 258)
                    {
                        ppuStatus.spriteZeroHit = 1;
                    }
                }
            }
        }
    }

    // Old TV noise generation
    screenOut.SetPixel(cycle - 1, scanline, getColor(palette, pixel));

    cycle++;
    if (cycle >= 341) { // This hardcoded value is the number of cycles needed for 1 line to complete
        cycle = 0; // We reset the cycle which correspond to the lines
        scanline++; // We start the next column

        if (scanline >= 261) { // This hardcoded value is the number of scanlines needed for an entire screen to complete
            scanline = -1;
            frameComplete = true;
        }
    }
}
