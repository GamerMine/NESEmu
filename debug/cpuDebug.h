//
// Created by maxime on 24/12/2021.
//

#ifndef NES_EMU_CPUDEBUG_H
#define NES_EMU_CPUDEBUG_H
#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"
#include "../Bus.h"

class cpuDebug : public olc::PixelGameEngine{
public:
    cpuDebug();

    Bus nes;
    std::map<uint16_t, std::string> mapAsm;
    std::shared_ptr<Gamepak> gamepak;

    bool run = false;
    float residualTime = 0.0f;
    uint8_t currentPage = 0x00;
    uint8_t selectedPalette = 0x00;
    const int swatchSize = 6;

    static std::string toHex(uint32_t number, uint8_t numDigits);

    void drawRam(int x, int y, uint16_t numAddr, int numRows, int numColumns);
    void drawCpu(int x, int y);
    void drawCode(int x, int y, int numLines);

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

};


#endif //NES_EMU_CPUDEBUG_H
