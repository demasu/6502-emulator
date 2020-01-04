#include "Mapper.h"

Mapper::Mapper(uint8_t prgBanks, uint8_t chrBanks) {
    nPRGBanks = prgBanks;
    nCHRBanks = chrBanks;

    //reset(); // Causes an error of [Pure virtual function called!]
}

Mapper::~Mapper() {}

void Mapper::reset() {}