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

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs[','] = 1;
        freqs['p'] = 1;
        freqs['h'] = 2;
        freqs['k'] = 2;
        freqs['m'] = 2;
        freqs['n'] = 2;
        freqs['a'] = 3;
        freqs['u'] = 9;
        tree.build(freqs);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('u', os);
    ASSERT_EQ(os.str(), "0");

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
    ASSERT_EQ(tree.decode(is), 'u');

    istringstream is1("1001");
    ASSERT_EQ(tree.decode(is1), 'p');

    istringstream is2("101");
    ASSERT_EQ(tree.decode(is2), 'a');
    cout << endl;
}
