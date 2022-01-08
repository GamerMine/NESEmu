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

private:
    Bus *bus = nullptr;

    std::shared_ptr<Gamepak> gamepak;

    // The PPU has 4 nametables and 1 nametable is 1KB long. The PPU have only 2KB of VRAM, not enough for 4 nametables.
    // 2 of 4 nametables are mirrored which means they contain the same data as the 2 others.
    // So, we are creating an array of 2 * 1024 bits to store 2 nametables of 1KB each.
    // According to the nesdev wiki, it is called CIRAM.
    uint8_t ciram[2][1024];

    // The PPU has also a Palette RAM that can be accessed from 0x3F00 to 0x3F1F.
    uint8_t paletteRAM[32];

private:
    olc::Pixel colors[0x40]; // 0x40 represent the number of colors that the PPU is capable of generating
    olc::Sprite screenOut = olc::Sprite(256, 240); // The screen resolution that the PPU is outputting is 256x240
    olc::Sprite nametables[2] = {olc::Sprite(256, 240),olc::Sprite(256, 240)}; // See the CIRAM note
    olc::Sprite patternTables[2] = {olc::Sprite(128, 128), olc::Sprite(128, 128)};

    int16_t scanline = 0;
    int16_t cycle = 0;

public:
    olc::Sprite& getScreen();
    olc::Sprite& getNametable(uint8_t i);
    olc::Sprite& getPatternTable(uint8_t i);
    bool frameComplete = false;
};


#endif //NES_EMU_PPURP2C02_H
