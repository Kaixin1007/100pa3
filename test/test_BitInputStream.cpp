#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include "BitInputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitInputStreamTests, SIMPLE_TEST) {
    string bitsStr = "10010000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ss;
    ss.str(ascii);
    BitInputStream bis(ss);
    ASSERT_EQ(144, bis.readChar());
    ss.str(ascii);
    ASSERT_EQ(1, bis.readBit());
    // ASSERT_EQ(0, bis.readBit());
    // ASSERT_EQ(0, bis.readBit());
    // ASSERT_EQ(1, bis.readBit());
}