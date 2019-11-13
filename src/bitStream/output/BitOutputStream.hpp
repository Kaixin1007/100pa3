/*
 * @Descripttion:bitwise operations to write each bit in the buffer.
 * @version: 1.0
 * @Author: Kaixin Lin
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <cmath>
#include <iostream>
typedef unsigned char byte;

using namespace std;

/**
 * @name: BitOutputStream
 * @msg: BitOutputStream class for bit operations
 */
class BitOutputStream {
  private:
    short buf;     // one byte buffer of bits
    int nbits;     // number of bits have been writen to buf
    ostream& out;  // reference to the output stream to use

  public:
    int flag_2Node;
    /**
     * @name: BitOutputStream
     * @msg: constructor for BitOutputStream
     */
    explicit BitOutputStream(ostream& os) : out(os) {
        buf = 0;
        nbits = 0;
        flag_2Node = 0;
    }

    /**
     * @name: flush
     * @msg: put the buffer to outputstream
     * @return: void
     */
    void flush();

    /**
     * @name: writeBit
     * @msg: write bit to buffer
     */
    void writeBit(int i);

    /**
     * @name: writeChar
     * @msg: write byte to buffer
     */
    void writeChar(unsigned char num);

    /**
     * @name: getBits
     * @msg: get the value of nbits
     */
    int getBits();

    /**
     * @name: writeShort
     * @msg: write short to buffer
     */
    void writeShort(unsigned short num);
};

#endif
