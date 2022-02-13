// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 26/12/2021.
//

#ifndef NES_EMU_GAMEPAK_H
#define NES_EMU_GAMEPAK_H


#include <vector>
#include <cstdint>
#include <string>
#include <memory>

// Mappers inclusions (WIP)
#include "mappers/Mapper_000.h"

class Gamepak {
public:
    explicit Gamepak(const char *filename);

    ~Gamepak();

public:
    // The GamePak or Cartridge is capable of reading and writing to the CPU and the PPU Buses.

    // Communicate with CPU Bus
    bool cpuRead(uint16_t addr, uint8_t &data);

    bool cpuWrite(uint16_t addr, uint8_t data);

    // Communicate with the PPU Bus
    bool ppuRead(uint16_t addr, uint8_t &data);

    bool ppuWrite(uint16_t addr, uint8_t data);

private:

    // NOTE: This 2 vectors are set up by mappers. It means that we don't know the size of these before inserting the
    // GamePak.
    std::vector<uint8_t> prgROM;    // The Program ROM (The ROMs where the program is contained)

    std::vector<uint8_t> prgRAM;    // The Program RAM
    std::vector<uint8_t> chrROM;    // The Characters Memory (The ROMs that contains the graphics data of the game)
    uint8_t mapperID = 0;       // Mapper ID e.g. Super Mario Bros. 3 uses the ID 4 (http://bootgod.dyndns.org:7777/profile.php?id=5)

    uint8_t prgROMbanks = 0; // Contains the number of Program ROMs in 16KB Units
    uint8_t prgRAMbanks = 0; // Contains the number of Program RAMs in 8KB Units
    uint8_t chrROMbanks = 0;    // Contains the number of Characters ROMs in 8KB Units
    std::shared_ptr<Mapper> mapper;

public:

    enum MIRRORING {
        HORIZONTAL,
        VERTICAL,
        ONE_SCREEN,
        FOUR_SCREEN,
    } mirror = HORIZONTAL;

};

#endif //NES_EMU_GAMEPAK_H
