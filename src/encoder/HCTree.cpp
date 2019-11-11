/**
 * TODO: file header
 *
 * Author:
 */
#include "HCTree.hpp"

void HCTree::deleteAll(HCNode*& root) {
    if (root == nullptr) return;
    deleteAll(root->c0);
    deleteAll(root->c1);
    delete (root);
}
/* TODO */
HCTree::~HCTree() {
    deleteAll(root);
    vector<HCNode*>().swap(leaves);
}

/* TODO */
void HCTree::build(const vector<unsigned int>& freqs) {
    my_quene forest;
    HCNode *least1, *least2, *next;
    // creat forest
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] != 0) {
            forest.push(new HCNode(freqs[i], (byte)i));
        }
    }
    if (forest.size() == 0)
        return;
    else if (forest.size() == 1) {
        root = forest.top();
        next = new HCNode(root->count, root->symbol, 0, 0, root);
        leaves.push_back(next);
        root->c0 = next;
        return;
    }
    while (forest.size() > 1) {
        // 得到最小的两个
        least1 = forest.top();
        forest.pop();
        least2 = forest.top();
        forest.pop();
        //更新
        next = new HCNode(least1->count + least2->count, least2->symbol, least1,
                          least2);
        least1->p = least2->p = next;
        if (least1->c0 == 0 & least1->c1 == 0) leaves.push_back(least1);
        if (least2->c0 == 0 & least2->c1 == 0) leaves.push_back(least2);
        forest.push(next);
    }
    root = forest.top();
}

/* TODO */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
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
    //如果有进入if里面
    if (i <= leaves.size()) {
        while (!stk.empty()) {
            out.writeBit(stk.top());
            stk.pop();
        }
    }
}

/* TODO */
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
    // output

    for (int i = 0; i < bitStr.size(); i++) {
        out << bitStr[bitStr.size() - i - 1];
    }
}
/* TODO */
byte HCTree::decode(BitInputStream& in) const {
    HCNode* node = root;
    int nextByte;
    byte code;
    while ((nextByte = in.readBit()) != EOF) {
        code = (unsigned char)nextByte;
        if (code == 0) {
            node = node->c0;
        } else {
            node = node->c1;
        }
        if (node->c0 == nullptr && node->c1 == nullptr) break;
    }
    // if (nextByte == EOF) isEnd = true;

    return node->symbol;
}
/* TODO */
byte HCTree::decode(istream& in) const {
    HCNode* node = root;
    int nextByte;
    byte code;
    while ((nextByte = in.get()) != EOF) {
        code = (unsigned char)nextByte;
        if (code == '0') {
            node = node->c0;
        } else {
            node = node->c1;
        }
        if (node->c0 == nullptr && node->c1 == nullptr) break;
    }
    // if (nextByte == EOF) isEnd = true;

    return node->symbol;
}
void HCTree::rebuild(string seq, byte symbol) {
    if (root == nullptr) {
        root = new HCNode(0, symbol, 0, 0, 0);
    }
    HCNode *node = root, *next;
    for (int i = 0; i < seq.length(); i++) {
        if (seq[i] == '0') {
            if (node->c0 == nullptr) {
                next = new HCNode(0, symbol, 0, 0, node);
                node->c0 = next;
            }
            node = node->c0;
        } else {
            if (node->c1 == nullptr) {
                next = new HCNode(0, symbol, 0, 0, node);
                node->c1 = next;
            }
            node = node->c1;
        }
    }
    leaves.push_back(node);
}

int HCTree::leaveSize() { return leaves.size(); }

HCNode* HCTree::getNode(int i) { return leaves[i]; }

void HCTree::encodeNode(BitOutputStream& out) { encodeNodeHelper(root, out); }

void HCTree::encodeNodeHelper(HCNode*& node, BitOutputStream& out) {
    if (node->c0 == nullptr && node->c1 == nullptr) {
        out.writeBit(1);

        out.writeChar(node->symbol);
    } else {
        out.writeBit(0);

        if (node->c0 != nullptr) encodeNodeHelper(node->c0, out);
        if (node->c1 != nullptr) encodeNodeHelper(node->c1, out);
    }
}
HCNode* HCTree::decodeNodeHelper(BitInputStream& in, int max) {
    if (leaves.size() >= max) {
        return nullptr;
    }
    if (in.readBit() == 1) {
        HCNode* node = new HCNode(0, in.readChar(), 0, 0, 0);
        leaves.push_back(node);
        return node;
    } else {
        HCNode* left = decodeNodeHelper(in, max);
        HCNode* right = decodeNodeHelper(in, max);
        return new HCNode(0, 0, left, right, 0);
    }
}
void HCTree::rebuild(BitInputStream& in, int max) {
    root = decodeNodeHelper(in, max);
}