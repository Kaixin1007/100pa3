/*
 * @Descripttion:
 * @version: 1.0
 * @Author: Kaixin Lin
 */
/*
 * @Descripttion:
 * @version: 1.0
 * @Author: Kaixin Lin
 */
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
    ASSERT_EQ(bis.getBits(), -1);
    ASSERT_EQ(144, bis.readChar());
    ss.str(ascii);
    ASSERT_EQ(1, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
}
TEST(BitInputStreamTests, 2NODE_TEST) {
    // string bitsStr = "1000000010000000";
    // string ascii = string(1, stoi(bitsStr, nullptr, 2));

    // stringstream ss;
    // ss.str(ascii);
    // BitInputStream bis(ss);
    // bis.flag_2Node = 1;
    // ASSERT_EQ(bis.getBits(), -1);
}
