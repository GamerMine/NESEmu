//
// Created by maxime on 20/12/2021.
//

#include "cpu6502.h"
#include "Bus.h"

cpu6502::cpu6502() {
    using a = cpu6502;
    instructions =
            {
                    {"BRK", &a::BRK, &a::IMP, 7}, {"ORA", &a::ORA, &a::IDX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"ORA", &a::ORA, &a::ZPG, 3}, {"ASL", &a::ASL, &a::ZPG, 5}, {"???", &a::ILL, &a::IMP, 2}, {"PHP", &a::PHP, &a::IMP, 3}, {"ORA", &a::ORA, &a::IMM, 2}, {"ASL", &a::ASL, &a::ACC, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"ORA", &a::ORA, &a::ABS, 4}, {"ASL", &a::ASL, &a::ABS, 6}, {"???", &a::ILL, &a::IMP, 2},
                    {"BPL", &a::BPL, &a::REL, 2}, {"ORA", &a::ORA, &a::IDY, 5}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"ORA", &a::ORA, &a::ZPX, 4}, {"ASL", &a::ASL, &a::ZPX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"CLC", &a::CLC, &a::IMP, 2}, {"ORA", &a::ORA, &a::ABY, 4}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"ORA", &a::ORA, &a::ABX, 4}, {"ASL", &a::ASL, &a::ABX, 7}, {"???", &a::ILL, &a::IMP, 2},
                    {"JSR", &a::JSR, &a::ABS, 6}, {"AND", &a::AND, &a::IDX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"BIT", &a::BIT, &a::ZPG, 3}, {"AND", &a::AND, &a::ZPG, 3}, {"ROL", &a::ROL, &a::ZPG, 5}, {"???", &a::ILL, &a::IMP, 2}, {"PLP", &a::PLP, &a::IMP, 4}, {"AND", &a::AND, &a::IMM, 2}, {"ROL", &a::ROL, &a::ACC, 2}, {"???", &a::ILL, &a::IMP, 2}, {"BIT", &a::BIT, &a::ABS, 4}, {"AND", &a::AND, &a::ABS, 4}, {"ROL", &a::ROL, &a::ABS, 6}, {"???", &a::ILL, &a::IMP, 2},
                    {"BMI", &a::BMI, &a::REL, 2}, {"AND", &a::AND, &a::IDY, 5}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"AND", &a::AND, &a::ZPX, 4}, {"ROL", &a::ROL, &a::ZPX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"SEC", &a::SEC, &a::IMP, 2}, {"AND", &a::AND, &a::ABY, 4}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"AND", &a::AND, &a::ABX, 4}, {"ROL", &a::ROL, &a::ABX, 7}, {"???", &a::ILL, &a::IMP, 2},
                    {"RTI", &a::RTI, &a::IMP, 6}, {"EOR", &a::EOR, &a::IDX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"EOR", &a::EOR, &a::ZPG, 3}, {"LSR", &a::LSR, &a::ZPG, 5}, {"???", &a::ILL, &a::IMP, 2}, {"PHA", &a::PHA, &a::IMP, 3}, {"EOR", &a::EOR, &a::IMM, 2}, {"LSR", &a::LSR, &a::ACC, 2}, {"???", &a::ILL, &a::IMP, 2}, {"JMP", &a::JMP, &a::ABS, 3}, {"EOR", &a::EOR, &a::ABS, 4}, {"LSR", &a::LSR, &a::ABS, 6}, {"???", &a::ILL, &a::IMP, 2},
                    {"BVC", &a::BVC, &a::REL, 2}, {"EOR", &a::EOR, &a::IDY, 5}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"EOR", &a::EOR, &a::ZPX, 4}, {"LSR", &a::LSR, &a::ZPX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"CLI", &a::CLI, &a::IMP, 2}, {"EOR", &a::EOR, &a::ABY, 4}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"EOR", &a::EOR, &a::ABX, 4}, {"LSR", &a::LSR, &a::ABX, 7}, {"???", &a::ILL, &a::IMP, 2},
                    {"RTS", &a::RTS, &a::IMP, 6}, {"ADC", &a::ADC, &a::IDX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"ADC", &a::ADC, &a::ZPG, 3}, {"ROR", &a::ROR, &a::ZPG, 5}, {"???", &a::ILL, &a::IMP, 2}, {"PLA", &a::PLA, &a::IMP, 4}, {"ADC", &a::ADC, &a::IMM, 2}, {"ROR", &a::ROR, &a::ACC, 2}, {"???", &a::ILL, &a::IMP, 2}, {"JMP", &a::JMP, &a::IND, 5}, {"ADC", &a::ADC, &a::ABS, 4}, {"ROR", &a::ROR, &a::ABS, 6}, {"???", &a::ILL, &a::IMP, 2},
                    {"BVS", &a::BVS, &a::REL, 2}, {"ADC", &a::ADC, &a::IDY, 5}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"ADC", &a::ADC, &a::ZPX, 4}, {"ROR", &a::ROR, &a::ZPX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"SEI", &a::SEI, &a::IMP, 2}, {"ADC", &a::ADC, &a::ABY, 4}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"ADC", &a::ADC, &a::ABX, 4}, {"ROR", &a::ROR, &a::ABX, 7}, {"???", &a::ILL, &a::IMP, 2},
                    {"???", &a::ILL, &a::IMP, 2}, {"STA", &a::STA, &a::IDX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"STY", &a::STY, &a::ZPG, 3}, {"STA", &a::STA, &a::ZPG, 3}, {"STX", &a::STX, &a::ZPG, 3}, {"???", &a::ILL, &a::IMP, 2}, {"DEY", &a::DEY, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"TXA", &a::TXA, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"STY", &a::STY, &a::ABS, 4}, {"STA", &a::STA, &a::ABS, 4}, {"STX", &a::STX, &a::ABS, 4}, {"???", &a::ILL, &a::IMP, 2},
                    {"BCC", &a::BCC, &a::REL, 2}, {"STA", &a::STA, &a::IDY, 6}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"STY", &a::STY, &a::ZPX, 4}, {"STA", &a::STA, &a::ZPX, 4}, {"STX", &a::STX, &a::ZPY, 4}, {"???", &a::ILL, &a::IMP, 2}, {"TYA", &a::TYA, &a::IMP, 2}, {"STA", &a::STA, &a::ABY, 5}, {"TXS", &a::TXS, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"STA", &a::STA, &a::ABX, 5}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2},
                    {"LDY", &a::LDY, &a::IMM, 2}, {"LDA", &a::LDA, &a::IDX, 6}, {"LDX", &a::LDX, &a::IMM, 2}, {"???", &a::ILL, &a::IMP, 2}, {"LDY", &a::LDY, &a::ZPG, 4}, {"LDA", &a::LDA, &a::ZPG, 3}, {"LDX", &a::LDX, &a::ZPG, 3}, {"???", &a::ILL, &a::IMP, 2}, {"TAY", &a::TAY, &a::IMP, 2}, {"LDA", &a::LDA, &a::IMM, 2}, {"TAX", &a::TAX, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"LDY", &a::LDY, &a::ABS, 4}, {"LDA", &a::LDA, &a::ABS, 4}, {"LDX", &a::LDX, &a::ABS, 4}, {"???", &a::ILL, &a::IMP, 2},
                    {"BCS", &a::BCS, &a::REL, 2}, {"LDA", &a::LDA, &a::IDY, 5}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"LDY", &a::LDY, &a::ZPX, 4}, {"LDA", &a::LDA, &a::ZPX, 4}, {"LDX", &a::LDX, &a::ZPY, 4}, {"???", &a::ILL, &a::IMP, 2}, {"CLV", &a::CLV, &a::IMP, 2}, {"LDA", &a::LDA, &a::ABY, 4}, {"TSX", &a::TSX, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"LDY", &a::LDY, &a::ABX, 4}, {"LDA", &a::LDA, &a::ABX, 4}, {"LDX", &a::LDX, &a::ABY, 4}, {"???", &a::ILL, &a::IMP, 2},
                    {"CPY", &a::CPY, &a::IMM, 2}, {"CMP", &a::CMP, &a::IDX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"CPY", &a::CPY, &a::ZPG, 3}, {"CMP", &a::CMP, &a::ZPG, 3}, {"DEC", &a::DEC, &a::ZPG, 5}, {"???", &a::ILL, &a::IMP, 2}, {"INY", &a::INY, &a::IMP, 2}, {"CMP", &a::CMP, &a::IMM, 2}, {"DEX", &a::DEX, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"CPY", &a::CPY, &a::ABS, 4}, {"CMP", &a::CMP, &a::ABS, 4}, {"DEC", &a::DEC, &a::ABS, 6}, {"???", &a::ILL, &a::IMP, 2},
                    {"BNE", &a::BNE, &a::REL, 2}, {"CMP", &a::CMP, &a::IDY, 5}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"CMP", &a::CMP, &a::ZPX, 4}, {"DEC", &a::DEC, &a::ZPX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"CLD", &a::CLD, &a::IMP, 2}, {"CMP", &a::CMP, &a::ABY, 4}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"CMP", &a::CMP, &a::ABX, 4}, {"DEC", &a::DEC, &a::ABX, 7}, {"???", &a::ILL, &a::IMP, 2},
                    {"CPX", &a::CPX, &a::IMM, 2}, {"SBC", &a::SBC, &a::IDX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"CPX", &a::CPX, &a::ZPG, 3}, {"SBC", &a::SBC, &a::ZPG, 3}, {"INC", &a::INC, &a::ZPG, 5}, {"???", &a::ILL, &a::IMP, 2}, {"INX", &a::INX, &a::IMP, 2}, {"SBC", &a::SBC, &a::IMM, 2}, {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"CPX", &a::CPX, &a::ABS, 4}, {"SBC", &a::SBC, &a::ABS, 4}, {"INC", &a::INC, &a::ABS, 6}, {"???", &a::ILL, &a::IMP, 2},
                    {"BEQ", &a::BEQ, &a::REL, 2}, {"SBC", &a::SBC, &a::IDY, 5}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"SBC", &a::SBC, &a::ZPX, 4}, {"INC", &a::INC, &a::ZPX, 6}, {"???", &a::ILL, &a::IMP, 2}, {"SED", &a::SED, &a::IMP, 2}, {"SBC", &a::SBC, &a::ABY, 4}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"???", &a::ILL, &a::IMP, 2}, {"SBC", &a::SBC, &a::ABX, 4}, {"INC", &a::INC, &a::ABX, 7}, {"???", &a::ILL, &a::IMP, 2},
            };
}

