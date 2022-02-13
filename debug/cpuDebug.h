// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 24/12/2021.
//

#ifndef NES_EMU_CPUDEBUG_H
#define NES_EMU_CPUDEBUG_H

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

#include "../Bus.h"

class cpuDebug : public olc::PixelGameEngine{
public:
    cpuDebug();

    Bus nes;
    std::map<uint16_t, std::string> mapAsm;
    std::shared_ptr<Gamepak> gamepak;

    bool run = false;
    float residualTime = 0.0f;
    uint8_t currentPage = 0x00;
    uint8_t selectedPalette = 0x00;
    const int swatchSize = 6;
    bool test = false;

    static std::string toHex(uint32_t number, uint8_t numDigits);

    void drawRam(int x, int y, uint16_t numAddr, int numRows, int numColumns);
    void drawCpu(int x, int y);
    void drawCode(int x, int y, int numLines);

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    bool OnUserDestroy() override;

};


#endif //NES_EMU_CPUDEBUG_H
