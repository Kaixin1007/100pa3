/*
 * @Descripttion: implement uncompress file in byte and bit for PA3
 * @version: 1.0
 * @Author: Kaixin Lin
 */

#include <fstream>
#include <iostream>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

/**
 * @name: pseudoDecompression
 * @msg: Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(string inFileName, string outFileName) {
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
    // open file
    myfile.open(inFileName, ios::binary);
    // read frequency
    for (int i = 0; i < 256; i++) {
        string frequency;
        while ((nextByte = myfile.get()) != '\n') {
            // nextByte = myfile.get();
            nextChar = (unsigned char)nextByte;
            frequency += nextChar;
            freqs[i] = atoi(frequency.c_str());
        }
    }
    // build tree
    tree.build(freqs);
    // decode
    nextByte = tree.decode(myfile);
    while (!myfile.eof()) {
        nextChar = (unsigned char)nextByte;
        temp.push_back(nextChar);
        nextByte = tree.decode(myfile);
    }
    myfile.close();
    out.open(outFileName, ios::binary);
    // output
    for (int i = 0; i < temp.size(); i++) out << temp[i];
    out.close();
}

/**
 * @name: trueDecompression
 * @msg: True decompression with bitwise i/o and small header (final)
 */
void trueDecompression(string inFileName, string outFileName) {
    ifstream myfile;
    HCTree tree;
    FileUtils fu;
    unsigned char nextChar;
    int nextByte;
    vector<unsigned char> temp;
    ofstream out;

    // check file is empty
    if (fu.isEmptyFile(inFileName)) {
        out.open(outFileName);
        out.close();
        return;
    }

    myfile.open(inFileName, ios::binary);

    long begin, end;
    BitInputStream bis(myfile);
    begin = myfile.tellg();

    myfile.seekg(-1, myfile.end);
    char trailZero = myfile.peek();
    // read all the byte
    myfile.seekg(-1, myfile.end);
    end = myfile.tellg();
    myfile.seekg(begin);
    // cout << "size is: " << (end - begin) << " bytes.\n";

    // read total ascii in header
    unsigned int total = 0;
    for (int i = 0; i < 8; i++) {
        total |= (bis.readBit() << (7 - i));
        // cout << myfile.tellg() << endl;
    }
    if (total == 0) total = 256;
    // for (int i = 0; i < 7; i++) {
    //     bis.readBit();
    //     cout << myfile.tellg() << endl;
    // }

    tree.rebuild(bis, total);
    cout << "size is: " << (myfile.tellg() - begin) << " bytes.\n";
    while (end != myfile.tellg()) {
        // cout << myfile.tellg() << endl;
        nextByte = tree.decode(bis);
        nextChar = (unsigned char)nextByte;
        temp.push_back(nextChar);
    }
    // myfile.seekg(-1, myfile.cur);
    // while (!end) {
    //     nextByte = tree.decode(bis);
    //     nextChar = (unsigned char)nextByte;
    //     temp.push_back(nextChar);
    //     myfile.seekg(1, myfile.cur);
    //     end = (myfile.tellg() == myfile.end);
    //     myfile.seekg(-1, myfile.cur);
    // }
    if (trailZero == 0) {
        trailZero = 8;
        do {
            nextByte = tree.decode(bis);
            nextChar = (unsigned char)nextByte;
            temp.push_back(nextChar);
        } while (bis.getBits() < trailZero);
    } else {
        while (bis.getBits() < trailZero) {
            nextByte = tree.decode(bis);
            nextChar = (unsigned char)nextByte;
            temp.push_back(nextChar);
        }
    }

    myfile.close();
    out.open(outFileName, ios::binary);
    for (int i = 0; i < temp.size(); i++) out << temp[i];
    out.close();
}

/*  Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./uncompress",
                             "Uncompresses files using Huffman Encoding");
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

    FileUtils fu;
    // check if argument is vaild
    if (!fu.isValidFile(inFileName)) return -1;

    if (isAsciiOutput)
        pseudoDecompression(inFileName, outFileName);
    else
        trueDecompression(inFileName, outFileName);
    return 0;
}
