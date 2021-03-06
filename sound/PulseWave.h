// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 06/03/2022.
//

#ifndef NESSOUNDENGINE_PULSEWAVE_H
#define NESSOUNDENGINE_PULSEWAVE_H
#define DEFAULT_SAMPLE_RATE 44100

#include <cstdint>
#include <rtaudio/RtAudio.h>

class PulseWave {
public:
    PulseWave();
    PulseWave(uint16_t sampleRate);
    ~PulseWave();

    void play();
    void stop();

    void setFrequency(uint16_t newFrequency);
    void setNbHarmonics(float newNbHarmonics);
    void setGain(float newGain);
    void setDutyCycle(float newDutyCycle);

    void mute();
    void unmute();

    uint16_t getFrequency() const;
    float getNbHarmonics() const;
    float getGain() const;
    float getDutyCycle() const;

    bool isPlaying() const;

public:
    typedef struct settings {
        uint16_t frequency;
        float nbHarmonics;
        float gain;
        float dutyCycle;
        uint8_t mute;
    } settings;

private:
    RtAudio audio;
    RtAudio::StreamParameters parameters;
    uint16_t sampleRate;
    unsigned int bufferFrames = 2;
    settings data = {0, 0, 0, 0, 0};
    bool playing = false;
};


#endif //NESSOUNDENGINE_PULSEWAVE_H
