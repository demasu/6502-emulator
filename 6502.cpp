#include "6502.h"
#include "Bus.h"

// Constructor
cpu6502::cpu6502() {
    // Assembles the translation table.
    using a = cpu6502;

    // Pulled from https://github.com/OneLoneCoder/olcNES/blob/master/Part%232%20-%20CPU/olc6502.cpp
    // (Such a huge table)
    lookup = {
        {"BRK", &a::BRK, &a::IMM, 7},
        {"ORA", &a::ORA, &a::IZX, 6},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 3},
        {"ORA", &a::ORA, &a::ZP0, 3},
        {"ASL", &a::ASL, &a::ZP0, 5},
        {"???", &a::XXX, &a::IMP, 5},
        {"PHP", &a::PHP, &a::IMP, 3},
        {"ORA", &a::ORA, &a::IMM, 2},
        {"ASL", &a::ASL, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::NOP, &a::IMP, 4},
        {"ORA", &a::ORA, &a::ABS, 4},
        {"ASL", &a::ASL, &a::ABS, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"BPL", &a::BPL, &a::REL, 2},
        {"ORA", &a::ORA, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4},
        {"ORA", &a::ORA, &a::ZPX, 4},
        {"ASL", &a::ASL, &a::ZPX, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"CLC", &a::CLC, &a::IMP, 2},
        {"ORA", &a::ORA, &a::ABY, 4},
        {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4},
        {"ORA", &a::ORA, &a::ABX, 4},
        {"ASL", &a::ASL, &a::ABX, 7},
        {"???", &a::XXX, &a::IMP, 7},
        {"JSR", &a::JSR, &a::ABS, 6},
        {"AND", &a::AND, &a::IZX, 6},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"BIT", &a::BIT, &a::ZP0, 3},
        {"AND", &a::AND, &a::ZP0, 3},
        {"ROL", &a::ROL, &a::ZP0, 5},
        {"???", &a::XXX, &a::IMP, 5},
        {"PLP", &a::PLP, &a::IMP, 4},
        {"AND", &a::AND, &a::IMM, 2},
        {"ROL", &a::ROL, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 2},
        {"BIT", &a::BIT, &a::ABS, 4},
        {"AND", &a::AND, &a::ABS, 4},
        {"ROL", &a::ROL, &a::ABS, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"BMI", &a::BMI, &a::REL, 2},
        {"AND", &a::AND, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4},
        {"AND", &a::AND, &a::ZPX, 4},
        {"ROL", &a::ROL, &a::ZPX, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"SEC", &a::SEC, &a::IMP, 2},
        {"AND", &a::AND, &a::ABY, 4},
        {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4},
        {"AND", &a::AND, &a::ABX, 4},
        {"ROL", &a::ROL, &a::ABX, 7},
        {"???", &a::XXX, &a::IMP, 7},
        {"RTI", &a::RTI, &a::IMP, 6},
        {"EOR", &a::EOR, &a::IZX, 6},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 3},
        {"EOR", &a::EOR, &a::ZP0, 3},
        {"LSR", &a::LSR, &a::ZP0, 5},
        {"???", &a::XXX, &a::IMP, 5},
        {"PHA", &a::PHA, &a::IMP, 3},
        {"EOR", &a::EOR, &a::IMM, 2},
        {"LSR", &a::LSR, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 2},
        {"JMP", &a::JMP, &a::ABS, 3},
        {"EOR", &a::EOR, &a::ABS, 4},
        {"LSR", &a::LSR, &a::ABS, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"BVC", &a::BVC, &a::REL, 2},
        {"EOR", &a::EOR, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4},
        {"EOR", &a::EOR, &a::ZPX, 4},
        {"LSR", &a::LSR, &a::ZPX, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"CLI", &a::CLI, &a::IMP, 2},
        {"EOR", &a::EOR, &a::ABY, 4},
        {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4},
        {"EOR", &a::EOR, &a::ABX, 4},
        {"LSR", &a::LSR, &a::ABX, 7},
        {"???", &a::XXX, &a::IMP, 7},
        {"RTS", &a::RTS, &a::IMP, 6},
        {"ADC", &a::ADC, &a::IZX, 6},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 3},
        {"ADC", &a::ADC, &a::ZP0, 3},
        {"ROR", &a::ROR, &a::ZP0, 5},
        {"???", &a::XXX, &a::IMP, 5},
        {"PLA", &a::PLA, &a::IMP, 4},
        {"ADC", &a::ADC, &a::IMM, 2},
        {"ROR", &a::ROR, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 2},
        {"JMP", &a::JMP, &a::IND, 5},
        {"ADC", &a::ADC, &a::ABS, 4},
        {"ROR", &a::ROR, &a::ABS, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"BVS", &a::BVS, &a::REL, 2},
        {"ADC", &a::ADC, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4},
        {"ADC", &a::ADC, &a::ZPX, 4},
        {"ROR", &a::ROR, &a::ZPX, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"SEI", &a::SEI, &a::IMP, 2},
        {"ADC", &a::ADC, &a::ABY, 4},
        {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4},
        {"ADC", &a::ADC, &a::ABX, 4},
        {"ROR", &a::ROR, &a::ABX, 7},
        {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 2},
        {"STA", &a::STA, &a::IZX, 6},
        {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 6},
        {"STY", &a::STY, &a::ZP0, 3},
        {"STA", &a::STA, &a::ZP0, 3},
        {"STX", &a::STX, &a::ZP0, 3},
        {"???", &a::XXX, &a::IMP, 3},
        {"DEY", &a::DEY, &a::IMP, 2},
        {"???", &a::NOP, &a::IMP, 2},
        {"TXA", &a::TXA, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 2},
        {"STY", &a::STY, &a::ABS, 4},
        {"STA", &a::STA, &a::ABS, 4},
        {"STX", &a::STX, &a::ABS, 4},
        {"???", &a::XXX, &a::IMP, 4},
        {"BCC", &a::BCC, &a::REL, 2},
        {"STA", &a::STA, &a::IZY, 6},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 6},
        {"STY", &a::STY, &a::ZPX, 4},
        {"STA", &a::STA, &a::ZPX, 4},
        {"STX", &a::STX, &a::ZPY, 4},
        {"???", &a::XXX, &a::IMP, 4},
        {"TYA", &a::TYA, &a::IMP, 2},
        {"STA", &a::STA, &a::ABY, 5},
        {"TXS", &a::TXS, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 5},
        {"???", &a::NOP, &a::IMP, 5},
        {"STA", &a::STA, &a::ABX, 5},
        {"???", &a::XXX, &a::IMP, 5},
        {"???", &a::XXX, &a::IMP, 5},
        {"LDY", &a::LDY, &a::IMM, 2},
        {"LDA", &a::LDA, &a::IZX, 6},
        {"LDX", &a::LDX, &a::IMM, 2},
        {"???", &a::XXX, &a::IMP, 6},
        {"LDY", &a::LDY, &a::ZP0, 3},
        {"LDA", &a::LDA, &a::ZP0, 3},
        {"LDX", &a::LDX, &a::ZP0, 3},
        {"???", &a::XXX, &a::IMP, 3},
        {"TAY", &a::TAY, &a::IMP, 2},
        {"LDA", &a::LDA, &a::IMM, 2},
        {"TAX", &a::TAX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 2},
        {"LDY", &a::LDY, &a::ABS, 4},
        {"LDA", &a::LDA, &a::ABS, 4},
        {"LDX", &a::LDX, &a::ABS, 4},
        {"???", &a::XXX, &a::IMP, 4},
        {"BCS", &a::BCS, &a::REL, 2},
        {"LDA", &a::LDA, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 5},
        {"LDY", &a::LDY, &a::ZPX, 4},
        {"LDA", &a::LDA, &a::ZPX, 4},
        {"LDX", &a::LDX, &a::ZPY, 4},
        {"???", &a::XXX, &a::IMP, 4},
        {"CLV", &a::CLV, &a::IMP, 2},
        {"LDA", &a::LDA, &a::ABY, 4},
        {"TSX", &a::TSX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 4},
        {"LDY", &a::LDY, &a::ABX, 4},
        {"LDA", &a::LDA, &a::ABX, 4},
        {"LDX", &a::LDX, &a::ABY, 4},
        {"???", &a::XXX, &a::IMP, 4},
        {"CPY", &a::CPY, &a::IMM, 2},
        {"CMP", &a::CMP, &a::IZX, 6},
        {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"CPY", &a::CPY, &a::ZP0, 3},
        {"CMP", &a::CMP, &a::ZP0, 3},
        {"DEC", &a::DEC, &a::ZP0, 5},
        {"???", &a::XXX, &a::IMP, 5},
        {"INY", &a::INY, &a::IMP, 2},
        {"CMP", &a::CMP, &a::IMM, 2},
        {"DEX", &a::DEX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 2},
        {"CPY", &a::CPY, &a::ABS, 4},
        {"CMP", &a::CMP, &a::ABS, 4},
        {"DEC", &a::DEC, &a::ABS, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"BNE", &a::BNE, &a::REL, 2},
        {"CMP", &a::CMP, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4},
        {"CMP", &a::CMP, &a::ZPX, 4},
        {"DEC", &a::DEC, &a::ZPX, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"CLD", &a::CLD, &a::IMP, 2},
        {"CMP", &a::CMP, &a::ABY, 4},
        {"NOP", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4},
        {"CMP", &a::CMP, &a::ABX, 4},
        {"DEC", &a::DEC, &a::ABX, 7},
        {"???", &a::XXX, &a::IMP, 7},
        {"CPX", &a::CPX, &a::IMM, 2},
        {"SBC", &a::SBC, &a::IZX, 6},
        {"???", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"CPX", &a::CPX, &a::ZP0, 3},
        {"SBC", &a::SBC, &a::ZP0, 3},
        {"INC", &a::INC, &a::ZP0, 5},
        {"???", &a::XXX, &a::IMP, 5},
        {"INX", &a::INX, &a::IMP, 2},
        {"SBC", &a::SBC, &a::IMM, 2},
        {"NOP", &a::NOP, &a::IMP, 2},
        {"???", &a::SBC, &a::IMP, 2},
        {"CPX", &a::CPX, &a::ABS, 4},
        {"SBC", &a::SBC, &a::ABS, 4},
        {"INC", &a::INC, &a::ABS, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"BEQ", &a::BEQ, &a::REL, 2},
        {"SBC", &a::SBC, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4},
        {"SBC", &a::SBC, &a::ZPX, 4},
        {"INC", &a::INC, &a::ZPX, 6},
        {"???", &a::XXX, &a::IMP, 6},
        {"SED", &a::SED, &a::IMP, 2},
        {"SBC", &a::SBC, &a::ABY, 4},
        {"NOP", &a::NOP, &a::IMP, 2},
        {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4},
        {"SBC", &a::SBC, &a::ABX, 4},
        {"INC", &a::INC, &a::ABX, 7},
        {"???", &a::XXX, &a::IMP, 7},
    };
}

