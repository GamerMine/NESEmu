// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 09/03/2022.
//

#ifndef NES_EMU_MAPPER_002_H
#define NES_EMU_MAPPER_002_H


#include "../Mapper.h"

class Mapper_002 : public Mapper {
public:
    Mapper_002(uint8_t prgRomUnits, uint8_t chrUnits);
    ~Mapper_002();

public:
    bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
    bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;

private:
    uint8_t bankSelect = 0;
};


#endif //NES_EMU_MAPPER_002_H
