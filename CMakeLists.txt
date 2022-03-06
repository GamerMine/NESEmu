cmake_minimum_required(VERSION 3.21)
project(NES_Emu)

set(CMAKE_CXX_STANDARD 23)
SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

LINK_LIBRARIES(X11 GL png openal asound rtaudio)

add_executable(NES_EmuD Bus.cpp Bus.h cpu6502.cpp cpu6502.h debug/cpuDebug.cpp debug/cpuDebug.h ppuRP2C02.cpp ppuRP2C02.h Gamepak.cpp Gamepak.h Mapper.cpp Mapper.h mappers/Mapper_000.cpp mappers/Mapper_000.h apu2A03.cpp apu2A03.h sound/PulseWave.cpp sound/PulseWave.h sound/TriangleWave.cpp sound/TriangleWave.h)
add_executable(NES_Emu Bus.cpp Bus.h cpu6502.cpp cpu6502.h bin/NESEmu.h bin/NESEmu.cpp ppuRP2C02.cpp ppuRP2C02.h Gamepak.cpp Gamepak.h Mapper.cpp Mapper.h mappers/Mapper_000.cpp mappers/Mapper_000.h bin/NESEmu.cpp bin/NESEmu.h apu2A03.cpp apu2A03.h sound/PulseWave.cpp sound/PulseWave.h sound/TriangleWave.cpp sound/TriangleWave.h)