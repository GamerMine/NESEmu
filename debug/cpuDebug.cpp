// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "cpuDebug.h"
#include "../sound/soundEngine.h"

cpuDebug::cpuDebug() {
    sAppName = "NES 6502 Debugger"; // The window name
}

// Convert a number into hexadecimal string
// number : The number to convert
// numDigits : The number of hexadecimal numbers to keep
std::string cpuDebug::toHex(uint32_t number, uint8_t numDigits) {
    std::string s(numDigits, '0');
    for (int i = numDigits - 1; i >= 0; i--, number >>= 4)
        s[i] = "0123456789ABCDEF"[number & 0xF];
    return s;
}

// Display the RAM dump of the nes at the x and y location
// x : The X location
// y : The Y location
// numAddr : The number of addresses that will be displayed
// numRows : Number of rows
// numColumns : Number of columns
void cpuDebug::drawRam(int x, int y, uint16_t numAddr, int numRows, int numColumns) {
    int nRamX = x, nRamY = y;
    for (int row = 0; row < numRows; row++)
    {
        std::string sOffset = "$" + toHex(numAddr, 4) + ":";
        for (int col = 0; col < numColumns; col++)
        {
            sOffset += " " + toHex(nes.cpuRead(numAddr), 2);
            numAddr += 1;
        }
        DrawString(nRamX, nRamY, sOffset);
        nRamY += 10;
    }
}

