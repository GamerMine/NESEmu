// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 09/03/2022.
//

#include <cstdio>
#include "Mapper_002.h"

Mapper_002::Mapper_002(uint8_t prgRomUnits, uint8_t chrUnits) : Mapper(prgRomUnits, chrUnits) {

}

Mapper_002::~Mapper_002() = default;

bool Mapper_002::cpuMapRead(uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x8000 && addr <= 0xBFFF) {
        mapped_addr = (addr & 0x3FFF) + (bankSelect * 16 * 1024);
        return true;
    } else if (addr >= 0xC000 && addr <= 0xFFFF) {
        mapped_addr = (addr & 0x3FFF) + ((numPRGUnits - 1) * 16 * 1024);
        return true;
    }
    return false;
}

bool Mapper_002::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        bankSelect = addr & 0x000F;
    }
    return false;
}

bool Mapper_002::ppuMapRead(uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_002::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        mapped_addr = addr;
        return true;
    }
    return false;
}
