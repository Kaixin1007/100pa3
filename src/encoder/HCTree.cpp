/*
 * @Descripttion: implemention the operation of Huffman Tree for compression and
 * uncompression in byte and bit for PA3
 * @version: 1.0
 * @Author: Kaixin Lin
 */
#include "HCTree.hpp"

/**
 * @name: deleteAll
 * @msg: deconstruction helper function
 */
void HCTree::deleteAll(HCNode*& root) {
    if (root == nullptr) return;
    deleteAll(root->c0);
    deleteAll(root->c1);
    delete (root);
}
/**
 * @name: ~HCTree
 * @msg: deconstruction for HCTree
 */
HCTree::~HCTree() {
    deleteAll(root);
    vector<HCNode*>().swap(leaves);
}

/**
 * @name: build
 * @msg: build Huffman by frequencies
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    my_quene forest;
    HCNode *least1, *least2, *next;
    // creat forest
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] != 0) {
            forest.push(new HCNode(freqs[i], (short)i));
        }
    }
    // edge cast for size = 0 and size = 1
    if (forest.size() == 0)
        return;
    else if (forest.size() == 1) {
        root = forest.top();
        next = new HCNode(root->count, root->symbol, 0, 0, root);
        leaves.push_back(next);
        root->c0 = next;
        return;
    }
    // loop until get a tree
    while (forest.size() > 1) {
        // get the min
        least1 = forest.top();
        forest.pop();
        least2 = forest.top();
        forest.pop();
        // update
        next = new HCNode(least1->count + least2->count, least2->symbol, least1,
                          least2);
        least1->p = least2->p = next;
        if (least1->c0 == 0 & least1->c1 == 0) leaves.push_back(least1);
        if (least2->c0 == 0 & least2->c1 == 0) leaves.push_back(least2);
        forest.push(next);
    }
    root = forest.top();
}

/**
 * @name: encode
 * @msg: encode stream in bit
 */
void HCTree::encode(short symbol, BitOutputStream& out) const {
    HCNode *parent, *node, *temp = root;
    stack<int> stk;
    int i;

    for (i = 0; i < leaves.size(); i++) {
        if (leaves[i]->symbol == symbol) {
            // find the symbol
            node = leaves[i];
            while (node != root) {
                parent = node->p;
                if (parent->c0 == node)
                    stk.push(0);

                else
                    stk.push(1);

                node = parent;
            }
            break;
        }
    }
    // output the code
    if (i <= leaves.size()) {
        while (!stk.empty()) {
            out.writeBit(stk.top());
            stk.pop();
        }
    }
}

/**
 * @name: encode
 * @msg: encode stream in byte
 */
void HCTree::encode(byte symbol, ostream& out) const {
    string bitStr;
    HCNode *parent, *node;
    for (int i = 0; i < leaves.size(); i++) {
        if (leaves[i]->symbol == symbol) {
            // find the symbol
            node = leaves[i];
            while (node != root) {
                parent = node->p;
                if (parent->c0 == node)
                    bitStr += '0';
                else
                    bitStr += '1';
                node = parent;
            }
            break;
        }
    }

    // output the code
    for (int i = 0; i < bitStr.size(); i++) {
        out << bitStr[bitStr.size() - i - 1];
    }
}

/**
 * @name: decode
 * @msg: decode stream in bit
 * @return: the decode byte
 */
short HCTree::decode(BitInputStream& in) const {
    HCNode* node = root;
    int nextByte;
    byte code;

    while (1) {
        // read bit
        nextByte = in.readBit();
        code = (unsigned short)nextByte;
        if (code == 0) {
            node = node->c0;
        } else {
            node = node->c1;
        }
        // if the node go to the root, break
        if (node->c0 == nullptr && node->c1 == nullptr) break;
    }

    return node->symbol;
}
/**
 * @name: decode
 * @msg: decode stream in byte
 * @return: the decode byte
 */
byte HCTree::decode(istream& in) const {
    HCNode* node = root;
    int nextByte;
    byte code;
    while (1) {
        nextByte = in.get();
        code = (unsigned char)nextByte;
        if (code == '0') {
            node = node->c0;
        } else {
            node = node->c1;
        }
        // if the node go to the root, break
        if (node->c0 == nullptr && node->c1 == nullptr) break;
    }

    return node->symbol;
}

/**
 * @name: leaveSize
 * @msg: get leaves' size
 * @return: leaves' size
 */
int HCTree::leaveSize() { return leaves.size(); }

/**
 * @name:getNode
 * @msg: get the leaves' ith node
 */
HCNode* HCTree::getNode(int i) { return leaves[i]; }

/**
 * @name: encodeNode
 * @msg: interator the Huffman tree to encode
 * for no-leaf node, we input 0, for leaf node, we input 1 and then
 * input the symbol(from left to right)
 */
void HCTree::encodeNode(BitOutputStream& out) { encodeNodeHelper(root, out); }

/**
 * @name: encodeNodeHelper
 * @msg: encodeNodeHelper function to iterator the tree
 */
void HCTree::encodeNodeHelper(HCNode*& node, BitOutputStream& out) {
    // leaf node
    if (node->c0 == nullptr && node->c1 == nullptr) {
        out.writeBit(1);
        if (flag_2Node == 1)
            out.writeShort(node->symbol);
        else
            out.writeChar(node->symbol);
    } else {
        // no leaf node
        out.writeBit(0);
        if (node->c0 != nullptr) encodeNodeHelper(node->c0, out);
        if (node->c1 != nullptr) encodeNodeHelper(node->c1, out);
    }
}

/**
 * @name: decodeNodeHelper
 * @msg: decode node helpfer function to iterator rebuild the tree
 */
HCNode* HCTree::decodeNodeHelper(BitInputStream& in, int max) {
    if (leaves.size() >= max) {
        return nullptr;
    }
    // leaf node
    if (in.readBit() == 1) {
        HCNode* node = new HCNode(0, in.readShort(), 0, 0, 0);
        leaves.push_back(node);
        return node;
    } else {
        // no leaf node
        HCNode* left = decodeNodeHelper(in, max);
        HCNode* right = decodeNodeHelper(in, max);
        return new HCNode(0, 0, left, right, 0);
    }
}

/**
 * @name: rebuild
 * @msg: rebuild the tree according the encode rule
 */
void HCTree::rebuild(BitInputStream& in, int max) {
    root = decodeNodeHelper(in, max);
}