cpu6502::~cpu6502() = default;

void cpu6502::write(uint16_t addr, uint8_t data) {
    bus->cpuWrite(addr, data);
}

uint8_t cpu6502::read(uint16_t addr) {
    return bus->cpuRead(addr);
}

void cpu6502::setFlag(FLAGS flag, bool value) {
    if (value) {
        sr |= flag;
    } else {
        sr &= ~flag;
    }
}

bool cpu6502::getFlag(FLAGS flag) const {
    return ((sr & flag) > 0);
}

void cpu6502::clock() {
    if (cycles == 0) {
        opcode = read(pc); // Read data from the bus at the Program Counter address
        pc++; // Increment the Program Counter

        cycles = instructions[opcode].cycles; // Get the number of cycles needed for the cpuRead opcode

        bool addCycle1 = (this->*instructions[opcode].addrmode)(); // Execute the appropriate Addressing Mode for the cpuRead opcode (if one more clock cycle is needed, it will keep it in this variable)
        bool addCycle2 = (this->*instructions[opcode].operate)(); // Execute the appropriate Instruction for the cpuRead opcode (if one more clock cycle is needed, it will keep it in this variable)

        cycles += (addCycle1 & addCycle2);
    }

    cycles--;
}

uint8_t cpu6502::fetch() {
    if (instructions[opcode].addrmode != &cpu6502::ACC) {
        fetched = read(addr_abs);
    }
    return fetched;
}