// Display the CPU flags and registers at the x and y locations
// x : The X location
// y : The Y location
void cpuDebug::drawCpu(int x, int y) {
    std::string status;
    DrawString(x , y , "STATUS:", olc::WHITE);
    DrawString(x  + 64, y, "N", nes.cpu.sr & cpu6502::N ? olc::GREEN : olc::RED);
    DrawString(x  + 80, y , "V", nes.cpu.sr & cpu6502::V ? olc::GREEN : olc::RED);
    DrawString(x  + 96, y , "-", nes.cpu.sr & cpu6502::U ? olc::GREEN : olc::RED);
    DrawString(x + 112, y, "B", nes.cpu.sr & cpu6502::B ? olc::GREEN : olc::RED);
    DrawString(x  + 128, y , "D", nes.cpu.sr & cpu6502::D ? olc::GREEN : olc::RED);
    DrawString(x  + 144, y , "I", nes.cpu.sr & cpu6502::I ? olc::GREEN : olc::RED);
    DrawString(x  + 160, y , "Z", nes.cpu.sr & cpu6502::Z ? olc::GREEN : olc::RED);
    DrawString(x  + 178, y , "C", nes.cpu.sr & cpu6502::C ? olc::GREEN : olc::RED);
    DrawString(x , y + 10, "PC: $" + toHex(nes.cpu.pc, 4));
    DrawString(x , y + 20, "A: $" + toHex(nes.cpu.ac, 2) + "  [" + std::to_string(nes.cpu.ac) + "]");
    DrawString(x , y + 30, "X: $" + toHex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
    DrawString(x , y + 40, "Y: $" + toHex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
    DrawString(x , y + 50, "Stack P: $" + toHex(nes.cpu.sp, 4));
}

// Draw the current program that is being executed at the x and y locations
// x : The x location
// y : The y location
// numLines : The number of lines to show
void cpuDebug::drawCode(int x, int y, int numLines) {
    auto it_a = mapAsm.find(nes.cpu.pc);
    int nLineY = (numLines >> 1) * 10 + y;
    if (it_a != mapAsm.end())
    {
        DrawString(x, nLineY, (*it_a).second, olc::CYAN);
        while (nLineY < (numLines * 10) + y)
        {
            nLineY += 10;
            if (++it_a != mapAsm.end())
            {
                DrawString(x, nLineY, (*it_a).second);
            }
        }
    }

    it_a = mapAsm.find(nes.cpu.pc);
    nLineY = (numLines >> 1) * 10 + y;
    if (it_a != mapAsm.end())
    {
        while (nLineY > y)
        {
            nLineY -= 10;
            if (--it_a != mapAsm.end())
            {
                DrawString(x, nLineY, (*it_a).second);
            }
        }
    }
}

void cpuDebug::drawOAM(int x, int y) {
    for (int i = 0; i < 26; i++) {
        std::string s = toHex(i, 2) + ": (" + std::to_string(nes.ppu.oam[i * 4 + 3])
                + ", " + std::to_string(nes.ppu.oam[i * 4]) + ") "
                + "ID: " + toHex(nes.ppu.oam[i * 4 + 1], 2) + " "
                + "ATT: " + toHex(nes.ppu.oam[i * 4 + 2], 2);

        DrawString(x, y + i * 10, s);
    }
}

bool cpuDebug::OnUserCreate() {
    gamepak = std::make_shared<Gamepak>("mspacman.nes");

    nes.insertGamepak(gamepak);

    mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

    std::ofstream output("decompASM.txt", std::ofstream::out);
    std::map<uint16_t, std::string>::iterator it;
    for (it = mapAsm.begin(); it != mapAsm.end(); it++) {
        output << it->second.c_str() << "\n";
    }
    output.close();

    audioEngine.initializeEngine();
    audioEngine.generateTone(soundEngine::SQUARE, 440, 0, 1);

    nes.reset();

    return true;
}

bool cpuDebug::OnUserUpdate(float fElapsedTime) {
    Clear(olc::DARK_BLUE);

    nes.controllers[0] = 0x00;
    nes.controllers[0] |= GetKey(olc::Key::A).bHeld ? 0x80 : 0x00;      // A key
    nes.controllers[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;      // B key
    nes.controllers[0] |= GetKey(olc::Key::W).bHeld ? 0x20 : 0x00;      // Select key
    nes.controllers[0] |= GetKey(olc::Key::X).bHeld ? 0x10 : 0x00;      // Start key
    nes.controllers[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;     // Up key
    nes.controllers[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;   // Down key
    nes.controllers[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;   // Left key
    nes.controllers[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;  // Right key

    if (run) {
        if (residualTime > 0.0f) {
            residualTime -= fElapsedTime;
        } else {
            residualTime += (1.0f / 60.0f) - fElapsedTime;
            do {
                nes.clock();
            } while (!nes.ppu.frameComplete);
            nes.ppu.frameComplete = false;
        }
    } else {
        if (GetKey(olc::Key::C).bPressed) {
            do {
                nes.clock();
            } while (nes.cpu.complete());
            do {
                nes.clock();
            } while (nes.cpu.complete());
        }
    }

    if (GetKey(olc::Key::H).bPressed) {
        if (hideDebug) {
            SetScreenSize(780, 480);
            olc_UpdateViewport();
        } else {
            SetScreenSize(480, 480);
            olc_UpdateViewport();
        }
        hideDebug = !hideDebug;
    }

    if (hideDebug) {
        DrawSprite(0, 0, &nes.ppu.getScreen(), 2);
    } else {

        if (GetKey(olc::Key::R).bPressed) {
            nes.reset();
        }
        if (GetKey(olc::Key::F).bPressed) {
            do {
                nes.clock();
            } while (!nes.ppu.frameComplete);
            do {
                nes.clock();
            } while (nes.cpu.complete());
            nes.ppu.frameComplete = false;
        }
        if (GetKey(olc::Key::SPACE).bPressed) {
            run = !run;
        }
        if (GetKey(olc::Key::N).bPressed) {
            if (currentPage >= 1) {
                currentPage = 0;
            } else {
                currentPage++;
            }
        }

        if (GetKey(olc::Key::P).bPressed) {
            ++selectedPalette &= 0x07;
        }

        if (GetKey(olc::Key::O).bPressed) {
            showOAM = !showOAM;
        }

        switch (currentPage) {
            case 0:
                drawCpu(516, 2);
                if (showOAM) {
                    drawOAM(516, 72);
                } else {
                    drawCode(516, 72, 26);
                }

                for (int p = 0; p < 8; p++) {
                    for (int s = 0; s < 4; s++) {
                        FillRect(516 + p * (swatchSize * 5) + s * swatchSize, 340, swatchSize, swatchSize,
                                 nes.ppu.getColor(p, s));
                    }
                }
                DrawRect(516 + selectedPalette * (swatchSize * 5) - 1, 339, (swatchSize * 4), swatchSize, olc::WHITE);

                DrawSprite(516, 348, &nes.ppu.getPatternTable(0, selectedPalette));
                DrawSprite(648, 348, &nes.ppu.getPatternTable(1, selectedPalette));
                //nes.ppu.printPaletteRAM();

                DrawSprite(0, 0, &nes.ppu.getScreen(), 2);

                // DEBUG
                /*olc::Sprite& pat = nes.ppu.getPatternTable(1, selectedPalette);
                for (uint8_t y = 0; y < 30; y++) {
                    for (uint8_t x = 0; x < 32; x++) {
                        uint8_t id = (uint32_t)nes.ppu.ciram[0][y * 32 + x];
                        DrawPartialSprite(x * 16, y * 16, &pat, (id & 0x0F) << 3, ((id >> 4) & 0x0F) << 3, 8, 8, 2);
                    }
                }*/

                break;
            case 1:
                drawRam(0, 0, 0x0000, 47, 24);
        }
    }

    return true;
}

bool cpuDebug::OnUserDestroy() {
    audioEngine.destroyEngine();
    return true;
}

int main()
{
    cpuDebug demo;
    demo.Construct(780, 480, 2, 2);
    demo.Start();
    return 0;
}