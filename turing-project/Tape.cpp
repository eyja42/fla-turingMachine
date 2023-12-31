#include "Tape.h"
#include <cstdlib>

char &Tape::operator[](int index) {
    if (abs(index) - 1 >= size)
        resize();
    if (index > 0) {
        return tapePositive[index - 1];
    } else if (index == 0) {
        return zero_c;
    } else {
        return tapeNegative[-index - 1];
    }
}

const char &Tape::operator[](int index) const {
    if (index > 0) {
        return tapePositive[index - 1];
    } else if (index == 0) {
        return zero_c;
    } else {
        return tapeNegative[-index - 1];
    }
}

void Tape::resize() {
    tapePositive.resize(this->size << 1, spaceSymbol);
    tapeNegative.resize(this->size << 1, spaceSymbol);
    this->size <<= 1;
}