cpu6502::~cpu6502() {
    // Destructor does nothing
}

// Bus connectivity

// Reads an 8-bit byte from the bus at the specified 16-bit address
uint8_t cpu6502::read(uint16_t addr) {
    // Read-only is set to false in normal operation as we want things to happen normally.
    // Some bus items may change state when read from and we want that.
    // However, when we're doing disassembly, we don't want that to happen.
    return bus->cpuRead(addr, false);
}

// Writes an 8-bit byte to the specified address
void cpu6502::write(uint16_t addr, uint8_t data) {
    bus->cpuWrite(addr, data);
}

// External Inputs

// Reset forces the CPU into a known state. The registers are set to 0x00, the status register is cleared
// except for the unused bit which stays at 1. An address is read from 0xFFFC and 0xFFFD which contains the
// address for the program counter to be set to.
void cpu6502::reset() {
    // Get the address to set the program counter to
    addr_abs    = 0xFFFC;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    pc          = (hi << 8) | lo;

    // Reset internal registers
    a      = 0;
    x      = 0;
    y      = 0;
    sptr   = 0xFD;
    status = 0x00 | U;

    // Clear internal helper variables
    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched  = 0x00;

    // The number of cycles for a reset
    cycles = 8;
}

// Interrupt requests happen only if the disable interrupt flag is 0.
// IRQs can happen at any time, but the current instruction is allowed to finish first.
void cpu6502::irq() {
    if (GetFlag(I) == 0) {
        // Push the program counter to the stack. Takes two pushes since it's 16-bits
        write(0x0100 + sptr, (pc >> 8) & 0x00FF);
        sptr--;
        write(0x0100 + sptr, pc & 0x00FF);
        sptr--;

        // Push the status register to the stack
        SetFlag(B, 0);
        SetFlag(U, 1);
        SetFlag(I, 1);
        write(0x0100 + sptr, status);
        sptr--;

        // Read the new program counter location from a fixed address
        addr_abs    = 0xFFFE;
        uint16_t lo = read(addr_abs + 0);
        uint16_t hi = read(addr_abs + 1);
        pc          = (hi << 8) | lo;

        // Cycles for the interrupt
        cycles = 7;
    }
}

