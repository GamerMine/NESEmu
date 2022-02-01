// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 26/12/2021.
//

#include "ppuRP2C02.h"

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
            break;
        case 0x0001: // Mask
            ppuMask.rawData = data;
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // Address
            if (addressLatch == 0x00) {
                ppuAddress = (ppuAddress & 0x00FF) | (data << 8);
                addressLatch = 0x01;
            } else {
                addressLatch = 0x00;
                ppuAddress = (ppuAddress & 0xFF00) | data;
            }
            break;
        case 0x0007: // Data
            ppuWrite(ppuAddress, data);
            ppuAddress++;
            break;
        case 0x4014: // OAM DMA
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
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // Address
            break;
        case 0x0007: // Data
            // According to nesdev, when reading from the ppuData register, we wants to have a read buffer. But, reading
            // palette data is direct, no need of a read buffer.
            data = readBuffer;
            readBuffer = ppuRead(ppuAddress);

            if (ppuAddress >= 0x3F00) data = readBuffer;
            ppuAddress++;
            break;
        case 0x4014: // OAM DMA
            break;
    }

    return data;
}

void ppuRP2C02::ppuWrite(uint16_t addr, uint8_t data) {
    // The Cartridge will intercept all the address that the Mapper accept
    if (gamepak->ppuWrite(addr, data)) {

    } else if (addr >= 0x0000 && addr <= 0x1FFF) { // Pattern table locations

    } else if (addr >= 0x2000 && addr <= 0x3EFF) { // Nametable locations
        if (Gamepak::getMirroringStatus() == Gamepak::MIRRORING::HORIZONTAL) {
            if (addr >= 0x2000 && addr <= 0x27FF) {
                ciram[0][(addr & 0x0400) & 0x0FFF] = data;
            } else if (addr >= 2800 && addr <= 0x2BFF) {
                ciram[1][(addr & 0x0400) & 0x0FFF] = data;
            }
        } else if (Gamepak::getMirroringStatus() == Gamepak::MIRRORING::VERTICAL) {
            if (addr >= 0x2000 && addr <= 0x23FF || addr >= 2800 && addr <= 0x2BFF) {
                ciram[0][(addr & 0x0400) & 0x0FFF] = data;
            } else if (addr >= 2400 && addr <= 0x27FF || addr >= 0x2C00 && addr <= 0x2FFF) {
                ciram[1][(addr & 0x0400) & 0x0FFF] = data;
            }
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
        if (Gamepak::getMirroringStatus() == Gamepak::MIRRORING::HORIZONTAL) {
            if (addr >= 0x2000 && addr <= 0x27FF) {
                data = ciram[0][(addr & 0x0400) & 0x0FFF];
            } else if (addr >= 2800 && addr <= 0x2BFF) {
                data = ciram[1][(addr & 0x0400) & 0x0FFF];
            }
        } else if (Gamepak::getMirroringStatus() == Gamepak::MIRRORING::VERTICAL) {
            if (addr >= 0x2000 && addr <= 0x23FF || addr >= 2800 && addr <= 0x2BFF) {
                data = ciram[0][(addr & 0x0400) & 0x0FFF];
            } else if (addr >= 2400 && addr <= 0x27FF || addr >= 0x2C00 && addr <= 0x2FFF) {
                data = ciram[1][(addr & 0x0400) & 0x0FFF];
            }
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

    if (scanline == -1 && cycle == 1) {
        ppuStatus.verticalBlank = 0;
    }

    // If the generate NMI is set in ppuCTRL, an NMI will be generated at each start of a frame
    if (scanline == 241 && cycle == 1) {
        ppuStatus.verticalBlank = 1;
        if (ppuCTRL.generateNMI) {
            nmi = true;
        }
    }

    // Old TV noise generation
    screenOut.SetPixel(cycle - 1, scanline, colors[(random() % 2) ? 0x3F : 0x30]);

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

// DEBUG METHODS, WILL BE REMOVED IN THE FUTURE
void ppuRP2C02::printPaletteRAM() {
    printf("Palette RAM content :\n");
    for (uint8_t value : paletteRAM) {
        printf("%X | ", value);
    }
    printf("\n\n");
}
