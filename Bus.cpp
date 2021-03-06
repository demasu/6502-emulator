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
    else if (addr == 0x4014) {
        // DMA transfer
        dma_page     = data;
        dma_addr     = 0x00;
        dma_transfer = true;
    }
    else if (addr >= 0x4016 && addr <= 0x4017) {
        // Controllers
        controller_state[addr & 0x0001] = controller[addr & 0x0001];
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
    else if (addr >= 0x4016 && addr <= 0x4017) {
        data = (controller_state[addr & 0x0001] & 0x80) > 0;
        controller_state[addr & 0x0001] <<= 1;
    }

    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cart = cartridge;
    ppu.ConnectCartridge(cartridge);
}

void Bus::reset() {
    cart->reset();
    cpu.reset();
    ppu.reset();
    nSystemClockCounter = 0;
    dma_page            = 0x00;
    dma_addr            = 0x00;
    dma_data            = 0x00;
    dma_dummy           = true;
    dma_transfer        = false;
}

void Bus::clock() {
    // Whatever calls this is responsible for the overall clocking frequency.
    // The PPU runs 3 times faster than the CPU clock, so we want to clock it every time
    ppu.clock();

    // Since the CPU is slower, call it every third time
    if (nSystemClockCounter % 3 == 0) {
        // Are we transfering to the DMA?
        if (dma_transfer) {
            // Need to wait until an even clock cycle
            if (dma_dummy) {
                if (nSystemClockCounter % 2 == 1) {
                    // Allow the DMA to start
                    dma_dummy = false;
                }
            }
            else {
                // Start DMA transfer
                if (nSystemClockCounter % 2 == 0) {
                    // On even clock cycles read from the CPU  bus
                    dma_data = cpuRead(dma_page << 8 | dma_addr);
                }
                else {
                    // On odd clock cycles write to PPU OAM
                    ppu.pOAM[dma_addr] = dma_data;
                    // Increment the address to get the next stuff
                    dma_addr++;
                    // If the address wraps around we know that we've written all 256 bytes so the
                    // DMA transfer is done
                    if (dma_addr == 0x00) {
                        dma_transfer = false;
                        dma_dummy    = true;
                    }
                }
            }
        }
        else {
            // No DMA transfer, so CPU can do whatever.
            cpu.clock();
        }
    }

    // The PPU is capable of emitting an interrupt to indicate the vertical blanking period has ended.
    // If it has we need to send that to the CPU.
    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nmi();
    }

    nSystemClockCounter++;
}
