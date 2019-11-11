#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include "BitOutputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitOutputStreamTests, SIMPLE_TEST) {
    stringstream ss;
    BitOutputStream bos(ss);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.flush();
    string bitsStr = "11000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
    bos.writeChar(192);
    ASSERT_EQ(ss.get(), asciiVal);
}