void cpu6502::reset() {
    ac = 0;
    x = 0;
    y = 0;
    sp = 0xFF;
    sr = 0x00;

    addr_abs = 0xFFFC; // The reset vector is 0xFFFC-0xFFFD
    pc = read(addr_abs);
    pc |= read(addr_abs + 1) << 8;

    addr_abs = 0x0000;
    addr_rel = 0x0000;
    fetched = 0x00;

    cycles = 8;
}

void cpu6502::irq() {
    if (getFlag(I) == 0) {
        write(0x0100 + sp, (pc >> 8) & 0x00FF);
        sp--;
        write(0x0100 + sp, pc & 0x00FF);
        sp--;

        setFlag(I, 1);
        setFlag(B, 1);
        write(0x0100 + sp, sr);
        setFlag(B, 0);
        sp--;

        pc = read(0xFFFE);
        pc |= read(0xFFFF) << 8;

        cycles = 7;
    }
}

void cpu6502::nmi() {
    write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(0x0100 + sp, pc & 0x00FF);
    sp--;

    setFlag(I, 1);
    setFlag(B, 1);
    write(0x0100 + sp, sr);
    setFlag(B, 0);
    sp--;

    pc = read(0xFFFA);
    pc |= read(0xFFFB) << 8;

    cycles = 8;
}

