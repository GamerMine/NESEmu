//
// Created by maxime on 26/12/2021.
//

#include "ppuRP2C02.h"

ppuRP2C02::ppuRP2C02() {
    // Here are all available colors
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
    
}

uint8_t ppuRP2C02::cpuRead(uint16_t addr) {
    switch (addr) {
        case 0x2000: // Controller
            break;
        case 0x2001: // Mask
            break;
        case 0x2002: // Status
            break;
        case 0x2003: // OAM Address
            break;
        case 0x2004: // OAM Data
            break;
        case 0x2005: // Scroll
            break;
        case 0x2006: // Address
            break;
        case 0x2007: // Data
            break;
        case 0x4014: // OAM DMA
            break;
    }
}

void ppuRP2C02::ppuWrite(uint16_t addr, uint8_t data) {
    // The Cartridge will intercept all the address that the Mapper accept
    if (gamepak->ppuWrite(addr, data)) {

    }
}

uint8_t ppuRP2C02::ppuRead(uint16_t addr) {
    uint8_t data = 0;
    if (gamepak->ppuRead(addr, data)) {

    }

    return data;
}

olc::Sprite &ppuRP2C02::getScreen() {
    return screenOut;
}

olc::Sprite &ppuRP2C02::getNametable(uint8_t i) {
    return nametables[i];
}

olc::Sprite &ppuRP2C02::getPatternTable(uint8_t i) {
    return patternTables[i];
}

void ppuRP2C02::clock() {
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