// Non-maskable interrupts cannot be ignored. It behaves in the same way as the regular IRQ
// but it reads from 0xFFFA instead.
void cpu6502::nmi() {
    write(0x0100 + sptr, (pc >> 9) * 0x00FF);
    sptr--;
    write(0x0100 + sptr, pc & 0x00FF);
    sptr--;

    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    write(0x100 + sptr, status);
    sptr--;

    addr_abs    = 0xFFFA;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    pc          = (hi << 8) | lo;

    cycles = 8;
}

// Perform one clock cycle's worth of emulation
void cpu6502::clock() {
    // Each instruction requires a variable number of cucles to execute.
    // This emulation only cares about the final result and is not a
    // clock-cycle accurate emulation. As such, we only do stuff when the cycle
    // count is 0.

    if (cycles == 0) {
        // Read the next instruction byte. This byte is used to index the translation table
        // so we can get the information we need to implement the instruction
        opcode = read(pc);

#ifdef LOGMODE
        uint16_t log_pc = pc;
#endif

        // Always set the unused status flag to 1
        SetFlag(U, true);

        // Increment the program counter since we've read the instruction byte
        pc++;

        // Get the starting number of cycles
        cycles = lookup[opcode].cycles;

        // Perform fetch of the intermediate data using the required addressing mode
        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();

        // Perform the actual operation
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

        // Since we may have a need for more clock cycles due to the addressing mode, we need
        // to add them here
        cycles += (additional_cycle1 & additional_cycle2);

        // Definitely make sure the unused status flag is 1
        SetFlag(U, true);

#ifdef LOGMODE
        // The logger dumps the entire processor state every cycle for analysis.
        // It can be used for debugging the emulation, but it is also very slow.
        if (logfile == nullptr) {
            logfile = fopen("cpu6502.txt", "a+");
        }
        if (logfile != nullptr) {
            fprintf(logfile, "%10d:%02d PC:%04X %s A:%02X X:%02X Y:%02X %s%s%s%s%s%s%s%s SPTR:%02X\n",
                    clock_count, 0, log_pc, lookup[opcode].name.c_str(), a, x, y, GetFlag(N) ? "N" : ".", GetFlag(V) ? "V" : ".", GetFlag(U) ? "U" : ".",
                    GetFlag(B) ? "B" : ".", GetFlag(D) ? "D" : ".", GetFlag(I) ? "I" : ".",
                    GetFlag(Z) ? "Z" : ".", GetFlag(C) ? "C" : ".", sptr);
            fclose(logfile);
            logfile = nullptr;
        }
#endif
    }

// Increment global clock count
#ifdef LOGMODE
    logger.log("Incrementing the global clock count");
#endif
    clock_count++;
#ifdef LOGMODE
    logger.log("Global clock count is now: [" + std::to_string(clock_count) + "]");
#endif

// Decrement the number of cycles remaining for this instruction
#ifdef LOGMODE
    logger.log("Decrementing the number of cycles to go");
#endif
    cycles--;
#ifdef LOGMODE
    logger.log("Cycles is now: [" + std::to_string(cycles) + "]");
#endif
}

