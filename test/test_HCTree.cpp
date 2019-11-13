/*
 * @Descripttion: Test file for HCTree
 * @version: 1.0
 * @Author: Kaixin Lin
 */
#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "HCTree.hpp"

using namespace std;
using namespace testing;

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;
    HCTree tree1;
    HCTree tree2;

  public:
    SimpleHCTreeFixture() {
        HCTree tree3;
        vector<unsigned int> freqs(256);
        tree3.build(freqs);
        vector<unsigned int> freqs1(256);
        freqs[','] = 1;
        freqs['p'] = 1;
        freqs['h'] = 2;
        freqs['k'] = 2;
        freqs['m'] = 2;
        freqs['n'] = 2;
        freqs['a'] = 3;
        freqs['u'] = 9;
        tree.build(freqs);
        freqs1['a'] = 5;
        tree2.build(freqs1);
        cout << tree.leaveSize();

        tree.getNode(0);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    BitOutputStream bos(os);
    tree.encode('u', os);
    tree.encode('u', bos);
    ASSERT_EQ(os.str(), "0");
    // ASSERT_EQ(bos.str(), "0");
    os.str("");
    tree.encode('m', os);
    ASSERT_EQ(os.str(), "1110");

    os.str("");
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "101");

    os.str("");
    tree.encode('p', os);
    ASSERT_EQ(os.str(), "1001");

    os.str("");
    tree.encode(',', os);
    ASSERT_EQ(os.str(), "1000");

    os.str("");
    tree.encode('n', os);
    ASSERT_EQ(os.str(), "1111");

    cout << endl;
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("0");
    stringstream ss;
    string bitsStr = "0";

    ss.str(bitsStr);
    BitInputStream bis(ss);
    ASSERT_EQ(tree.decode(is), 'u');
    ASSERT_EQ(tree.decode(bis), 'u');
    istringstream is1("1001");
    ASSERT_EQ(tree.decode(is1), 'p');
    bitsStr = "10010000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));
    ss.str("");
    ss.str(ascii);
    BitInputStream bis1(ss);
    ASSERT_EQ(tree.decode(bis1), 'p');

    istringstream is2("101");
    ASSERT_EQ(tree.decode(is2), 'a');

    bitsStr = "10100000";
    ascii = string(1, stoi(bitsStr, nullptr, 2));
    ss.str(ascii);
    BitInputStream bis2(ss);
    ASSERT_EQ(tree.decode(bis2), 'a');

    cout << endl;
}

TEST_F(SimpleHCTreeFixture, TEST_REBUILD) {
    ostringstream os;
    BitOutputStream bos(os);
    tree.encodeNode(bos);

    string bitsStr = "10010000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));
    stringstream ss;
    ss.str(ascii);
    BitInputStream bis(ss);
    tree1.rebuild(bis, 2);
}
