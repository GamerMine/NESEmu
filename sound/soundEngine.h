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

    class Tone {
    public:
        ALuint buffer;
        ALuint sampleRate;
        ALuint source;
        ALfloat *data;
        ALuint dataSize;

    public:
        Tone();
        Tone(ALuint sampleRate);
    };

public:
    static void generatePulseWave(Tone tone, ALuint freq, ALfloat gain, ALfloat dutyCycle, ALfloat nbHarmonics);
    static void generateTriangleWave(Tone tone, ALuint freq, ALfloat gain);
    static void generateNoise(Tone tone, ALfloat gain);

    static bool playTone(Tone tone, bool loop);
    static void stopTone(Tone tone);

};


#endif //NES_EMU_SOUNDENGINE_H
