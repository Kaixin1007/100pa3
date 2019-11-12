/*
 * @Descripttion:bitwise operations to write each bit in the buffer.
 * @version: 1.0
 * @Author: Kaixin Lin
 */

#include "BitOutputStream.hpp"

/**
 * @name: flush
 * @msg: put the buffer to outputstream
 * @return: void
 */
void BitOutputStream::flush() {
    out.put(buf);
    buf = 0;
    nbits = 0;
}

/**
 * @name: writeBit
 * @msg: write bit to buffer
 */
void BitOutputStream::writeBit(int i) {
    if (flag_2Node == 0) {
        if (nbits == 8) flush();
        buf = buf | (i << (7 - nbits));
    } else {
        if (nbits == 16) flush();
        buf = buf | (i << (15 - nbits));
    }
    nbits++;
}
/**
 * @name: writeChar
 * @msg: write byte to buffer
 */
void BitOutputStream::writeChar(unsigned char num) {
    unsigned char temp = 0;
    for (int i = 0; i < 8; i++) {
        temp = pow(2, 7 - i);
        writeBit(num / temp);
        num = num % temp;
    }
}

/**
 * @name: getBits
 * @msg: get the value of nbits
 */
int BitOutputStream::getBits() { return nbits; }

/**
 * @name: writeChar
 * @msg: write byte to buffer
 */
void BitOutputStream::writeShort(unsigned short num) {
    unsigned char temp = 0;
    for (int i = 0; i < 16; i++) {
        temp = pow(2, 15 - i);
        writeBit(num / temp);
        num = num % temp;
    }
}