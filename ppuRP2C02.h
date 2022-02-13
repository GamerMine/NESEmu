// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 26/12/2021.
//

#ifndef NES_EMU_PPURP2C02_H
#define NES_EMU_PPURP2C02_H


#include <cstdint>
#include <memory>
#include "Gamepak.h"

#include "debug/olcPixelGameEngine.h"

class Bus;

class ppuRP2C02 {
public:
    ppuRP2C02();
    ~ppuRP2C02();

    void connectBus(Bus *n) { bus = n; } // Manage Bus connection
    void connectGamepak(const std::shared_ptr<Gamepak>& gamepak); // Manage Cartridge connection

public:
    // NOTE: The PPU can access to the CPU BUS and his own bus that we will call the PPU BUS
    // In this case when you call cpuRead(addr), you are reading something from the CPU bus that can be used by the PPU

    // Read/Write to the CPU BUS
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr);

    // Read/Write to the PPU BUS
    void ppuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr);

    void clock(); // Like the CPU, the PPU has his clock function
    // The PPU has 4 nametables and 1 nametable is 1KB long. The PPU have only 2KB of VRAM, not enough for 4 nametables.
    // 2 of 4 nametables are mirrored which means they contain the same data as the 2 others.
    // So, we are creating an array of 2 * 1024 bits to store 2 nametables of 1KB each.
    // According to the nesdev wiki, it is called CIRAM.
    uint8_t ciram[2][1024];

    bool nmi = false;
private:

    Bus *bus = nullptr;

    std::shared_ptr<Gamepak> gamepak;

    // The PPU has also a Palette RAM that can be accessed from 0x3F00 to 0x3F1F.
    uint8_t paletteRAM[32] = {};

private:
    olc::Pixel colors[0x40]; // 0x40 represent the number of colors that the PPU is capable of generating
    olc::Sprite screenOut = olc::Sprite(256, 240); // The screen resolution that the PPU is outputting is 256x240
    olc::Sprite nametables[2] = {olc::Sprite(256, 240),olc::Sprite(256, 240)}; // See the CIRAM note
    olc::Sprite patternTables[2] = {olc::Sprite(128, 128), olc::Sprite(128, 128)};

    int16_t scanline = 0;
    int16_t cycle = 0;

private:
    union {
        struct {
            uint8_t nametableAddr1 : 1;
            uint8_t nametableAddr2 : 1;
            uint8_t vramAddrIncrement : 1;
            uint8_t spritePatternTableAddr : 1;
            uint8_t backgroundPatternTableAddr : 1;
            uint8_t spriteSize : 1;
            uint8_t ppuSelect : 1;
            uint8_t generateNMI : 1;
        };
        uint8_t rawData;
    } ppuCTRL;

    union {
        struct {
            uint8_t greyscale : 1;
            uint8_t showBackgroundLeft : 1;
            uint8_t showSpritesLeft : 1;
            uint8_t showBackground : 1;
            uint8_t showSprites : 1;
            uint8_t enphasizeRed : 1;
            uint8_t enphasizeGreen : 1;
            uint8_t enphasizeBlue : 1;
        };
        uint8_t rawData;
    } ppuMask;

    union {
        struct {
            uint8_t unused : 5;
            uint8_t spriteOverflow : 1;
            uint8_t spriteZeroHit : 1;
            uint8_t verticalBlank : 1;
        };
        uint8_t rawData;
    } ppuStatus;

    union ppuScroll {
        struct {
            uint16_t coarseX : 5;
            uint16_t coarseY : 5;
            uint16_t nametableSelect1 : 1;
            uint16_t nametableSelect2 : 1;
            uint16_t fineY : 3;
            uint16_t unused : 1;
        };
        uint16_t rawData = 0x0000;
    };

    uint8_t addressLatch = 0x00; // The CPU can only write 8 bit values. This is the reason of this latch
    uint8_t readBuffer = 0x00;

    ppuScroll vRegister; // Current VRAM Address
    ppuScroll tRegister; // Temporary VRAM Address
    uint8_t fineX = 0x00; // Fine X Scroll

    uint8_t bgNextTileID = 0x00;
    uint8_t bgNextTileAttribute = 0x00;
    uint8_t bgNextTileLSB = 0x00;
    uint8_t bgNextTileMSB = 0x00;

    uint16_t bgShifterPatternLO = 0x0000;
    uint16_t bgShifterPatternHI = 0x0000;
    uint16_t bgShifterAttributeLO = 0x0000;
    uint16_t bgShifterAttributeHI = 0x0000;

public:
    olc::Sprite& getScreen();
    olc::Sprite& getNametable(uint8_t i);
    olc::Sprite& getPatternTable(uint8_t i, uint8_t palette);
    olc::Pixel& getColor(uint8_t palette, uint8_t pixel);
    bool frameComplete = false;

public:
    // DEBUG METHODS, WILL BE REMOVED IN THE FUTURE
    void printPaletteRAM();
};




#endif //NES_EMU_PPURP2C02_H
