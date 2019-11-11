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
// unsigned int BitInputStream::readChar() {
//     unsigned char num = 0;
//     for (int i = 0; i < 8; i++) {
//         num |= (readBit() << (7 - i));
//     }

//     return num;
// }
int BitInputStream::getBits() { return nbits; }