// Flag functions

// Returns the value of a specific bit of the status register
uint8_t cpu6502::GetFlag(FLAGS6502 f) {
    return ((status & f) > 0) ? 1 : 0;
}

// Set or clear a specific status flag
void cpu6502::SetFlag(FLAGS6502 f, bool v) {
    if (v) {
        status |= f;
    }
    else {
        status &= ~f;
    }
}

// Addressing modes

// Address Mode: Implied
// There is no additional data required for this instruction. The instruction
// does something very simple like like sets a status bit. However, we will
// target the accumulator, for instructions like PHA
uint8_t cpu6502::IMP() {
    fetched = a;
    return 0;
}

// Address Mode: Immediate
// The instruction expects the next byte to be used as a value, so we'll prep
// the read address to point to the next byte
uint8_t cpu6502::IMM() {
    addr_abs = pc++;
    return 0;
}

// Address Mode: Zero Page
// To save program bytes, zero page addressing allows you to absolutely address
// a location in first 0xFF bytes of address range. Clearly this only requires
// one byte instead of the usual two.
uint8_t cpu6502::ZP0() {
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

// Address Mode: Zero Page with X Offset
// Fundamentally the same as Zero Page addressing, but the contents of the X Register
// is added to the supplied single byte address. This is useful for iterating through
// ranges within the first page.
uint8_t cpu6502::ZPX() {
    addr_abs = (read(pc) + x);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

// Address Mode: Zero Page with Y Offset
// Same as above but uses Y Register for offset
uint8_t cpu6502::ZPY() {
    addr_abs = (read(pc) + y);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

// Address Mode: Relative
// This address mode is exclusive to branch instructions. The address
// must reside within -128 to +127 of the branch instruction, i.e.
// you cant directly branch to any address in the addressable range.
uint8_t cpu6502::REL() {
    addr_rel = read(pc);
    pc++;
    if (addr_rel & 0x80) {
        addr_rel |= 0xFF00;
    }
    return 0;
}

// Address Mode: Absolute
// A full 16-bit address is loaded and used
uint8_t cpu6502::ABS() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;

    return 0;
}

// Address Mode: Absolute with X Offset
// Fundamentally the same as absolute addressing, but the contents of the X Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t cpu6502::ABX() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += x;

    if ((addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }
    else {
        return 0;
    }
}

// Address Mode: Absolute with Y Offset
// Fundamentally the same as absolute addressing, but the contents of the Y Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t cpu6502::ABY() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }
    else {
        return 0;
    }
}

// Note: The next 3 address modes use indirection (aka Pointers!)

// Address Mode: Indirect
// The supplied 16-bit address is read to get the actual 16-bit address. This is
// instruction is unusual in that it has a bug in the hardware! To emulate its
// function accurately, we also need to emulate this bug. If the low byte of the
// supplied address is 0xFF, then to read the high byte of the actual address
// we need to cross a page boundary. This doesnt actually work on the chip as
// designed, instead it wraps back around in the same page, yielding an
// invalid actual address
uint8_t cpu6502::IND() {
    uint16_t ptr_lo = read(pc);
    pc++;
    uint16_t ptr_hi = read(pc);
    pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    if (ptr_lo == 0x00FF) {  // Simulate page boundary hardware bug
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    }
    else {  // Behave normally
        addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
    }

    return 0;
}

// Address Mode: Indirect X
// The supplied 8-bit address is offset by X Register to index
// a location in page 0x00. The actual 16-bit address is read
// from this location
uint8_t cpu6502::IZX() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;

    return 0;
}

