/*
 * @Descripttion:bitwise operations to read each bit in the buffer.
 * @version: 1.0
 * @Author: Kaixin Lin
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/**
 * @name: BitInputStream
 * @msg: BitInputStream class for bit operations
 */
class BitInputStream {
  private:
    char buf;     // one byte buffer of bits
    int nbits;    // number of bits have been writen to buf
    istream& in;  // reference to the input stream to use

  public:
    int flag_2Node;
    /**
     * @name: BitInputStream
     * @msg: construction function for BitInputStream
     */
    explicit BitInputStream(istream& is) : in(is) {
        nbits = -1;
        buf = 0;
        flag_2Node = 0;
    };

    /**
     * @name: fill
     * @msg: put buffer to inputstream
     * @return:void
     */
    void fill();

    /**
     * @name: readBit
     * @msg: read bit to buffer
     * @return: current bit
     */
    unsigned int readBit();

    /**
     * @name: readChar
     * @msg: read byte to buffer
     * @return: current byte
     */
    unsigned int readChar();
    /**
     * @name: getBits
     * @msg: get the value of nbits
     */
    int getBits();

    /**
     * @name: readShort
     * @msg: read Short to buffer
     * @return: current Short
     */
    unsigned int readShort();
};

#endif