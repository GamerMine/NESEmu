//
// Created by maxime on 20/12/2021.
//

#include "Bus.h"

Bus::Bus() {
    // Temp RAM initialization
    for (unsigned char & i : cpuRAM) {
        i = 0x00;
    }

    cpu.connectBus(this);
    ppu.connectBus(this);
}

Bus::~Bus() = default;

// NOTE : The RAM is mirrored. Actually the RAM is capable of storing 2KB of data but is addressable from 0x0000 to 0x1FFF.
// It is the reason for that (addr & 0x07FF) i.e. 0x07FF = 2047 (0-2047 so 2048 values = 2KB). Each time we are accessing
// the cpuRAM with higher values than 0x07FF, it is 'converted' to the range 0x0000-0x07FF.
void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    // The Cartridge will intercept all the address that the Mapper accept
    if (gamepak->cpuWrite(addr, data)) {

    } else if (addr >= 0x0000 && addr <= 0x1FFF) {
        cpuRAM[addr & 0x07FF] = data;
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        ppu.cpuWrite(addr & 0x0007, data);
    }
}

uint8_t Bus::cpuRead(uint16_t addr) {
    uint8_t data = 0;
    if (gamepak->cpuRead(addr, data)) {

    } else if (addr >= 0x0000 && addr <= 0x1FFF) {
        return cpuRAM[addr & 0x07FF];
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        ppu.cpuRead(addr & 0x0007);
    }

    return data;
}

void Bus::insertGamepak(const std::shared_ptr<Gamepak>& gpk) {
    this->gamepak = gpk;
    ppu.connectGamepak(gpk);
}

void Bus::reset() {
    cpu.reset();
    clockCounter = 0;
}

void Bus::clock() {
    ppu.clock();
    if (clockCounter % 3 == 0) {
        cpu.clock();
    }
    clockCounter++;
}
