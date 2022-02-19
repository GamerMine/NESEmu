// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 18/02/2022.
//

#ifndef NES_EMU_SOUNDENGINE_H
#define NES_EMU_SOUNDENGINE_H

#include <AL/alc.h>
#include <AL/al.h>

class soundEngine {

public:
    soundEngine();
    ~soundEngine();

    bool initializeEngine();
    void destroyEngine() const;

public:

    ALCdevice* audioDevice{};
    ALCcontext* audioContext{};

    enum TONETYPE {
        PULSE_WAVE,
        TRIANGLE,
        NOISE,
    };

    class Tone {
    public:
        TONETYPE tonetype;
        ALuint buffer;
        ALuint sampleRate;

    public:
        Tone(TONETYPE tonetype, ALuint buffer, ALuint sampleRate);
    };

public:
    static Tone generatePulseWave(ALuint freq, ALfloat gain, ALfloat pulseWidth, ALuint sampleRate, ALfloat nbHarmonics);

    static bool playTone(Tone tone);

private:
    static ALfloat* generateDataBuffer(ALuint sampleRate);
    static ALuint generateBuffer(ALfloat *data, ALuint sampleRate);

};


#endif //NES_EMU_SOUNDENGINE_H
