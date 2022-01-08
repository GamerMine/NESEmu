//
// Created by maxime on 27/12/2021.
//

#ifndef NES_EMU_MAPPER_000_H
#define NES_EMU_MAPPER_000_H


#include "../Mapper.h"

class Mapper_000 : public Mapper {
public:
    Mapper_000(uint8_t prgRomUnits, uint8_t chrUnits);
    ~Mapper_000();

public:
    bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
    bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
};


#endif //NES_EMU_MAPPER_000_H