// ####################################################################################################################
// ####################################################################################################################
//                                               ADDRESSING MODES
// IMPORTANT NOTE : The Program Counter is set to the data followed by the instruction
// ####################################################################################################################
// ####################################################################################################################

bool cpu6502::ABS() {
    uint16_t lo = read(pc); // Reading the low-endian byte i.e: $00LL
    pc++;
    uint16_t hi = read(pc); // Reading the high-endian byte i.e: $00HH
    pc++;

    addr_abs = (hi << 8) | lo; // Create an address from the low-endian and the high-endian. i.e: $HHLL

    return 0;
}

bool cpu6502::ABX() {
    uint16_t lo = read(pc); // Reading the low-endian byte i.e: $00LL
    pc++;
    uint16_t hi = read(pc); // Reading the high-endian byte i.e: $00HH
    pc++;

    addr_abs = (hi << 8) | lo; // Create an address from the low-endian and the high-endian. i.e: $HHLL
    addr_abs += x; // Add the X register to create the final address

    if ((addr_abs & 0xFF00) != (hi << 8)) { // Checking if the final address is on the same page as the initial address. (The page is values between 0x00 and 0xFF on the high byte)
        return 1; // Adding the X register can result the address to be in another page, we add 1 cycle
    } else {
        return 0; // Same page, nothing to do
    }
}

bool cpu6502::ABY() {
    uint16_t lo = read(pc); // Reading the low-endian byte i.e: $00LL
    pc++;
    uint16_t hi = read(pc); // Reading the high-endian byte i.e: $00HH
    pc++;

    addr_abs = (hi << 8) | lo; // Create an address from the low-endian and the high-endian. i.e: $HHLL
    addr_abs += y; // Add the Y register to create the final address

    if ((addr_abs & 0xFF00) != (hi << 8)) { // Checking if the final address is on the same page as the initial address. (The page is values between 0x00 and 0xFF on the high byte)
        return 1; // Adding the Y register can result the address to be in another page, we add 1 cycle
    } else {
        return 0; // Same page, nothing to do
    }
}

bool cpu6502::ACC() {
    fetched = ac; // The accumulator is fetched because it contains the address
    return 0; // When the address is the accumulator, no more cycles are needed in any instructions
}

bool cpu6502::IMM() {
    addr_abs = pc; // The address is contained after the instruction. The program knows where to go
    pc++;
    return 0; // When the address is the next byte, no more cycles are needed in any instructions
}

bool cpu6502::IMP() {
    return 0; // When the address is implied, no more cycles are needed in any instructions
}

bool cpu6502::IND() {
    uint16_t lo = read(pc); // Reading the low-endian byte i.e: $00LL
    pc++;
    uint16_t hi = read(pc); // Reading the high-endian byte i.e: $00HH
    pc++;

    uint16_t ptr = (hi << 8) | lo; // Create an address from the low-endian and the high-endian. i.e: $HHLL

    addr_abs = (read(ptr + 1) << 8) | read(ptr + 0); // We create an address from the data cpuRead at the ptr address

    return 0;
}

