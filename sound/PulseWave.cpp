// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 06/03/2022.
//

#include <cmath>
#include "PulseWave.h"

int generatePulseWave(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime,
                                 unsigned int status, void *userData) {

    auto *buffer = (double *) outputBuffer;
    auto *pSettings = (PulseWave::settings *) userData;

    if ( status )
        printf("Stream underflow detected!");

    for (unsigned int i = 0; i < nBufferFrames; i++) {
        for (int h = 1; h < pSettings->nbHarmonics; h++) {
            *buffer += (float)((1 / (float)h) * sin(M_PI * (float)h * pSettings->dutyCycle) * sin(2 * M_PI * (float)h * pSettings->frequency * streamTime));
        }
        *buffer++ *= (float)(pSettings->gain * pSettings->dutyCycle + ((2 * pSettings->gain) / M_PI));
    }

    return 0;
}

PulseWave::PulseWave() {
    this->sampleRate = DEFAULT_SAMPLE_RATE;

    this->parameters.deviceId = this->audio.getDefaultOutputDevice();
    this->parameters.nChannels = 2;

    this->audio.openStream(&parameters,
                           nullptr,
                           RTAUDIO_FLOAT64,
                           this->sampleRate,
                           &bufferFrames,
                           &generatePulseWave,
                           &data);
}

PulseWave::PulseWave(uint16_t sampleRate) {
    this->sampleRate = sampleRate;

    this->parameters.deviceId = this->audio.getDefaultOutputDevice();
    this->parameters.nChannels = 2;

    this->audio.openStream(&parameters,
                           nullptr,
                           RTAUDIO_FLOAT64,
                           this->sampleRate,
                           &bufferFrames,
                           &generatePulseWave,
                           &data);
}

PulseWave::~PulseWave() {
    this->audio.closeStream();
}

void PulseWave::play() {
    this->audio.startStream();
    isPlaying = true;
}

void PulseWave::stop() {
    this->audio.stopStream();
    isPlaying = false;
}

void PulseWave::setFrequency(uint16_t newFrequency) {
    this->data.frequency = newFrequency;
}

void PulseWave::setNbHarmonics(float newNbHarmonics) {
    this->data.nbHarmonics = newNbHarmonics;
}

void PulseWave::setGain(float newGain) {
    this->data.gain = newGain;
}

void PulseWave::setDutyCycle(float newDutyCycle) {
    this->data.dutyCycle = newDutyCycle;
}

uint16_t PulseWave::getFrequency() const {
    return this->data.frequency;
}

float PulseWave::getNbHarmonics() const {
    return this->data.nbHarmonics;
}

float PulseWave::getGain() const {
    return this->data.gain;
}

float PulseWave::getDutyCycle() const {
    return this->data.dutyCycle;
}
