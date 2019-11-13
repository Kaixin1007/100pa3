/*
 * @Descripttion: test file for bitoutStream
 * @version: 1.0
 * @Author: Kaixin Lin
 */
#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include "BitOutputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitOutputStreamTests, SIMPLE_TEST) {
    stringstream ss;
    BitOutputStream bos(ss);
    ASSERT_EQ(bos.getBits(), 0);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.flush();
    string bitsStr = "11000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
    bos.writeChar(0);
    bos.flush();
    ASSERT_EQ(ss.get(), 0);
}
TEST(BitOutputStreamTests, TWOSMBOL_TEST) {
    stringstream ss;
    BitOutputStream bos(ss);
    bos.flag_2Node = 1;
    bos.writeBit(1);
    bos.writeBit(0);
    bos.flush();
    string bitsStr = "0000000010000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
    bos.writeShort(0);
    bos.flush();
    ASSERT_EQ(ss.get(), 0);
}