// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 19/02/2022.
//

#include "apu2A03.h"
#include "cpu6502.h"

apu2A03::apu2A03() = default;

apu2A03::~apu2A03() = default;

void apu2A03::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
        case 0x4000:
            channelPulse1.status.rawData = data;

            play();
            break;

        case 0x4001:
            channelPulse1.sweep.rawData = data;
            break;

        case 0x4002:
            channelPulse1.timerLo = data;
            break;

        case 0x4003:
            channelPulse1.timerHi.rawData = data;
            break;

        case 0x4004:
            // channelPulse2Status = data;
            break;

        case 0x4005:
            // channelPulse2Sweep = data;
            break;

        case 0x4006:
            // channelPulse2TimerLo = data;
            break;

        case 0x4007:
            // channelPulse2TimerHi = data;
            break;

        case 0x4008:
            // channelTriangleLinearCounter = data;
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
                channelPulse1.timerHi.lenghtCounterLoad = 0x00;
                if (pulseWave.isPlaying()) {
                    pulseWave.mute();
                }
            } else {
                pulseWave.unmute();
            }

            /*if (!apuStatus.channelPulse2) {
                channelPulse2TimerHi &= 0x07;
            }

            if (!apuStatus.channelTriangle) {
                channelTriangleTimerHi &= 0x07;
            }

            if (!apuStatus.channelNoise) {
                channelNoiseLengthCounterLoad &= 0x07;
            }*/

            break;

        case 0x4017:
            frameCounterData = data;
            break;
    }
}

uint8_t apu2A03::cpuRead(uint16_t addr) {
    uint8_t data = 0x00;

    if (addr == 0x4015) {
        data |= (channelPulse1.timerHi.lenghtCounterLoad > 0) & 0x01;
/*        data |= ((channelPulse2TimerHi & 0xF1) > 0) & 0x02;
        data |= ((channelTriangleTimerHi & 0xF1) > 0) & 0x03;
        data |= ((channelNoiseLengthCounterLoad & 0xF1) > 0) & 0x04;*/
    }

    return data;
}

bool clocked = false;
void apu2A03::clock() {
    // Executed every 2 CPU cycles
    if (counter % 6 == 0) {
        if (sequencer >= (frameCounterData >> 7 == 0 ? 4 : 5)) {
            sequencer = 0x00;
        }
        sequencer++;
    }

    // The length counter of each channels is clocked twice per sequences.
    if (sequencer % 2 == 0) {
        if (!channelPulse1.status.lengthCounterHalt) {
            if (channelPulse1.timerHi.lenghtCounterLoad > 0) {
                channelPulse1.timerHi.lenghtCounterLoad--;
                pulseWave.unmute();
            } else {
                if (pulseWave.isPlaying()) {
                    pulseWave.mute();
                }
            }
        }
/*
        if (!((channelPulse2Status & 0x20) >> 5)) {

        }

        if (channelTriangleLinearCounter >> 7) {

        }*/
    }

    counter++;
}

void apu2A03::reset() {
    apuStatus.rawData = 0x00;
    frameCounterData = 0x00;
    sequencer = 0x00;
    pulseWave.play();
    cpuWrite(0x4015, 0x00);
}

void apu2A03::play() {
    uint16_t frequency, timer;
    float volume, dutyCycle;

    timer = (channelPulse1.timerHi.timerHi << 8) | channelPulse1.timerLo;
    volume = 0x00;

    if (timer >= 8) {
        frequency = cpu6502::cpuSpeed / (16 * (timer + 1));
        volume = 0.1;
        switch (channelPulse1.status.dutyCycle) {
            case 0:
                dutyCycle = 0.125;
                break;

            case 1:
                dutyCycle = 0.25;
                break;

            case 2:
                dutyCycle = 0.50;
                break;

            case 3:
                dutyCycle = 0.75;
                break;
        }
        pulseWave.setGain(0.01f);
        pulseWave.setFrequency(frequency);
        pulseWave.setNbHarmonics(30);
        pulseWave.setDutyCycle(dutyCycle);
    }
}
