// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 06/03/2022.
//

#include "TriangleWave.h"
#include <cmath>

int generateTriangleWave(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime,
                      unsigned int status, void *userData) {

    auto *buffer = (double *) outputBuffer;
    auto *pSettings = (TriangleWave::settings *) userData;

    if ( status )
        printf("Stream underflow detected!");

    for (unsigned int i = 0; i < nBufferFrames; i++) {
        for (int h = 1; h < 16; h++) {
            *buffer += pSettings->gain * (pow(-1, (float)h) / pow(2 * (float)h - 1, 2)) * sin(2 * M_PI * pSettings->frequency * (2 * (float)h - 1) * streamTime);
        }
        *buffer++ *= -(8 / (float)pow(M_PI, 2));
    }

    return 0;
}

TriangleWave::TriangleWave() {
    this->sampleRate = DEFAULT_SAMPLE_RATE;

    this->parameters.deviceId = this->audio.getDefaultOutputDevice();
    this->parameters.nChannels = 2;

    this->audio.openStream(&parameters,
                           nullptr,
                           RTAUDIO_FLOAT64,
                           this->sampleRate,
                           &bufferFrames,
                           &generateTriangleWave,
                           &data);
}

TriangleWave::TriangleWave(uint16_t sampleRate) {
    this->sampleRate = sampleRate;

    this->parameters.deviceId = this->audio.getDefaultOutputDevice();
    this->parameters.nChannels = 2;

    this->audio.openStream(&parameters,
                           nullptr,
                           RTAUDIO_FLOAT64,
                           this->sampleRate,
                           &bufferFrames,
                           &generateTriangleWave,
                           &data);
}

TriangleWave::~TriangleWave() {
    this->audio.closeStream();
}

void TriangleWave::play() {
    this->audio.startStream();
}

void TriangleWave::stop() {
    this->audio.stopStream();
}

void TriangleWave::setFrequency(uint16_t newFrequency) {
    this->data.frequency = newFrequency;
}

void TriangleWave::setGain(float newGain) {
    this->data.gain = newGain;
}

uint16_t TriangleWave::getFrequency() const {
    return this->data.frequency;
}

float TriangleWave::getGain() const {
    return this->data.gain;
}
