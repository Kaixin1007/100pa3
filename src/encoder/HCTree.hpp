/*
 * @Descripttion: definition the operation of Huffman Tree for compression and
 * uncompression in byte and bit for PA3
 * @version: 1.0
 * @Author: Kaixin Lin
 */

#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"
using namespace std;
/**
 * @name: HCTree
 * @msg:  the class of Huffman Tree for compression and uncompression
 */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes
    typedef priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> my_quene;

    /**
     * @name: deleteAll
     * @msg: deconstruction helper function
     */
    void deleteAll(HCNode*& root);

  public:
    /**
     * @name: HCTree
     * @msg: construction for HCTree
     */
    HCTree() {
        root = nullptr;
        leaves = {};
    }

    /**
     * @name: ~HCTree
     * @msg: deconstruction for HCTree
     */
    ~HCTree();

    /**
     * @name: build
     * @msg: build Huffman by frequencies
     */
    void build(const vector<unsigned int>& freqs);

    /**
     * @name: encode
     * @msg: encode stream in bit
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /**
     * @name: encode
     * @msg: encode stream in byte
     */
    void encode(byte symbol, ostream& out) const;

    /**
     * @name: decode
     * @msg: decode stream in bit
     * @return: the decode byte
     */
    byte decode(BitInputStream& in) const;

    /**
     * @name: decode
     * @msg: decode stream in byte
     * @return: the decode byte
     */
    byte decode(istream& in) const;

    /**
     * @name: leaveSize
     * @msg: get leaves' size
     * @return: leaves' size
     */
    int leaveSize();

    /**
     * @name:getNode
     * @msg: get the leaves' ith node
     */
    HCNode* getNode(int i);

    /**
     * @name: encodeNode
     * @msg: interator the Huffman tree to encode
     * for no-leaf node, we input 0, for leaf node, we input 1 and then
     * input the symbol(from left to right)
     */
    void encodeNode(BitOutputStream& out);

    /**
     * @name: encodeNodeHelper
     * @msg: encodeNodeHelper function to iterator the tree
     */
    void encodeNodeHelper(HCNode*& node, BitOutputStream& out);

    /**
     * @name: decodeNodeHelper
     * @msg: decode node helpfer function to iterator rebuild the tree
     */
    HCNode* decodeNodeHelper(BitInputStream& in, int max);

    /**
     * @name: rebuild
     * @msg: rebuild the tree according the encode rule
     */
    void rebuild(BitInputStream& in, int max);
};

#endif  // HCTREE_HPP
