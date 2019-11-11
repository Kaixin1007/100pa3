/**
 * TODO: file header
 *
 * Author:
 */
#include "BitOutputStream.hpp"

/* TODO */
void BitOutputStream::flush() {
    out.put(buf);
    buf = 0;
    nbits = 0;
}

/* TODO */
void BitOutputStream::writeBit(int i) {
    if (nbits == 8) flush();
    buf = buf | (i << (7 - nbits));
    nbits++;
}

void BitOutputStream::writeChar(unsigned char num) {
    unsigned char temp = 0;
    for (int i = 0; i < 8; i++) {
        temp = pow(2, 7 - i);
        writeBit(num / temp);
        num = num % temp;
    }
}
void BitOutputStream::writenBit(unsigned char num, int n) {
    unsigned char temp = 0;
    for (int i = 0; i < n; i++) {
        temp = pow(2, n - 1 - i);
        writeBit(num / temp);
        num = num % temp;
    }
}
int BitOutputStream::getBits() { return nbits; }