bool cpu6502::IDX() {
    uint16_t ptr = read(pc); // Reading the low-endian byte i.e: $00LL
    pc++;

    addr_abs = (read((ptr + (uint16_t)x + 1) & 0x00FF) << 8) | read((ptr + (uint16_t)x + 0) & 0x00FF); // We create an address from the data cpuRead at the ptr address

    return 0;
}

bool cpu6502::IDY() {
    uint16_t ptr = read(pc); // Reading the low-endian byte i.e: $00LL
    pc++;

    addr_abs = (read((ptr + 1) & 0x00FF) << 8) | read((ptr & 0x00FF) + 0); // We create an address from the data cpuRead at the ptr address
    addr_abs += y; // Adding Y register to create the final address

    if ((addr_abs & 0xFF00) != (read((ptr + 1) & 0x00FF) << 8)) { // Checking if the final address is on the same page as the initial address. (The page is values between 0x00 and 0xFF on the high byte)
        return 1; // Adding the Y register can result the address to be in another page, we add 1 cycle
    } else {
        return 0; // Same page, nothing to do
    }
}

bool cpu6502::REL() {
    addr_rel = read(pc); // Reading the relative address
    pc++;
    if (addr_rel & 0x80) { // If the address is > 0x80 (128)
        addr_rel |= 0xFF00;
    }
    return 0;
}

bool cpu6502::ZPG() {
    addr_abs = read(pc); // Reading the address
    pc++;
    addr_abs &= 0x00FF; // Make sure that the address is on the zero page
    return 0;
}

bool cpu6502::ZPX() {
    addr_abs = read(pc); // Reading the address
    pc++;
    addr_abs += x; // Adding X to create the final address
    addr_abs &= 0x00FF; // Make sure that the address is on the zero page
    return 0;
}

bool cpu6502::ZPY() {
    addr_abs = read(pc); // Reading the address
    pc++;
    addr_abs += y; // Adding X to create the final address
    addr_abs &= 0x00FF; // Make sure that the address is on the zero page
    return 0;
}

// ####################################################################################################################
// ####################################################################################################################
//                                                      INSTRUCTIONS
// ####################################################################################################################
// ####################################################################################################################

bool cpu6502::ADC() {
    fetch(); // Read the value

    uint16_t sum = ac + fetched + getFlag(C); // Add the fetched value to the accumulator with carry

    setFlag(N, sum & 0x80); // Set the negative flag if the most significant bit is 1
    setFlag(Z, (sum & 0x00FF) == 0); // Set the Zero flag if the sum is equal to 0x0000
    setFlag(C, sum > 255); // Set the Carry flag if the sum is greater than 255 (the max 8-bit value)
    // To understand this check : http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
    setFlag(V, !((ac ^ fetched) & 0x80) && ((ac ^ sum) & 0x80));

    ac = sum & 0x00FF; // The Accumulator is an 8 bit value, so we only keep the 8-bit value

    return 1; // On some case ADC add 1 more cycle
}

bool cpu6502::AND() {
    fetch(); // Read the value

    ac = ac & fetched; // Logic AND on the Accumulator and the fetched value

    setFlag(N, ac & 0x80); // Set the Negative flag if the most significant bit is 1
    setFlag(Z, ac == 0x00); // Set the Zero flag if the Accumulator is 0x00

    return 1; // On some case AND add 1 more cycle
}

bool cpu6502::ASL() {
    fetch(); // Read the value

    uint16_t temp = fetched << 1; // Shift Left One bit the fetched value

    setFlag(N, temp & 0x80);
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(C, (temp & 0xFF00) > 0);

    if (instructions[opcode].addrmode == &cpu6502::ACC) {
        ac = temp & 0x00FF; // If the addressing mode is Accumulator, set the accumulator
    } else {
        write(addr_abs, temp & 0x00FF); // Else the memory is impacted, we cpuWrite the value at the memory location
    }

    return 0;
}

bool cpu6502::BCC() {
    if (getFlag(C) == 0) { // If the Carry flag is 0
        cycles++; // Add a cycle
        addr_abs = pc + addr_rel; // Change the absolute address to the Program Counter + the Relative Address

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) { // If the new absolute address is not on the same page as the current Address
            cycles++; // Add a cycle
        }

        pc = addr_abs; // The absolute address become the current address
    }

    return 0;
}

