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
#include "AL/alext.h"

soundEngine::soundEngine() = default;

soundEngine::~soundEngine() = default;

bool soundEngine::initializeEngine(){

    audioDevice = alcOpenDevice(nullptr);
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

void soundEngine::generatePulseWave(soundEngine::Tone tone, ALuint freq, ALfloat gain, ALfloat dutyCycle, ALfloat nbHarmonics) {
    ALuint i, h;
    ALfloat time, dTime;

    time = 0;
    dTime = 1 / (ALfloat)tone.sampleRate;

    for (i = 0; i < tone.sampleRate; i++) {
        ALfloat value;
        for (h = 1; h < nbHarmonics; h++) {
            value += (ALfloat)((1 / (ALfloat)h) * sin(M_PI * (ALfloat)h * dutyCycle) * sin(2 * M_PI * (ALfloat)h * (ALfloat)freq * time));
        }
        value *= (ALfloat)(gain * dutyCycle + ((2 * gain) / M_PI));
        tone.data[i] = value;
        time += dTime;
    }

    alBufferData(tone.buffer, AL_FORMAT_MONO_FLOAT32, tone.data, (ALsizei)tone.dataSize, (ALsizei)tone.sampleRate);
    alSourcei(tone.source, AL_BUFFER, (ALint)tone.buffer);
}

void soundEngine::generateTriangleWave(soundEngine::Tone tone, ALuint freq, ALfloat gain) {
    ALuint i, h;
    ALfloat time, dTime;

    dTime = 1 / (ALfloat)tone.sampleRate;
    time = 0;

    for (i = 0; i < tone.sampleRate; i++) {
        ALfloat value;
        for (h = 1; h < 16; h++) {
            value += gain * (pow(-1, (ALfloat)h) / pow(2 * (ALfloat)h - 1, 2)) * sin(2 * M_PI * freq * (2 * (ALfloat)h - 1) * time);
        }
        value *= -(8 / (ALfloat)pow(M_PI, 2));
        tone.data[i] = value;
        time += dTime;
    }

    alBufferData(tone.buffer, AL_FORMAT_MONO_FLOAT32, tone.data, (ALsizei)tone.dataSize, (ALsizei)tone.sampleRate);
    alSourcei(tone.source, AL_BUFFER, (ALint)tone.buffer);
}

void soundEngine::generateNoise(soundEngine::Tone tone, ALfloat gain) {
    ALuint i, h;
    ALfloat time, dTime;

    time = 0;
    dTime = 1 / (ALfloat)tone.sampleRate;

    for (i = 0; i < tone.sampleRate; i++) {
        ALfloat value;
        for (h = 1; h < 30; h++) {
            value += (ALfloat)((1 / (ALfloat)h) * sin(M_PI * (ALfloat)h * 0.5) * cos(2 * M_PI * (ALfloat)h * (ALfloat)(random() % (447000 - 29 + 1) + 29) * time));
        }
        value *= (ALfloat)(gain * 0.5 + ((2 * gain) / M_PI));
        tone.data[i] = value;
        time += dTime;
    }

    alBufferData(tone.buffer, AL_FORMAT_MONO_FLOAT32, tone.data, (ALsizei)tone.dataSize, (ALsizei)tone.sampleRate);
    alSourcei(tone.source, AL_BUFFER, (ALint)tone.buffer);
}

bool soundEngine::playTone(soundEngine::Tone tone, bool loop) {
    alSourcei(tone.source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourceStop(tone.source);
    alSourcePlay(tone.source);

    return true;
}

void soundEngine::stopTone(Tone tone) {
    alSourceStop(tone.source);
}

soundEngine::Tone::Tone() {
    this->buffer = 0;
    this->source = 0;
    this->sampleRate = 44100;
    this->dataSize = (ALuint)(this->sampleRate * sizeof(ALfloat));
    this->data = (ALfloat*) calloc(1, this->dataSize);

    alGenBuffers(1, &this->buffer);
    alGenSources(1, &this->source);

}

soundEngine::Tone::Tone(ALuint sampleRate) {
    this->buffer = 0;
    this->source = 0;
    this->sampleRate = sampleRate;
    this->dataSize = (ALuint)(this->sampleRate * sizeof(ALfloat));
    this->data = (ALfloat*) calloc(1, this->dataSize);

    alGenBuffers(1, &this->buffer);
    alGenSources(1, &this->source);
}
