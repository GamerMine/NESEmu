// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 19/02/2022.
//

#include "apu2A03.h"

apu2A03::apu2A03() = default;

apu2A03::~apu2A03() = default;

void apu2A03::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
        case 0x4000:
            channelPulse1Status = data;
            break;

        case 0x4001:
            channelPulse1Sweep = data;
            break;

        case 0x4002:
            channelPulse1TimerLo = data;
            break;

        case 0x4003:
            channelPulse1TimerHi = data;
            break;

        case 0x4004:
            channelPulse2Status = data;
            break;

        case 0x4005:
            channelPulse2Sweep = data;
            break;

        case 0x4006:
            channelPulse2TimerLo = data;
            break;

        case 0x4007:
            channelPulse2TimerHi = data;
            break;

        case 0x4008:
            break;

        case 0x4009:
            break;

        case 0x400A:
            break;

        case 0x400B:
            break;

        case 0x400C:
            break;

        case 0x400D:
            break;

        case 0x400E:
            break;

        case 0x400F:
            break;

        case 0x4010:
            break;

        case 0x4011:
            break;

        case 0x4012:
            break;

        case 0x4013:
            break;

        case 0x4015:
            apuStatus.rawData = data;

            if (!apuStatus.channelPulse1) {
                channelPulse1TimerHi &= 0x07;
            }

            if (!apuStatus.channelPulse2) {
                channelPulse2TimerHi &= 0x07;
            }

            if (!apuStatus.channelTriangle) {
                channelTriangleTimerHi &= 0x07;
            }

            if (!apuStatus.channelNoise) {
                channelNoiseLengthCounterLoad &= 0x07;
            }

            break;

        case 0x4017:
            break;
    }
}

uint8_t apu2A03::cpuRead(uint16_t addr) {
    uint8_t data = 0x00;

    if (addr == 0x4015) {
        data |= ((channelPulse1TimerHi & 0xF1) > 0) & 0x01;
        data |= ((channelPulse2TimerHi & 0xF1) > 0) & 0x02;
        data |= ((channelTriangleTimerHi & 0xF1) > 0) & 0x03;
        data |= ((channelNoiseLengthCounterLoad & 0xF1) > 0) & 0x04;
    }

    return data;
}

void apu2A03::clock() {

}

void apu2A03::reset() {
    apuStatus.rawData = 0x00;
}