// Address Mode: Indirect Y
// The supplied 8-bit address indexes a location in page 0x00. From
// here the actual 16-bit address is read, and the contents of
// Y Register is added to it to offset it. If the offset causes a
// change in page then an additional clock cycle is required.
uint8_t cpu6502::IZY() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }
    else {
        return 0;
    }
}

// This function sources the data used by the instruction into
// a convenient numeric variable. Some instructions dont have to
// fetch data as the source is implied by the instruction. For example
// "INX" increments the X register. There is no additional data
// required. For all other addressing modes, the data resides at
// the location held within addr_abs, so it is read from there.
// Immediate adress mode exploits this slightly, as that has
// set addr_abs = pc + 1, so it fetches the data from the
// next byte for example "LDA $FF" just loads the accumulator with
// 256, i.e. no far reaching memory fetch is required. "fetched"
// is a variable global to the CPU, and is set by calling this
// function. It also returns it for convenience.
uint8_t cpu6502::fetch() {
    if (!(lookup[opcode].addrmode == &cpu6502::IMP)) {
        fetched = read(addr_abs);
    }
    return fetched;
}

// Instruction implementations

// Add with carry
uint8_t cpu6502::ADC() {
    // Grab the data that we are adding to the accumulator
    fetch();

    // Add is performed in 16-bit domain for emulation to capture any
    // carry bit, which will exist in bit 8 of the 16-bit word
    temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);

    // The carry flag out exists in the high byte bit 0
    SetFlag(C, temp > 255);

    // The Zero flag is set if the result is 0
    SetFlag(Z, (temp & 0x00FF) == 0);

    // The signed Overflow flag is set based on all that up there! :D
    SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

    // The negative flag is set to the most significant bit of the result
    SetFlag(N, temp & 0x80);

    // Load the result into the accumulator (it's 8-bit dont forget!)
    a = temp & 0x00FF;

    // This instruction has the potential to require an additional clock cycle
    return 1;
}

