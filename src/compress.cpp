/*
 * @Descripttion: implement compress file in byte and bit for PA3
 * encode rule for final:
 * byte: 1 -- length for existing symbol in header|
 * 2~header end --header contain Tree structure and symbol|
 * 3~end-1 -- file code|
 * end -- the last byte contains the number of bit|
 * @version: 1.0
 * @Author: Kaixin Lin
 */
#include <fstream>
#include <iostream>
#include <stack>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"
using namespace std;
/**
 * @name: pseudoCompression
 * @msg: Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
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
    // if nextbyte is no the EOF, then read
    while ((nextByte = myfile.get()) != EOF) {
        nextChar = (unsigned char)nextByte;
        freqs[nextChar]++;
        temp.push_back(nextChar);
    }
    myfile.close();
    // build tree
    tree.build(freqs);

    out.open(outFileName, ios::binary);
    // write header
    for (int i = 0; i < 256; i++) {
        out << freqs[i] << endl;
    }
    // encode tree in byte
    for (int i = 0; i < temp.size(); i++) {
        tree.encode(temp[i], out);
    }
    out.close();
}
/**
 * @name: trueCompression
 * @msg: True compression with bitwise i/o and small header (final)
 */
void trueCompression(string inFileName, string outFileName) {
    ifstream myfile;
    HCTree tree;
    FileUtils fu;
    unsigned char nextChar;
    int nextByte, total = 0;
    vector<unsigned char> temp;
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

    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] != 0) total++;
    }

    tree.build(freqs);
    out.open(outFileName, ios::binary);

    // write header
    // length for existing symbol in header
    bos.writeChar((unsigned char)total);
    tree.encodeNode(bos);
    // encode
    for (int i = 0; i < temp.size(); i++) {
        tree.encode(temp[i], bos);
    }
    int extrabit = bos.getBits();
    // edge case
    if (extrabit != 8) bos.flush();

    bos.writeChar(extrabit);
    bos.flush();
    out.close();
}
/**
 * @name: extraCompression
 * @msg: extra compression with bitwise i/o and 2 byte compression(extra)
 */
void extraCompression(string inFileName, string outFileName) {
    ifstream myfile;
    HCTree tree;
    FileUtils fu;
    unsigned short nextShort;
    int nextByte, tempByte, total = 0;
    vector<unsigned short> temp;
    vector<unsigned int> freqs(65536);
    ofstream out;
    BitOutputStream bos(out);

    tree.flag_2Node = 1;
    bos.flag_2Node = 1;  // open write two node encode fuction

    // check file is empty
    if (fu.isEmptyFile(inFileName)) {
        out.open(outFileName);
        out.close();
        return;
    }

    myfile.open(inFileName, ios::binary);
    bool readEOF = false;
    int isOdd = 0;
    if ((nextByte = myfile.get()) != EOF) {
        if ((tempByte = myfile.get()) != EOF) {
            nextByte = (nextByte << 8);
            nextByte |= tempByte;
            nextShort = (unsigned short)nextByte;
            temp.push_back(nextShort);
            freqs[nextShort]++;
        } else {
            isOdd = 1;
            readEOF = true;
        }

    } else
        readEOF = true;
    while (readEOF == false) {
        if ((nextByte = myfile.get()) != EOF) {
            if ((tempByte = myfile.get()) != EOF) {
                nextByte = (nextByte << 8);
                nextByte |= tempByte;
                nextShort = (unsigned short)nextByte;
                temp.push_back(nextShort);
                freqs[nextShort]++;
            } else {
                isOdd = 1;
                readEOF = true;
            }
        } else
            readEOF = true;
    }
    myfile.close();

    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] != 0) total++;
    }

    tree.build(freqs);
    out.open(outFileName, ios::binary);

    // write header
    // first total header
    bos.writeShort((unsigned short)total);
    // write Is odd end
    bos.writeShort((unsigned short)isOdd);

    if (isOdd) bos.writeShort((unsigned short)nextByte);

    tree.encodeNode(bos);

    // encode
    for (int i = 0; i < temp.size(); i++) {
        tree.encode(temp[i], bos);
    }
    int extrabit = bos.getBits();
    // edge case
    if (extrabit != 16) bos.flush();

    bos.writeShort(extrabit);
    bos.flush();
    out.close();
}
/* Main program that runs the compress */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    bool isBlockOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "block", "Write output in two byte symbols instead of one byte",
        cxxopts::value<bool>(isBlockOutput))(
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

    if (isAsciiOutput)
        pseudoCompression(inFileName, outFileName);
    else
        extraCompression(inFileName, outFileName);
    // else if (isBlockOutput)
    //     extraCompression(inFileName, outFileName);
    // else
    //     trueCompression(inFileName, outFileName);
    return 0;
}