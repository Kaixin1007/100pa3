/**
 * TODO: file header
 *
 * Author:
 */
#include <fstream>
#include <iostream>
#include <stack>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"
using namespace std;

void pseudoCompression(string inFileName, string outFileName) {
    ifstream myfile;
    HCTree tree;
    FileUtils fu;
    unsigned char nextChar;
    int nextByte;
    vector<unsigned char> temp;
    vector<unsigned int> freqs(256);
    ofstream out;
    // check file is empty
    if (fu.isEmptyFile(inFileName)) {
        out.open(outFileName);
        out.close();
        return;
    }

    myfile.open(inFileName, ios::binary);

    while ((nextByte = myfile.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        freqs[nextChar]++;
        temp.push_back(nextChar);
    }
    myfile.close();

    tree.build(freqs);

    out.open(outFileName, ios::binary);
    // write header
    for (int i = 0; i < 256; i++) {
        out << freqs[i] << endl;
    }
    for (int i = 0; i < temp.size(); i++) {
        tree.encode(temp[i], out);
    }
    out.close();
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(string inFileName, string outFileName) {
    ifstream myfile;
    HCTree tree;
    FileUtils fu;
    unsigned char nextChar, height;
    int nextByte, total = 0;
    vector<unsigned char> temp, stk_vec;
    vector<unsigned int> freqs(256);
    ofstream out;
    BitOutputStream bos(out);
    // check file is empty
    if (fu.isEmptyFile(inFileName)) {
        out.open(outFileName);
        out.close();
        return;
    }

    myfile.open(inFileName, ios::binary);

    while ((nextByte = myfile.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        freqs[nextChar]++;
        temp.push_back(nextChar);
    }
    myfile.close();
    // 统计ASCII码中不为0的频率个数
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] != 0) total++;
    }

    tree.build(freqs);
    out.open(outFileName, ios::binary);

    // write header
    //写个数  总个数 最长长度
    bos.writeChar((unsigned char)total);
    tree.encodeNode(bos);

    for (int i = 0; i < temp.size(); i++) {
        tree.encode(temp[i], bos);
    }
    int extrabit = bos.getBits();
    bos.flush();
    //当好满的时候会自动刷新 所以此时最后一位为0 不用更新
    if (extrabit != 0) bos.writeChar(extrabit);
    out.close();
    //写最长高度
    // height = (unsigned char)tree.getDepth();
    // bos.writeChar(height);

    // for (int i = 0; i < tree.leaveSize(); i++) {
    //     stack<char> stk;
    //     HCNode *parent, *node = tree.getNode(i);
    //     char num_stk;
    //     //写第i个ascii码
    //     bos.writeChar(node->symbol);
    //     while (node->p != nullptr) {
    //         parent = node->p;
    //         if (parent->c0 == node)
    //             stk.push(0);
    //         else
    //             stk.push(1);

    //         node = parent;
    //     }

    //     for (int i = 0; i < stk.size(); i++) {
    //         num_stk = stk.top();
    //         stk.pop();
    //         if (num_stk == 1) {
    //             stk_vec.push_back('1');
    //         } else
    //             stk_vec.push_back('0');
    //     }
    //     //写对应树的码
    //     for (int i = 0; i < height; i++) {
    //         if (i > stk_vec.size())
    //             bos.writeBit(0);
    //         else
    //             bos.writeBit(stk_vec[i]);
    //     }
    // }
}

/* TODO: Main program that runs the compress */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");
    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }

    // check if argument is vaild
    FileUtils fu;
    if (!fu.isValidFile(inFileName)) return -1;
    // pseudoCompression(inFileName, outFileName);
    // trueCompression(inFileName, outFileName);
    if (isAsciiOutput)
        pseudoCompression(inFileName, outFileName);
    else
        trueCompression(inFileName, outFileName);
}