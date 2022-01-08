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

};


#endif //NES_EMU_BUS_H