/**
 * TODO: file header
 *
 * Author:
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() { buf = in.get(); }

/* TODO */
unsigned int BitInputStream::readBit() {
    if (nbits == 8 || nbits == -1) {
        nbits = 0;
        fill();
    }

    unsigned int bit = (buf >> (7 - nbits)) & 0x01;
    nbits++;
    return bit;
}

bool BitInputStream::eof() {
    if (buf == -1) {
        buf = 0;
        return true;
    }
    return false;
}