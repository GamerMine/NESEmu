// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 27/12/2021.
//

#ifndef NES_EMU_MAPPER_H
#define NES_EMU_MAPPER_H


#include <cstdint>

class Mapper {
public:
    Mapper(uint8_t prgUnits, uint8_t chrUnits);
    ~Mapper();

public:
    virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

protected:
    uint8_t numPRGUnits = 0;
    uint8_t numCHRUnits = 0;
};


#endif //NES_EMU_MAPPER_H
