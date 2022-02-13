// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 20/12/2021.
//

#ifndef NES_EMU_CPU6502_H
#define NES_EMU_CPU6502_H

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>

class Bus;

// The NES CPU (A modified version from the MOS 6502) is one of the main component of the NES. The CPU has his own bus
// that communicate with the PPU, APU, Controls, Cartridge etc...
class cpu6502 {

public:
    cpu6502();
    ~cpu6502();

    void connectBus(Bus *n) { bus = n; } // Connect to an already existing bus

public:
    // 6502 cpu Flags
    enum FLAGS {
        C = (1 << 0), // Carry              (bit 0)
        Z = (1 << 1), // Zero               (bit 1)
        I = (1 << 2), // Interrupt disable  (bit 2)
        D = (1 << 3), // Decimal            (bit 3) Not implemented because on the NES it has no effects
        B = (1 << 4), // Break              (bit 4)
        U = (1 << 5), // Unused             (bit 5)
        V = (1 << 6), // Overflow           (bit 6)
        N = (1 << 7)  // Negative           (bit 7)
    };

    // 6502 cpu Registers
    uint16_t pc = 0x0000; // Program Counter
    uint8_t ac = 0x00; // Accumulator
    uint8_t x = 0x00; // X register
    uint8_t y = 0x00; // Y register
    uint8_t sr = 0x34; // Status Register
    uint8_t sp = 0xFD; // Stack Pointer

    // 6502 cpu Addressing Modes
    bool ACC(); // Accumulator mode
    bool ABS(); // Absolute mode
    bool ABX(); // Absolute, X-Indexed mode
    bool ABY(); // Absolute, Y-indexed mode
    bool IMM(); // Immediate mode
    bool IMP(); // Implied mode
    bool IND(); // Indirect mode
    bool IDX(); // X-Indexed, Indirect mode
    bool IDY(); // Indirect, Y-Indexed mode
    bool REL(); // Relative
    bool ZPG(); // ZeroPage mode
    bool ZPX(); // ZeroPage, X-Indexed mode
    bool ZPY(); // ZeroPage, Y-Indexed mode

    // 6502 Operations Code
    bool ADC(); // Add with Carry
    bool AND(); // AND (with Accumulator)
    bool ASL(); // Arithmetic shift left
    bool BCC(); // Branch on carry clear
    bool BCS(); // Branch on carry set
    bool BEQ(); // Branch on equal (zero set)
    bool BIT(); // Bit test
    bool BMI(); // Branch on minus (negative set)
    bool BNE(); // Branch on not equal (zero clear)
    bool BPL(); // Branch on plus (negative clear)
    bool BRK(); // Break / Interrupt
    bool BVC(); // Branch on overflow clear
    bool BVS(); // Branch on overflow set
    bool CLC(); // Clear carry
    bool CLD(); // Clear decimal
    bool CLI(); // Clear interrupt disable
    bool CLV(); // Clear overflow
    bool CMP(); // Compare (with accumulator)
    bool CPX(); // Compare with X
    bool CPY(); // Compare with Y
    bool DEC(); // Decrement
    bool DEX(); // Decrement X
    bool DEY(); // Decrement Y
    bool EOR(); // Exclusive or (with accumulator)
    bool INC(); // Increment
    bool INX(); // Increment X
    bool INY(); // Increment Y
    bool JMP(); // Jump
    bool JSR(); // jump subroutine
    bool LDA(); // Load Accumulator
    bool LDX(); // Load X
    bool LDY(); // Load Y
    bool LSR(); // Logical shift right
    bool NOP(); // No operation
    bool ORA(); // Or with accumulator
    bool PHA(); // Push accumulator
    bool PHP(); // Push processor status (SR)
    bool PLA(); // Pull accumulator
    bool PLP(); // Pull processor status (SR)
    bool ROL(); // Rotate left
    bool ROR(); // Rotate right
    bool RTI(); // Return from interrupt
    bool RTS(); // Return from subroutine
    bool SBC(); // Subtract with carry
    bool SEC(); // Set carry
    bool SED(); // Set decimal
    bool SEI(); // Set interrupt disable
    bool STA(); // Store Accumulator
    bool STX(); // Store X
    bool STY(); // Store Y
    bool TAX(); // Transfer Accumulator to X
    bool TAY(); // Transfer Accumulator to Y
    bool TSX(); // Transfer Stack pointer to X
    bool TXA(); // Transfer X to accumulator
    bool TXS(); // Transfer X to stack pointer
    bool TYA(); // Transfer Y to Accumulator

    bool ILL(); // Illegal operation (this is not original from the 6502, this is error handling)

    // Other signals
    void clock(); // Run the CPU for 1 clock cycle
    void reset(); // Reset the cpu
    void irq(); // Interrupt Request (Depend on the Interrupt disable flag)
    void nmi(); // Non-maskable Interrupt (Does not depend on Interrupt disable flag)

    // Internal utility functions (For 6502 implementation only)
    uint8_t fetch(); // Fetch data
    uint8_t fetched = 0x00; // Contains data that was fetched previously

    uint16_t addr_abs = 0x0000; // If we want to get data from an absolute address
    uint16_t addr_rel = 0x0000; // if we want to get data from a relative address

    uint8_t opcode = 0x00; // Contains the current Operation Code that is being executed
    uint8_t cycles = 0x00; // Contains the current cycle

    bool complete();

    std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);

private:
    Bus *bus = nullptr;

    void write(uint16_t addr, uint8_t data); // Write into the bus
    uint8_t read(uint16_t addr); // Read from the bus

    void setFlag(FLAGS flag, bool value); // Set a flag to the specified value (0 or 1)
    bool getFlag(FLAGS flag) const; // Get the value of the specified flag

    struct INSTRUCTION {
        std::string name; // Name
        bool(cpu6502::*operate)() = nullptr; // Operation Code
        bool(cpu6502::*addrmode)() = nullptr; // Addressing mode
        uint8_t cycles = 0; // Required number of cycles
    };

    std::vector<INSTRUCTION> instructions;
};


#endif //NES_EMU_CPU6502_H
