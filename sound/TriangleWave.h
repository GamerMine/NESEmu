// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 06/03/2022.
//

#ifndef NESSOUNDENGINE_TRIANGLEWAVE_H
#define NESSOUNDENGINE_TRIANGLEWAVE_H
#define DEFAULT_SAMPLE_RATE 44100

#include <cstdint>
#include <rtaudio/RtAudio.h>

class TriangleWave {
public:
    TriangleWave();
    TriangleWave(uint16_t sampleRate);
    ~TriangleWave();

    void play();
    void stop();

    void setFrequency(uint16_t newFrequency);
    void setGain(float newGain);

    uint16_t getFrequency() const;
    float getGain() const;

public:
    typedef struct settings {
        uint16_t frequency;
        float gain;
    } settings;

private:
    RtAudio audio;
    RtAudio::StreamParameters parameters;
    uint16_t sampleRate;
    unsigned int bufferFrames = 2;
    settings data = {0, 0};
};


#endif //NESSOUNDENGINE_TRIANGLEWAVE_H