bool cpu6502::BCS() {
    if (getFlag(C) != 0) { // If the Carry flag is 1
        cycles++; // Add a cycle
        addr_abs = pc + addr_rel; // Change the absolute address to the Program Counter + the Relative address

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) { // If the new absolute address is not on the same page ad the current address
            cycles++; // Add a cycle
        }

        pc = addr_abs; // The absolute address become the current address
    }

    return 0;
}

bool cpu6502::BEQ() {
    if (getFlag(Z) != 0) { // If the Carry flag is 1
        cycles++; // Add a cycle
        addr_abs = pc + addr_rel; // Change the absolute address to the Program Counter + the Relative address

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) { // If the new absolute address is not on the same page ad the current address
            cycles++; // Add a cycle
        }

        pc = addr_abs; // The absolute address become the current address
    }

    return 0;
}

bool cpu6502::BIT() {
    fetch(); // Read the value

    uint16_t temp = ac & fetched; // Bitwise AND between Accumulator and the fetched value

    setFlag(N, fetched & 0x80); // Negative flag set according to the 7th bit of the fetched value
    setFlag(Z, (temp & 0x00FF) == 0); // If the sum between Accumulator and the fetched value is equal to 0, the Zero flag is set
    setFlag(V, fetched & 0x40); // Overflow flag is set according to the 6th bit of the fetched value

    return 0;
}

bool cpu6502::BMI() {
    if (getFlag(N) != 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }

    return 0;
}

bool cpu6502::BNE() {
    if (getFlag(Z) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }

    return 0;
}

bool cpu6502::BPL() {
    if (getFlag(N) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }

    return 0;
}

bool cpu6502::BRK() {
    setFlag(I, 1);

    write(0x0100 + sp, ((pc + 1) >> 8) & 0x00FF);
    sp--;
    write(0x0100 + sp, (pc + 1) & 0x00FF);
    sp--;

    setFlag(B, 1);
    write(0x0100 + sp, sr);
    setFlag(B, 0);
    sp--;

    pc = read(0xFFFE) | (read(0xFFFF) << 8);

    return 0;
}

bool cpu6502::BVC() {
    if (getFlag(V) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }

    return 0;
}

bool cpu6502::BVS() {
    if (getFlag(V) != 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }

    return 0;
}

bool cpu6502::CLC() {
    setFlag(C, 0);

    return 0;
}

bool cpu6502::CLD() {
    setFlag(D, 0);

    return 0;
}

bool cpu6502::CLI() {
    setFlag(I, 0);

    return 0;
}

bool cpu6502::CLV() {
    setFlag(V, 0);

    return 0;
}

bool cpu6502::CMP() {
    fetch();

    uint16_t temp = ac - fetched;

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(C, ac >= fetched);

    return 1;
}

bool cpu6502::CPX() {
    fetch();

    uint16_t temp = x - fetched;

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(C, x >= fetched);

    return 0;
}

bool cpu6502::CPY() {
    fetch();

    uint16_t temp = y - fetched;

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(C, y >= fetched);

    return 0;
}

bool cpu6502::DEC() {
    fetch();

    uint16_t temp = fetched - 0x0001;

    write(addr_abs, temp & 0x00FF);

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);


    return 0;
}

bool cpu6502::DEX() {
    uint16_t temp = x - 0x0001;

    x = temp & 0x00FF;

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);

    return 0;
}

bool cpu6502::DEY() {
    uint16_t temp = y - 0x0001;

    y = temp & 0x00FF;

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);

    return 0;
}

bool cpu6502::EOR() {
    fetch();

    ac = ac ^ fetched;

    setFlag(N, ac & 0x80);
    setFlag(Z, ac == 0x00);

    return 1;
}

bool cpu6502::INC() {
    fetch();

    uint16_t temp = fetched + 0x0001;
    write(addr_abs, temp & 0x00FF);

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);

    return 0;
}

bool cpu6502::INX() {
    x = x + 0x01;

    setFlag(N, x & 0x80);
    setFlag(Z, x == 0x00);

    return 0;
}

