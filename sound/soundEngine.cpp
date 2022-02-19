// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 18/02/2022.
//

#include "soundEngine.h"
#include <stdexcept>
#include <cmath>

soundEngine::soundEngine() = default;

soundEngine::~soundEngine() = default;

bool soundEngine::initializeEngine(){

    audioDevice = alcOpenDevice(alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER));
    if (!audioDevice) {
        throw std::runtime_error("Unable to open the audio device.");
        return false;
    }

    audioContext = alcCreateContext(audioDevice, nullptr);
    if (!audioContext) {
        throw std::runtime_error("Unable to create the audio context.");
        return false;
    }

    alcMakeContextCurrent(audioContext);
    return true;
}

void soundEngine::destroyEngine() const {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}

ALfloat soundEngine::generateSquareWave(ALfloat* data, ALuint frequency, ALuint harmonicNb, ALfloat gain) {
    // Using the Gibbs phenomenon
    ALfloat value;

    for (ALuint i = 0; i < harmonicNb; i++) {
        value += gain * (sin((2 * harmonicNb + 1) * 2 * M_PI * frequency) / (2 * harmonicNb + 1));
    }
    value *= (4 / M_PI);

    printf("Actual value: %f\n", value);

    return value;
}

ALuint soundEngine::generateTone(soundEngine::TONETYPE tonetype, ALuint frequency, ALuint sampleRate, ALfloat gain) {
    const ALuint nbHarmonics = 20;

    ALuint buffer;
    ALuint dataSize;
    ALfloat *data;

    switch (tonetype) {
        case TONETYPE::SQUARE:
            generateSquareWave(data, frequency, nbHarmonics, gain);
            break;

        case TONETYPE::TRIANGLE:
            break;

        case TONETYPE::NOISE:
            break;

        default:
            throw std::runtime_error("Invalid tone type.");
            return 0;
    }

    buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16, data, nbHarmonics * sizeof(ALfloat), nbHarmonics);

    ALenum errors = alGetError();
    if (errors != AL_NO_ERROR) {
        printf("Error!!");
        if (alIsBuffer(buffer)) {
            alDeleteBuffers(1, &buffer);
        }
        return 0;
    }

    ALuint source = 0;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcePlay(source);

    return buffer;
}