// Subtract with carry
uint8_t cpu6502::SBC() {
    fetch();

    // Operating in 16-bit domain to capture carry out

    // We can invert the bottom 8 bits with bitwise xor
    uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

    // Notice this is exactly the same as addition from here!
    temp = (uint16_t)a + value + (uint16_t)GetFlag(C);
    SetFlag(C, temp & 0xFF00);
    SetFlag(Z, ((temp & 0x00FF) == 0));
    SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
    SetFlag(N, temp & 0x0080);
    a = temp & 0x00FF;
    return 1;
}

// Instruction: Bitwise Logic AND
// Function:    A = A & M
// Flags Out:   N, Z
uint8_t cpu6502::AND() {
    fetch();
    a = a & fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t cpu6502::ASL() {
    fetch();
    temp = (uint16_t)fetched << 1;
    SetFlag(C, (temp & 0xFF00) > 0);
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x80);
    if (lookup[opcode].addrmode == &cpu6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addr_abs, temp & 0x00FF);
    }
    return 0;
}

// Instruction: Branch if Carry Clear
// Function:    if(C == 0) pc = address
uint8_t cpu6502::BCC() {
    if (GetFlag(C) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

// Instruction: Branch if Carry Set
// Function:    if(C == 1) pc = address
uint8_t cpu6502::BCS() {
    if (GetFlag(C) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

// Instruction: Branch if Equal
// Function:    if(Z == 1) pc = address
uint8_t cpu6502::BEQ() {
    if (GetFlag(Z) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

// Instruction: Test Bits
// Function: Z = addr & A) S = addr_value & 0x8000 V = addr_value & 0x4000
uint8_t cpu6502::BIT() {
    fetch();
    temp = a & fetched;
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, fetched & (1 << 7));
    SetFlag(V, fetched & (1 << 6));
    return 0;
}

// Instruction: Branch if Negative
// Function:    if(N == 1) pc = address
uint8_t cpu6502::BMI() {
    if (GetFlag(N) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

// Instruction: Branch if Not Equal
// Function:    if(Z == 0) pc = address
uint8_t cpu6502::BNE() {
    if (GetFlag(Z) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

// Instruction: Branch if Positive
// Function:    if(N == 0) pc = address
uint8_t cpu6502::BPL() {
    if (GetFlag(N) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t cpu6502::BRK() {
    pc++;

    SetFlag(I, 1);
    write(0x0100 + sptr, (pc >> 8) & 0x00FF);
    sptr--;
    write(0x0100 + sptr, pc & 0x00FF);
    sptr--;

    SetFlag(B, 1);
    write(0x0100 + sptr, status);
    sptr--;
    SetFlag(B, 0);

    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}

// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) pc = address
uint8_t cpu6502::BVC() {
    if (GetFlag(V) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

// Instruction: Branch if Overflow Set
// Function:    if(V == 1) pc = address
uint8_t cpu6502::BVS() {
    if (GetFlag(V) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

// Instruction: Clear Carry Flag
// Function:    C = 0
uint8_t cpu6502::CLC() {
    SetFlag(C, false);
    return 0;
}

// Instruction: Clear Decimal Flag
// Function:    D = 0
uint8_t cpu6502::CLD() {
    SetFlag(D, false);
    return 0;
}

// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t cpu6502::CLI() {
    SetFlag(I, false);
    return 0;
}

// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t cpu6502::CLV() {
    SetFlag(V, false);
    return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t cpu6502::CMP() {
    fetch();
    temp = (uint16_t)a - (uint16_t)fetched;
    SetFlag(C, a >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 1;
}

// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t cpu6502::CPX() {
    fetch();
    temp = (uint16_t)x - (uint16_t)fetched;
    SetFlag(C, x >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t cpu6502::CPY() {
    fetch();
    temp = (uint16_t)y - (uint16_t)fetched;
    SetFlag(C, y >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t cpu6502::DEC() {
    fetch();
    temp = fetched - 1;
    write(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t cpu6502::DEX() {
    x--;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t cpu6502::DEY() {
    y--;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t cpu6502::EOR() {
    fetch();
    a = a ^ fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t cpu6502::INC() {
    fetch();
    temp = fetched + 1;
    write(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t cpu6502::INX() {
    x++;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t cpu6502::INY() {
    y++;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// Instruction: Jump To Location
// Function:    pc = address
uint8_t cpu6502::JMP() {
    pc = addr_abs;
    return 0;
}

// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t cpu6502::JSR() {
    pc--;

    write(0x0100 + sptr, (pc >> 8) & 0x00FF);
    sptr--;
    write(0x0100 + sptr, pc & 0x00FF);
    sptr--;

    pc = addr_abs;
    return 0;
}

// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t cpu6502::LDA() {
    fetch();
    a = fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t cpu6502::LDX() {
    fetch();
    x = fetched;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 1;
}

// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t cpu6502::LDY() {
    fetch();
    y = fetched;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 1;
}

uint8_t cpu6502::LSR() {
    fetch();
    SetFlag(C, fetched & 0x0001);
    temp = fetched >> 1;
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &cpu6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addr_abs, temp & 0x00FF);
    }
    return 0;
}

uint8_t cpu6502::NOP() {
    // Sadly not all NOPs are equal, Ive added a few here
    // based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
    // and will add more based on game compatibility, and ultimately
    // I'd like to cover all illegal opcodes too
    switch (opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
            break;
    }
    return 0;
}

// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t cpu6502::ORA() {
    fetch();
    a = a | fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Push Accumulator to Stack
// Function:    A -> stack
uint8_t cpu6502::PHA() {
    write(0x0100 + sptr, a);
    sptr--;
    return 0;
}

// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t cpu6502::PHP() {
    write(0x0100 + sptr, status | B | U);
    SetFlag(B, 0);
    SetFlag(U, 0);
    sptr--;
    return 0;
}

// Instruction: Pop Accumulator off Stack
// Function:    A <- stack
// Flags Out:   N, Z
uint8_t cpu6502::PLA() {
    sptr++;
    a = read(0x0100 + sptr);
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t cpu6502::PLP() {
    sptr++;
    status = read(0x0100 + sptr);
    SetFlag(U, 1);
    return 0;
}

uint8_t cpu6502::ROL() {
    fetch();
    temp = (uint16_t)(fetched << 1) | GetFlag(C);
    SetFlag(C, temp & 0xFF00);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &cpu6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addr_abs, temp & 0x00FF);
    }
    return 0;
}

uint8_t cpu6502::ROR() {
    fetch();
    temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
    SetFlag(C, fetched & 0x01);
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &cpu6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addr_abs, temp & 0x00FF);
    }
    return 0;
}

uint8_t cpu6502::RTI() {
    sptr++;
    status = read(0x0100 + sptr);
    status &= ~B;
    status &= ~U;

    sptr++;
    pc = (uint16_t)read(0x0100 + sptr);
    sptr++;
    pc |= (uint16_t)read(0x0100 + sptr) << 8;
    return 0;
}

uint8_t cpu6502::RTS() {
    sptr++;
    pc = (uint16_t)read(0x0100 + sptr);
    sptr++;
    pc |= (uint16_t)read(0x0100 + sptr) << 8;

    pc++;
    return 0;
}

// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t cpu6502::SEC() {
    SetFlag(C, true);
    return 0;
}

// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t cpu6502::SED() {
    SetFlag(D, true);
    return 0;
}

// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t cpu6502::SEI() {
    SetFlag(I, true);
    return 0;
}

// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t cpu6502::STA() {
    write(addr_abs, a);
    return 0;
}

// Instruction: Store X Register at Address
// Function:    M = X
uint8_t cpu6502::STX() {
    write(addr_abs, x);
    return 0;
}

// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t cpu6502::STY() {
    write(addr_abs, y);
    return 0;
}

// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t cpu6502::TAX() {
    x = a;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t cpu6502::TAY() {
    y = a;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t cpu6502::TSX() {
    x = sptr;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}

// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t cpu6502::TXA() {
    a = x;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t cpu6502::TXS() {
    sptr = x;
    return 0;
}

// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t cpu6502::TYA() {
    a = y;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

// This function captures illegal opcodes
uint8_t cpu6502::XXX() {
    return 0;
}

// Helper Functions

bool cpu6502::complete() {
    return cycles == 0;
}

// The disassembly function!
// Takes the instruction and turns it into human-readable (kinda) stuff.
std::map<uint16_t, std::string> cpu6502::disassemble(uint16_t nStart, uint16_t nStop) {
    uint32_t addr = nStart;
    uint8_t value = 0x00;
    uint8_t lo    = 0x00;
    uint8_t hi    = 0x00;
    std::map<uint16_t, std::string> mapLines;
    uint16_t line_addr = 0;

    // Lambda function to convert int into hex
    auto hex = [](uint32_t n, uint8_t d) {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4) {
            s[i] = "0123456789ABCDEF"[n & 0xF];
        }

        return s;
    };

    // Starting at the specified address we read an instruction byte, which lets us use
    // the lookup table to see how many bytes we need to read and what the addressing mode is.

    // As the instruction is decoded, a std::string is assembled with the output
    while (addr <= (uint32_t)nStop) {
        line_addr = addr;

        // Prefix line with instruction address
        std::string sInst = "$" + hex(addr, 4) + ": ";

        // Read instruction and get its readable name
        uint8_t opcode = bus->cpuRead(addr, true);
        addr++;
        sInst += lookup[opcode].name + " ";

        // Get operands from desired locations and form the instruction based upon its addressing mode.
        if (lookup[opcode].addrmode == &cpu6502::IMP) {
            sInst += "             {IMP}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IMM) {
            value = bus->cpuRead(addr, true);
            addr++;
            sInst += "#$" + hex(value, 2) + "         {IMM}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ZP0) {
            lo = bus->cpuRead(addr, true);
            addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + "          {ZP0}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ZPX) {
            lo = bus->cpuRead(addr, true);
            addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + ", X       {ZPX}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ZPY) {
            lo = bus->cpuRead(addr, true);
            addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + ", Y       {ZPY}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IZX) {
            lo = bus->cpuRead(addr, true);
            addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + ", X)     {IZX}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IZY) {
            lo = bus->cpuRead(addr, true);
            addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + "), Y     {IZY}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ABS) {
            lo = bus->cpuRead(addr, true);
            addr++;
            hi = bus->cpuRead(addr, true);
            addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + "        {ABS}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ABX) {
            lo = bus->cpuRead(addr, true);
            addr++;
            hi = bus->cpuRead(addr, true);
            addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X     {ABX}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ABY) {
            lo = bus->cpuRead(addr, true);
            addr++;
            hi = bus->cpuRead(addr, true);
            addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y     {ABY}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IND) {
            lo = bus->cpuRead(addr, true);
            addr++;
            hi = bus->cpuRead(addr, true);
            addr++;
            sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ")      {IND}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::REL) {
            value = bus->cpuRead(addr, true);
            addr++;
            sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "]  {REL}";
        }

        mapLines[line_addr] = sInst;
    }

    return mapLines;
}