bool cpu6502::INY() {
    y = y + 0x01;

    setFlag(N, y & 0x80);
    setFlag(Z, y == 0x00);

    return 0;
}

bool cpu6502::JMP() {
    pc = addr_abs;

    return 0;
}

bool cpu6502::JSR() {
    write(0x0100 + sp, ((pc - 1) >> 8) & 0x00FF);
    sp--;
    write(0x0100, (pc -1) & 0x00FF);
    sp--;

    pc = addr_abs;

    return 0;
}

bool cpu6502::LDA() {
    fetch();

    ac = fetched;

    setFlag(N, ac & 0x80);
    setFlag(Z, ac == 0x00);

    return 1;
}

bool cpu6502::LDX() {
    fetch();

    x = fetched;

    setFlag(N, x & 0x80);
    setFlag(Z, x == 0x00);

    return 1;
}

bool cpu6502::LDY() {
    fetch();

    y = fetched;

    setFlag(N, y & 0x80);
    setFlag(Z, y == 0x00);

    return 1;
}

bool cpu6502::LSR() {
    fetch();

    setFlag(C, fetched & 0x0001);

    uint16_t temp = fetched >> 1;

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);

    if (instructions[opcode].addrmode == &cpu6502::ACC) {
        ac = temp & 0x00FF;
    } else {
        write(addr_abs, temp & 0x00FF);
    }

    return 0;
}

bool cpu6502::NOP() {
    return 0;
}

bool cpu6502::ORA() {
    fetch();

    ac = ac | fetched;

    setFlag(N, ac & 0x80);
    setFlag(Z, ac == 0x00);

    return 1;
}

bool cpu6502::PHA() {
    write(0x0100 + sp, ac);
    sp--;

    return 0;
}

bool cpu6502::PHP() {
    setFlag(B, 1);
    write(0x0100 + sp, sr & 0x00FF);
    setFlag(B, 0);
    sp--;

    return 0;
}

bool cpu6502::PLA() {
    sp++;
    ac = read(0x0100 + sp);

    setFlag(N, ac & 0x80);
    setFlag(Z, ac == 0x00);

    return 0;
}

bool cpu6502::PLP() {
    sp++;
    sr = read(0x0100 + sp);

    return 0;
}

bool cpu6502::ROL() {
    fetch();

    uint16_t temp = (fetched << 1) | getFlag(C);

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(C, (temp & 0xFF00) > 0x0000);

    if (instructions[opcode].addrmode == &cpu6502::ACC) {
        ac = temp & 0x00FF;
    } else {
        write(addr_abs, temp & 0x00FF);
    }

    return 0;
}

bool cpu6502::ROR() {
    fetch();

    uint16_t temp = (fetched >> 1) | getFlag(C);

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(C, (temp & 0xFF00) > 0x0000);

    if (instructions[opcode].addrmode == & cpu6502::ACC) {
        ac = temp & 0x00FF;
    } else {
        write(addr_abs, temp & 0x00FF);
    }

    return 0;
}

bool cpu6502::RTI() {
    sp++;
    sr = read(0x0100 + sp);

    sp++;
    pc = read(0x0100 + sp);
    sp++;
    pc |= read(0x0100 + sp) << 8;

    return 0;
}

bool cpu6502::RTS() {
    sp++;
    pc = read(0x0100 + sp);
    sp++;
    pc |= read(0x0100 + sp) << 8;

    return 0;
}

bool  cpu6502::SBC() {
    fetch();

    uint16_t temp = ac - fetched - getFlag(C);

    setFlag(N, temp & 0x0080);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(C, (temp & 0xFF00) > 0x0000);
    setFlag(V, !((ac ^ fetched) & 0x80) && ((ac ^ temp) & 0x80));

    ac = temp & 0x00FF;

    return 1;
}

bool cpu6502::SEC() {
    setFlag(C, 1);

    return 0;
}

bool cpu6502::SED() {
    setFlag(D, 1);

    return 0;
}

bool cpu6502::SEI() {
    setFlag(I, 1);

    return 0;
}

