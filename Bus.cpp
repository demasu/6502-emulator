#include "Bus.h"

Bus::Bus() {
    // Connect CPU to the communication bus
    cpu.ConnectBus(this);
}

Bus::~Bus() {}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    if (cart->cpuWrite(addr, data)) {
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // System RAM
        cpuRam[addr & 0x07FF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        // PPU Address range
        ppu.cpuWrite(addr & 0x0007, data);
    }
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) {
    uint8_t data = 0x00;
    if (cart->cpuRead(addr, data)) {
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // System RAM
        data = cpuRam[addr & 0x07FF];
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        data = ppu.cpuRead(addr & 0x0007, bReadOnly);
    }

    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cart = cartridge;
    ppu.ConnectCartridge(cartridge);
}

void Bus::reset() {
    cpu.reset();
    nSystemClockCounter = 0;
}

void Bus::clock() {
    // Whatever calls this is responsible for the overall clocking frequency.
    // The PPU runs 3 times faster than the CPU clock, so we want to clock it every time
    ppu.clock();

    // Since the CPU is slower, call it every third time
    if (nSystemClockCounter % 3 == 0) {
        cpu.clock();
    }

    nSystemClockCounter++;
}
