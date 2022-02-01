// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 26/12/2021.
//

#include <fstream>
#include <filesystem>
#include "Gamepak.h"

Gamepak::Gamepak(const char *filename) {
    struct Header { // This is what is contained into a .nes file header
        char name[4]; //        Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
        uint8_t prgRomSize; //  Size of PRG ROM in 16KB units
        uint8_t chrRomSize; //  Size of CHR ROM in 8KB units
        uint8_t mapper1; //     Flags 6 : Mapper, curMirroring, battery, trainer
        uint8_t mapper2; //     Flags 7 : Mapper, VS/Playchoice, NES 2.0
        uint8_t prgRamSize; //  Flags 8 : PRG RAM size
        uint8_t tvSystem1; //   Flags 9 : TV system
        uint8_t tvSystem2; //   Flags 10 :  TV system, PRG RAM presence
        char unused[5]; //      Unused
    } header{};

    std::ifstream ifstream;
    ifstream.open(filename, std::ifstream::binary); // Open the .nes file in binary mode

    if (ifstream.is_open()) {
        ifstream.read((char*)&header, sizeof(Header)); // Read the file header and store it into the struct

        if (header.mapper1 & 0x04) { // If the bit 4 of Flags 6 is set, we will ignore the next 512 bytes of data because they are not used by any original games.
            ifstream.seekg(512, std::ios_base::cur); // Skip 512 bytes of useless data
        }

        mapperID = (((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4)); // Get the Mapper ID

        uint8_t inesFormat = 1;
        if (inesFormat == 0) { // Corresponding to Archaic iNES format

        } else if (inesFormat == 1) { // Corresponding to iNES format
            prgROMbanks = header.prgRomSize;
            prgROM.resize(prgROMbanks * 16 * 1024); // The PRG ROMs are 16KB memories, so we multiply that by the number of units
            ifstream.read((char*)prgROM.data(), prgROM.size()); // We can read the next x bytes of data corresponding to the size of the vector

            prgRAMbanks = header.prgRamSize;
            prgRAM.resize(prgRAMbanks * 8 * 1024);

            chrROMbanks = header.chrRomSize;
            chrROM.resize(chrROMbanks * 8 * 1024); // The CHR ROMs are 8KB memories, so we multiply that by the number of units
            ifstream.read((char*)chrROM.data(), chrROM.size()); // We can read the next x bytes of data corresponding to the size of the vector

            if ((header.mapper1 & 0x08) == 0x00) {
                if ((header.mapper1 & 0x01) == 0x00) {
                    Gamepak::curMirroring = Gamepak::MIRRORING::HORIZONTAL;
                } else if ((header.mapper1 & 0x01) == 0x01) {
                    Gamepak::curMirroring = Gamepak::MIRRORING::VERTICAL;
                }
            } else {
                curMirroring = Gamepak::MIRRORING::FOUR_SCREEN;
            }
        } else if (inesFormat == 2) { // Corresponding to NES 2.0 format

        }

        // Here we are selecting the correct Mapper (WIP)
        switch (mapperID) {
            case 0: mapper = std::make_shared<Mapper_000>(prgROMbanks, chrROMbanks); break;
        }

        ifstream.close();
    } else {
        throw std::invalid_argument("File not found!");
    }
}

Gamepak::~Gamepak() = default;

bool Gamepak::cpuRead(uint16_t addr, uint8_t &data) {
    uint32_t mapped_addr = 0; // Temporary variable that contains the new offset to then read in the PRG ROM
    if (mapper->cpuMapRead(addr, mapped_addr)) {
        data = prgROM[mapped_addr]; // Get the data at the mapped address
        return true; // This read is always successful if the Mapper accepted the request
    }
    return false; // This read is always unsuccessful if the Mapper does not accept the request
}

bool Gamepak::cpuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (mapper->cpuMapWrite(addr, mapped_addr)) {
        prgRAM[mapped_addr] = data;
        return true;
    }
    return false;
}

bool Gamepak::ppuRead(uint16_t addr, uint8_t &data) {
    uint32_t mapped_addr = 0;
    if (mapper->ppuMapRead(addr, mapped_addr)) {
        data = chrROM[mapped_addr];
        return true;
    }
    return false;
}

bool Gamepak::ppuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (mapper->ppuMapWrite(addr, mapped_addr)) {
        // TODO: CHR RAM writes if exists
        return true;
    }
    return false;
}

uint8_t Gamepak::getMirroringStatus() {
    return curMirroring;
}

