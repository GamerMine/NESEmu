//
// Created by maxime on 27/12/2021.
//

#include "Mapper_000.h"

// NOTE: Even if the CPU is addressing things to the ROM starting from 0x6000, actually the ROM starts at 0x0000

Mapper_000::Mapper_000(uint8_t prgRomUnits, uint8_t chrUnits) : Mapper(prgRomUnits, chrUnits) {

}

Mapper_000::~Mapper_000() = default;

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x6000 && addr <= 0x7FFF) { // For Family Basic only, there is a RAM located between 0x6000 and 0x7FFF.
        mapped_addr = addr & 0x1FFF;
        return true;
    } else if (addr >= 0x8000 && addr <= 0xFFFF) { // We can read the PRG ROMs from 0x8000 to 0xFFFF
        // This depends on the number of 16KB units. It can be 1 16KB or 2 16KB ROMs.
        // If there is only 1 16KB ROM, then we mirror the first 16KB between 0x8000 and 0xBFFF
        mapped_addr = addr & (numPRGUnits > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x6000 && addr <= 0x7FFF) { // For Family Basic only, there is a RAM located between 0x6000 and 0x7FFF.
        mapped_addr = addr & 0x1FFF;
        return true;
    }
    return false;
}

bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr) {
    // No bank-switching, the mapped address is the same as the provided address.
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
    // Since this is necessarily ROMs (Read-Only Memory), we can't write to them.
    return false;
}