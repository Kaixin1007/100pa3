/**
 * TODO: file header
 *
 * Author:
 */
#include <fstream>
#include <iostream>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

/* TODO: Pseudo decompression with ascii encoding and naive header (checkpoint)
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

    myfile.open(inFileName, ios::binary);
    for (int i = 0; i < 256; i++) {
        string frequency;
        while ((nextByte = myfile.get()) != '\n') {
            // nextByte = myfile.get();
            nextChar = (unsigned char)nextByte;
            frequency += nextChar;
            freqs[i] = atoi(frequency.c_str());
        }
    }
    tree.build(freqs);
    nextByte = tree.decode(myfile);
    while (!myfile.eof()) {
        nextChar = (unsigned char)nextByte;
        temp.push_back(nextChar);
        nextByte = tree.decode(myfile);
        // cout << nextByte << endl;
    }
    myfile.close();
    out.open(outFileName, ios::binary);
    for (int i = 0; i < temp.size(); i++) out << temp[i];
    out.close();
}

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {
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
    for (int i = 0; i < 256; i++) {
        string frequency;
        while ((nextByte = myfile.get()) != '\n') {
            // nextByte = myfile.get();
            nextChar = (unsigned char)nextByte;
            frequency += nextChar;
            freqs[i] = atoi(frequency.c_str());
        }
    }
    BitInputStream bis(myfile);
    tree.build(freqs);
    nextByte = tree.decode(bis);
    while (!bis.eof()) {
        nextChar = (unsigned char)nextByte;
        temp.push_back(nextChar);
        nextByte = tree.decode(bis);
        // cout << nextByte << endl;
    }
    myfile.close();
    out.open(outFileName, ios::binary);
    for (int i = 0; i < temp.size(); i++) out << temp[i];
    out.close();
}

/* TODO: Main program that runs the uncompress */
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
    // pseudoDecompression(argv[1], argv[2]);
    // check if argument is vaild
    if (!fu.isValidFile(inFileName)) return -1;
    trueDecompression(inFileName, outFileName);
    // if (isAsciiOutput)
    //     pseudoDecompression(inFileName, outFileName);
    // else
    //     trueDecompression(inFileName, outFileName);
    return 0;
}
