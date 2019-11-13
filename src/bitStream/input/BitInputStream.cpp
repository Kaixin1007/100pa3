/*
 * @Descripttion:bitwise operations to read each bit in the buffer.
 * @version: 1.0
 * @Author: Kaixin Lin
 */

#include "BitInputStream.hpp"

/**
 * @name: fill
 * @msg: put buffer to inputstream
 * @return:void
 */
void BitInputStream::fill() {
    byte temp;
    if (flag_2Node == 0)
        buf = in.get();
    else {
        buf = in.get();
        buf = buf << 8;
        temp = in.get();
        buf |= temp;
    }
}

/**
 * @name: readBit
 * @msg: read bit to buffer
 * @return: current bit
 */
unsigned int BitInputStream::readBit() {
    unsigned int bit;
    if (flag_2Node == 0) {
        if (nbits == 8 || nbits == -1) {
            nbits = 0;
            fill();
        }

        bit = (buf >> (7 - nbits)) & 0x01;

    } else {
        if (nbits == 16 || nbits == -1) {
            nbits = 0;
            fill();
        }
        bit = (buf >> (15 - nbits)) & 0x01;
    }
    nbits++;
    return bit;
}

/**
 * @name: readChar
 * @msg: read byte to buffer
 * @return: current byte
 */
unsigned int BitInputStream::readChar() {
    unsigned char num = 0;
    for (int i = 0; i < 8; i++) {
        num |= (readBit() << (7 - i));
    }

    return num;
}

/**
 * @name: readShort
 * @msg: read Short to buffer
 * @return: current Short
 */
unsigned int BitInputStream::readShort() {
    unsigned short num = 0;
    for (int i = 0; i < 16; i++) {
        num |= (readBit() << (15 - i));
    }

    return num;
}

/**
 * @name: getBits
 * @msg: get the value of nbits
 */
int BitInputStream::getBits() { return nbits; }