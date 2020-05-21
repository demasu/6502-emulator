#include "Cartridge.h"
#include <iostream>

Cartridge::Cartridge(const std::string &sFileName) {
    // iNES format header
    struct sHeader {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    bImageValid = false;

    std::ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);
    if (ifs.is_open()) {
        // Read header
        ifs.read((char *)&header, sizeof(sHeader));

        // If a "trainer" exists we need to read past it
        if (header.mapper1 & 0x04) {
            ifs.seekg(512, std::ios_base::cur);
        }

        auto hex = [](uint32_t n, uint8_t d) {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4) {
                s[i] = "0123456789ABCDEF"[n & 0xF];
            }

            return s;
        };
        std::cerr << "Name portion of the header is: [" << header.name << "]" << std::endl;
        std::cerr << "prg_rom_chunks of the header is: [" << hex(header.prg_rom_chunks, 2) << "]" << std::endl;
        std::cerr << "chr_rom_chunks of the header is: [" << hex(header.chr_rom_chunks, 2) << "]" << std::endl;
        std::cerr << "mapper1 of the header is: [" << hex(header.mapper1, 2) << "]" << std::endl;
        std::cerr << "mapper2 of the header is: [" << hex(header.mapper2, 2) << "]" << std::endl;
        std::cerr << "prg_ram_size of the header is: [" << hex(header.prg_ram_size, 2) << "]" << std::endl;
        std::cerr << "tv_system1 of the header is: [" << hex(header.tv_system1, 2) << "]" << std::endl;
        std::cerr << "tv_system2 of the header is: [" << hex(header.tv_system2, 2) << "]" << std::endl;

        // Determine mapper ID
        std::cerr << "First half of the mapper is: [" << hex( ((header.mapper2 >> 4) << 4), 4) << "]" << std::endl;
        std::cerr << "Last  half of the mapper is: [" << hex( (header.mapper1 >> 4), 4)        << "]" << std::endl;
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        mirror    = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        uint8_t nFileType = 1;

        if (nFileType == 0) {
        }
        if (nFileType == 1) {
            nPRGBanks = header.prg_rom_chunks;
            vPRGMemory.resize(nPRGBanks * 16384);
            ifs.read((char *)vPRGMemory.data(), vPRGMemory.size());
            std::cerr << "First four bytes of PRG Memory are: [" << hex(vPRGMemory.at(0), 2) << " " << hex(vPRGMemory.at(1), 2) << " " << hex(vPRGMemory.at(2), 2) << " " << hex(vPRGMemory.at(3), 2) << "]" << std::endl;
            std::cerr << "Last four bytes of PRG Memory are: [" << hex(vPRGMemory.at(0x3FFC), 2) << " " << hex(vPRGMemory.at(0x3FFD), 2) << " " << hex(vPRGMemory.at(0x3FFE), 2) << " " << hex(vPRGMemory.at(0x3FFF), 2) << "]" << std::endl;
            std::cerr << "Size of vPRGMemory is: [" << hex(vPRGMemory.size(), 4) << "]" << std::endl;
            std::cerr << "Max size is: [" << hex(vPRGMemory.max_size(), 4) << "]" << std::endl;
            std::cerr << "Capacity is: [" << hex(vPRGMemory.capacity(), 4) << "]" << std::endl;

            std::cerr << "PRG Memory is:" << std::endl;
            for ( int i = 0; i < vPRGMemory.size(); i += 16 ) {
                std::cerr << "0x" << hex(i, 4) << ": ";
                for ( int y = 0; y < 16; y++ ) {
                    std::cerr << hex(vPRGMemory.at(y+i), 2) << " ";
                }
                std::cerr << std::endl;
            }
            //for ( auto &d : vPRGMemory ) {
            //}

            nCHRBanks = header.chr_rom_chunks;
            if (nCHRBanks == 0) {
                vCHRMemory.resize(8192);
            }
            else {
                vCHRMemory.resize(nCHRBanks * 8192);
            }
            ifs.read((char *)vCHRMemory.data(), vCHRMemory.size());
            std::cerr << "First four bytes of CHR Memory are: [" << hex(vCHRMemory.at(0), 2) << " " << hex(vCHRMemory.at(1), 2) << " " << hex(vCHRMemory.at(2), 2) << " " << hex(vCHRMemory.at(3), 2) << "]" << std::endl;
        }
        if (nFileType == 2) {
        }

        // Load appropriate mapper
        std::cerr << "Detecting mapper type" << std::endl;
        std::cerr << "Mapper ID is: [" << std::to_string(nMapperID) << std::endl;
        switch (nMapperID) {
            case 0:
                std::cerr << "Mapper is mapper 0" << std::endl;
                pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
                break;
        }

        bImageValid = true;
        ifs.close();
    }
}

Cartridge::~Cartridge() {}

bool Cartridge::ImageValid() {
    return bImageValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data) {
    auto hex = [](uint32_t n, uint8_t d) {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4) {
            s[i] = "0123456789ABCDEF"[n & 0xF];
        }

        return s;
    };
    //std::cerr << "Address is: [" << hex(addr, 4) << "]" << std::endl;
    uint32_t mapped_addr = 0;
    //std::cerr << "Calling cpuMapRead on the mapper with address and mapped address" << std::endl;
    if (pMapper->cpuMapRead(addr, mapped_addr)) {
        if ( addr == 0xFFFC || addr == 0xFFFD ) {
            std::cerr << "Mapped address is: [" << hex(mapped_addr, 4) << "]" << std::endl;
            std::cerr << "Data at " << hex(mapped_addr, 4) << " is: [" << hex(vPRGMemory[mapped_addr], 2) << "]" << std::endl;
        }
        data = vPRGMemory[mapped_addr];
        return true;
    }
    else {
        return false;
    }
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapWrite(addr, mapped_addr, data)) {
        vPRGMemory[mapped_addr] = data;
        return true;
    }
    else {
        return false;
    }
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t &data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        data = vCHRMemory[mapped_addr];
        return true;
    }
    else {
        return false;
    }
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapWrite(addr, mapped_addr)) {
        vCHRMemory[mapped_addr] = data;
        return true;
    }
    else {
        return false;
    }
}

void Cartridge::reset() {
    if (pMapper != nullptr) {
        pMapper->reset();
    }
}
