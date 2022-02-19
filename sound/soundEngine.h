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
    enum TONETYPE {
        SQUARE,
        TRIANGLE,
        NOISE,
    };

    ALCdevice* audioDevice{};
    ALCcontext* audioContext{};

    // https://github.com/kcat/openal-soft/blob/master/examples/altonegen.c
    ALuint generateTone(enum TONETYPE tonetype, ALuint frequency, ALuint sampleRate, ALfloat gain);

private:
    ALfloat generateSquareWave(ALfloat* data, ALuint frequency, ALuint harmonicNb, ALfloat gain);

};


#endif //NES_EMU_SOUNDENGINE_H
