// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 19/02/2022.
//

#ifndef NES_EMU_APU2A03_H
#define NES_EMU_APU2A03_H

#include <cstdint>
#include "sound//PulseWave.h"

class apu2A03 {
public:
    apu2A03();
    ~apu2A03();

public:
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr);

    void clock();
    void reset();

private:

    void play();

    uint8_t counter = 0x00;

private:
    union {
        struct {
            uint8_t channelPulse1 : 1;
            uint8_t channelPulse2 : 1;
            uint8_t channelTriangle : 1;
            uint8_t channelNoise : 1;
            uint8_t channelDMC : 1;
            uint8_t unused : 3;
        };
        uint8_t rawData;
    } apuStatus{};

    // Pulse 1 channel register
    union {
        union {
            struct {
                uint8_t envelopeDivider : 4;
                uint8_t envelopeFlag : 1;
                uint8_t lengthCounterHalt : 1;
                uint8_t dutyCycle : 2;
            };
            uint8_t rawData;
        } status;

        union {
            struct {
                uint8_t shiftCount : 3;
                uint8_t negateFlag : 1;
                uint8_t deviderPeriod : 3;
                uint8_t enabledFlag : 1;
            };
            uint8_t rawData;
        } sweep;

        uint8_t timerLo;

        union {
            struct {
                uint8_t timerHi : 3;
                uint8_t lenghtCounterLoad : 5;
            };
            uint8_t rawData;
        } timerHi;
    } channelPulse1;

    PulseWave pulseWave{44100};
    // Pulse 2 channel registers
    uint8_t channelPulse2Status = 0x00;
    uint8_t channelPulse2Sweep = 0x00;
    uint8_t channelPulse2TimerLo = 0x00;
    uint8_t channelPulse2TimerHi = 0x00; // It also contains the length counter load

    // Triangle channel registers
    uint8_t channelTriangleLinearCounter = 0x00;
    uint8_t channelTriangleTimerLo = 0x00;
    uint8_t channelTriangleTimerHi = 0x00; // It also contains the length counter load

    // Noise channel registers
    uint8_t channelNoiseStatus = 0x00;
    uint8_t channelNoiseModePeriod = 0x00;
    uint8_t channelNoiseLengthCounterLoad = 0x00;

    // Frame counter
    uint8_t frameCounterData = 0x00;
    uint8_t sequencer = 0x01;
};


#endif //NES_EMU_APU2A03_H
