#pragma once
#include <map>
#include <string>
#include <vector>

//#define LOGMODE

#ifdef LOGMODE
    #include <stdio.h>
#endif

class Bus;
class GenericBus;

class cpu6502 {
   public:
    cpu6502();
    ~cpu6502();

    uint8_t a      = 0x00;    // Accumulator Register
    uint8_t x      = 0x00;    // X Register
    uint8_t y      = 0x00;    // Y Register
    uint8_t sptr   = 0x00;    // Stack pointer
    uint16_t pc    = 0x0000;  // Program counter
    uint8_t status = 0x00;    // Status register

    void reset();
    void irq();
    void nmi();
    void clock();

    bool complete();
    void ConnectBus(Bus *n) { bus = n; }
    //void ConnectBus(GenericBus *n) { bus = n; }

    std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);

    enum FLAGS6502 {
        C = (1 << 0),  // Carry
        Z = (1 << 1),  // Zero
        I = (1 << 2),  // Disable Interrupts
        D = (1 << 3),  // Decimal Mode
        B = (1 << 4),  // Break
        U = (1 << 5),  // Unused
        V = (1 << 6),  // Overflow
        N = (1 << 7),  // Negative
    };

   private:
    // Convenience functions for the status register
    uint8_t GetFlag(FLAGS6502 f);
    void SetFlag(FLAGS6502 f, bool v);

    // Convenience variables to help with emulation
    uint8_t fetched      = 0x00;    // Input value to ALU
    uint16_t temp        = 0x0000;  // Convience variable
    uint16_t addr_abs    = 0x0000;  // Memory addresses for use
    uint16_t addr_rel    = 0x0000;  // Relative address for branching instructions
    uint8_t opcode       = 0x00;    // The instruction byte
    uint8_t cycles       = 0;       // How many cycles the instruction has remaining
    uint32_t clock_count = 0;       // Global accumulation of the number of clock cycles

    // Used to get the STP command to work as it does in real life
    bool isRunning = true;

    // Links to the bus
    Bus *bus = nullptr;
    uint8_t read(uint16_t a);
    void write(uint16_t a, uint8_t d);

    // Function to fetch the next bit of data
    // Will decide where it is based on the addressing mode
    uint8_t fetch();

    // This structure and vector are used to store the opcode translation table.
    // The opcodes are stored in numerical order so they can be looked up without
    // decoding anything.
    // Each entry holds:
    //      Mnemonic: A word (or word-like) representation of the instruction
    //      Opcode function: A function pointer to the implementation of the opcode
    //      Opcode address mode: A function pointer to the implementation of the addressing mechanism for the instruction
    //      Cycle count: An integer that represents the lowest number of clock cycles for the instruction
    struct INSTRUCTION {
        std::string name;
        uint8_t (cpu6502::*operate)(void)  = nullptr;
        uint8_t (cpu6502::*addrmode)(void) = nullptr;
        uint8_t cycles                     = 0;
    };
    std::vector<INSTRUCTION> lookup;

    // Addressing modes
    uint8_t IMP();  // Implicit
    uint8_t IMM();  // Immediate
    uint8_t ZP0();  // Zero page
    uint8_t ZPX();  // Zero page X offset
    uint8_t ZPY();  // Zero page Y offset
    uint8_t REL();  // Relative
    uint8_t ABS();  // Absolute
    uint8_t ABX();  // Absolute X offset
    uint8_t ABY();  // Absolute Y offset
    uint8_t IND();  // Indirect
    uint8_t IZX();  // Indexed indirect
    uint8_t IZY();  // Indirect indexed

    // Opcodes
    uint8_t ADC();  // Add with carry
    uint8_t AND();  // Bitwise AND with accumulator
    uint8_t ASL();  // Arithmetic shift left
    uint8_t BCC();  // Branch on carry clear
    uint8_t BCS();  // Branch on carry set
    uint8_t BEQ();  // Branch on equal
    uint8_t BIT();  // Test bits
    uint8_t BMI();  // Branch on minus
    uint8_t BNE();  // Branch on not equal
    uint8_t BPL();  // Branch on plus
    uint8_t BRK();  // Break
    uint8_t BVC();  // Branch on overflow clear
    uint8_t BVS();  // Branch on overflow set
    uint8_t CLC();  // Clear carry
    uint8_t CLD();  // Clear decimal
    uint8_t CLI();  // Clear interrupt
    uint8_t CLV();  // Clear overflow
    uint8_t CMP();  // Compare accumulator
    uint8_t CPX();  // Compare X register
    uint8_t CPY();  // Compare Y register
    uint8_t DEC();  // Decrement memory
    uint8_t DEX();  // Decrement X
    uint8_t DEY();  // Decrement Y
    uint8_t EOR();  // Bitwise exclusive OR
    uint8_t INC();  // Increment memory
    uint8_t INX();  // Increment X register
    uint8_t INY();  // Increment Y register
    uint8_t JMP();  // Jump
    uint8_t JSR();  // Jump to subroutine
    uint8_t LDA();  // Load Accumulator
    uint8_t LDX();  // Load X register
    uint8_t LDY();  // Load Y register
    uint8_t LSR();  // Logical shift right
    uint8_t NOP();  // No operation
    uint8_t ORA();  // Bitwise OR with Accumulator
    uint8_t PHA();  // Push Accumulator
    uint8_t PHP();  // Push Processor status
    uint8_t PLA();  // Pull Accumulator
    uint8_t PLP();  // Pull Processor status
    uint8_t ROL();  // Rotate left
    uint8_t ROR();  // Rotate right
    uint8_t RTI();  // Return from interrupt
    uint8_t RTS();  // Return from subroutine
    uint8_t SBC();  // Subtract with carry
    uint8_t SEC();  // Set Carry
    uint8_t SED();  // Set Decimal
    uint8_t SEI();  // Set Interrupt
    uint8_t STA();  // Store Accumulator
    uint8_t STX();  // Store X register
    uint8_t STY();  // Store Y register
    uint8_t TAX();  // Transfer A to X
    uint8_t TAY();  // Transfer A to Y
    uint8_t TSX();  // Transfer stack pointer to X
    uint8_t TXA();  // Transfer X to A
    uint8_t TXS();  // Transfer X to stack pointer
    uint8_t TYA();  // Transfer Y to A

    // Unofficial opcode definitions
    uint8_t SLO();
    uint8_t ANC();
    uint8_t RLA();
    uint8_t SRE();
    uint8_t RRA();
    uint8_t SAX();
    uint8_t XAA();
    uint8_t LAX();
    uint8_t DCP();
    uint8_t ISB();  // AKA ISC
    uint8_t STP();
    uint8_t AXS();
    uint8_t ARR();
    uint8_t AHX();
    uint8_t TAS();
    uint8_t SHX();
    uint8_t LAS();
    uint8_t ALR();

    // Unofficial opcodes that aren't yet defined call this, which acts as a NOP
    uint8_t XXX();

#ifdef LOGMODE
    FILE *logfile = nullptr;
#endif
};
