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

soundEngine::Tone soundEngine::generatePulseWave(ALuint freq, ALfloat gain, ALfloat dutyCycle, ALuint sampleRate, ALfloat nbHarmonics) {
    ALuint i, h, buffer, source;
    ALfloat time, dTime;
    ALfloat *data;

    time = 0;
    dTime = 1 / (ALfloat)sampleRate;
    data = generateDataBuffer(sampleRate);

    for (i = 0; i < sampleRate; i++) {
        ALfloat value;
        for (h = 1; h < nbHarmonics; h++) {
            value += (ALfloat)((1 / (ALfloat)h) * sin(M_PI * (ALfloat)h * dutyCycle) * cos(2 * M_PI * (ALfloat)h * (ALfloat)freq * time));
        }
        value *= (ALfloat)(gain * dutyCycle + ((2 * gain) / M_PI));
        data[i] = value;
        time += dTime;
    }

    buffer = generateBuffer(data, sampleRate);
    source = generateSource(buffer);

    return {soundEngine::PULSE_WAVE, buffer, sampleRate, source};
}

soundEngine::Tone soundEngine::generateTriangleWave(ALuint freq, ALfloat gain, ALuint sampleRate) {
    ALuint i, h, buffer, source;
    ALfloat time, dTime;
    ALfloat *data;

    dTime = 1 / (ALfloat)sampleRate;
    time = 0;
    data = generateDataBuffer(sampleRate);

    for (i = 0; i < sampleRate; i++) {
        ALfloat value;
        for (h = 1; h < 16; h++) {
            value += gain * (pow(-1, (ALfloat)h) / pow(2 * (ALfloat)h - 1, 2)) * sin(2 * M_PI * freq * (2 * (ALfloat)h - 1) * time);
        }
        value *= -(8 / (ALfloat)pow(M_PI, 2));
        data[i] = value;
        time += dTime;
    }

    buffer = generateBuffer(data, sampleRate);
    source = generateSource(buffer);

    return {soundEngine::TRIANGLE, buffer, sampleRate, source};
}

soundEngine::Tone soundEngine::generateNoise(ALfloat gain, ALuint sampleRate) {
    ALuint i, h, buffer, source;
    ALfloat time, dTime;
    ALfloat *data;

    time = 0;
    dTime = 1 / (ALfloat)sampleRate;
    data = generateDataBuffer(sampleRate);

    for (i = 0; i < sampleRate; i++) {
        ALfloat value;
        for (h = 1; h < 30; h++) {
            value += (ALfloat)((1 / (ALfloat)h) * sin(M_PI * (ALfloat)h * 0.5) * cos(2 * M_PI * (ALfloat)h * (ALfloat)(random() % (447000 - 29 + 1) + 29) * time));
        }
        value *= (ALfloat)(gain * 0.5 + ((2 * gain) / M_PI));
        data[i] = value;
        time += dTime;
    }

    buffer = generateBuffer(data, sampleRate);
    source = generateSource(buffer);

    return {soundEngine::NOISE, buffer, sampleRate, source};
}

bool soundEngine::playTone(soundEngine::Tone tone, bool loop) {
    alSourcei(tone.source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourcePlay(tone.source);

    return true;
}

void soundEngine::stopTone(Tone tone) {
    alSourceStop(tone.source);
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

ALuint soundEngine::generateSource(ALuint buffer) {
    ALuint source;

    source = 0;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, (ALint)buffer);

    return source;
}

soundEngine::Tone::Tone(soundEngine::TONETYPE tonetype, ALuint buffer, ALuint sampleRate, ALuint source) {
    this->tonetype = tonetype;
    this->buffer = buffer;
    this->sampleRate = sampleRate;
    this->source = source;
}
