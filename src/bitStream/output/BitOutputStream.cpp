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

