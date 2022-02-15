// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 20/12/2021.
//

#ifndef NES_EMU_BUS_H
#define NES_EMU_BUS_H

#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include "cpu6502.h"
#include "ppuRP2C02.h"
#include "Gamepak.h"

// The Bus is part of the NES, it is here where every device communicate to each other.
// This is not mandatory to make this object but ut make the code more readable and understandable.
// Also, it avoids sending data directly to one component and being confused.
class Bus {
public:
    Bus();
    ~Bus();

public: // Devices connected to the bus
    cpu6502 cpu;
    ppuRP2C02 ppu;

    std::array<uint8_t, 2 * 1024> cpuRAM{}; // 1 byte = 1024 bits and the NES is equipped with 2KB of CPU RAM

    std::shared_ptr<Gamepak> gamepak;

    uint8_t controllers[2];

public: // Read and Write data on the bus

    // CPU BUS
    // The CPU has his own bus
    void cpuWrite(uint16_t addr, uint8_t data); // Write 8 bits of data at the 16 bits address
    uint8_t cpuRead(uint16_t addr); // Read data at the specified 16 bits address

public: // System Interface
    void insertGamepak(const std::shared_ptr<Gamepak>& gamepak); // We can insert a Cartridge into the NES
    void reset(); // The NES has a button to entirely reset the console
    void clock(); // This function is called to cycle 1 time

private:
    uint32_t clockCounter = 0;

    // This represents the two controllers input of the NES.
    uint8_t controllers_status[2];

    // The DMA is actually on the CPU Bus.
    // The dmaPage (PP) and dmaAddr (AA) represent the full address : 0xPPAA.
    uint8_t dmaPage = 0x00;
    uint8_t dmaAddr = 0x00;
    uint8_t dmaData = 0x00;
    bool isDMAActive = false;
    bool dmaFlag = true;

};


#endif //NES_EMU_BUS_H
