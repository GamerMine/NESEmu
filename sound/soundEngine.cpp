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

soundEngine::Tone soundEngine::generatePulseWave(ALuint freq, ALfloat gain, ALfloat pulseWidth, ALuint sampleRate, ALfloat nbHarmonics) {
    ALuint i, h, buffer;
    ALfloat time, dTime;
    ALfloat *data;

    time = 0;
    dTime = 1 / (ALfloat)sampleRate;
    data = generateDataBuffer(sampleRate);

    for (i = 0; i < sampleRate; i++) {
        ALfloat value;
        for (h = 1; h < nbHarmonics; h++) {
            value += (ALfloat)((1 / (ALfloat)h) * sin(M_PI * (ALfloat)h * pulseWidth) * cos(2 * M_PI * (ALfloat)h * (ALfloat)freq * time));
        }
        value *= (ALfloat)(gain * pulseWidth + ((2 * gain) / M_PI));
        data[i] = value;
        time += dTime;
    }

    buffer = generateBuffer(data, sampleRate);

    return {soundEngine::PULSE_WAVE, buffer, sampleRate};
}

bool soundEngine::playTone(soundEngine::Tone tone) {
    ALuint source;

    source = 0;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, (ALint)tone.buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcePlay(source);

    return true;
}

ALfloat* soundEngine::generateDataBuffer(ALuint sampleRate) {
    ALuint dataSize;
    ALfloat *data;

    dataSize = (ALuint)(sampleRate * sizeof(ALfloat));
    data = (ALfloat*) calloc(1, dataSize);

    return data;
}

ALuint soundEngine::generateBuffer(ALfloat *data, ALuint sampleRate) {
    ALuint buffer, dataSize;

    dataSize = (ALuint)(sampleRate * sizeof(ALfloat));

    buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO_FLOAT32, data, (ALsizei)dataSize, (ALsizei)sampleRate);

    return buffer;
}

soundEngine::Tone::Tone(soundEngine::TONETYPE tonetype, ALuint buffer, ALuint sampleRate) {
    this->tonetype = tonetype;
    this->buffer = buffer;
    this->sampleRate = sampleRate;
}