bool cpu6502::STA() {
    write(addr_abs, ac);

    return 0;
}

bool cpu6502::STX() {
    write(addr_abs, x);

    return 0;
}

bool cpu6502::STY() {
    write(addr_abs, y);

    return 0;
}

bool cpu6502::TAX() {
    x = ac;

    setFlag(N, x & 0x80);
    setFlag(Z, x == 0x00);

    return 0;
}

bool cpu6502::TAY() {
    y = ac;

    setFlag(N, y & 0x80);
    setFlag(Z, y == 0x00);

    return 0;
}

bool cpu6502::TSX() {
    x = sp;

    setFlag(N, x & 0x80);
    setFlag(Z, x == 0x00);

    return 0;
}

bool cpu6502::TXA() {
    ac = x;

    setFlag(N, ac & 0x80);
    setFlag(Z, ac == 0x00);

    return 0;
}

bool cpu6502::TXS() {
    sp = x;

    return 0;
}

bool cpu6502::TYA() {
    ac = y;

    setFlag(N, ac & 0x80);
    setFlag(Z, ac == 0x00);

    return 0;
}

bool cpu6502::ILL() {
    return 0;
}

bool cpu6502::complete()
{
    return cycles == 0;
}

// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> cpu6502::disassemble(uint16_t nStart, uint16_t nStop)
{
    uint32_t addr = nStart;
    uint8_t value = 0x00, lo = 0x00, hi = 0x00;
    std::map<uint16_t, std::string> mapLines;
    uint16_t line_addr = 0;

    // A convenient utility to convert variables into
    // hex strings because "modern C++"'s method with 
    // streams is atrocious
    auto hex = [](uint32_t n, uint8_t d)
    {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    };

    // Starting at the specified address we read an instruction
    // byte, which in turn yields information from the instructions table
    // as to how many additional bytes we need to read and what the
    // addressing mode is. I need this info to assemble human readable
    // syntax, which is different depending upon the addressing mode

    // As the instruction is decoded, a std::string is assembled
    // with the readable output
    while (addr <= (uint32_t)nStop)
    {
        line_addr = addr;

        // Prefix line with instruction address
        std::string sInst = "$" + hex(addr, 4) + ": ";

        // Read instruction, and get its readable name
        uint8_t opcode = bus->cpuRead(addr); addr++;
        sInst += instructions[opcode].name + " ";

        // Get oprands from desired locations, and form the
        // instruction based upon its addressing mode. These
        // routines mimmick the actual fetch routine of the
        // 6502 in order to get accurate data as part of the
        // instruction
        if (instructions[opcode].addrmode == &cpu6502::IMP)
        {
            sInst += " {IMP}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::IMM)
        {
            value = bus->cpuRead(addr); addr++;
            sInst += "#$" + hex(value, 2) + " {IMM}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::ZPG)
        {
            lo = bus->cpuRead(addr); addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + " {ZP0}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::ZPX)
        {
            lo = bus->cpuRead(addr); addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + ", X {ZPX}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::ZPY)
        {
            lo = bus->cpuRead(addr); addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::IDX)
        {
            lo = bus->cpuRead(addr); addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + ", X) {IZX}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::IDY)
        {
            lo = bus->cpuRead(addr); addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + "), Y {IZY}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::ABS)
        {
            lo = bus->cpuRead(addr); addr++;
            hi = bus->cpuRead(addr); addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::ABX)
        {
            lo = bus->cpuRead(addr); addr++;
            hi = bus->cpuRead(addr); addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::ABY)
        {
            lo = bus->cpuRead(addr); addr++;
            hi = bus->cpuRead(addr); addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::IND)
        {
            lo = bus->cpuRead(addr); addr++;
            hi = bus->cpuRead(addr); addr++;
            sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
        }
        else if (instructions[opcode].addrmode == &cpu6502::REL)
        {
            value = bus->cpuRead(addr); addr++;
            sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
        }

        // Add the formed string to a std::map, using the instruction's
        // address as the key. This makes it convenient to look for later
        // as the instructions are variable in length, so a straight up
        // incremental index is not sufficient.
        mapLines[line_addr] = sInst;
    }

    return mapLines;
}
