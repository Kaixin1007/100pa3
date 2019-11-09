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
    unsigned char nextChar;
    int nextByte;
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

    tree.build(freqs);

    out.open(outFileName, ios::binary);
    // write header
    for (int i = 0; i < 256; i++) {
        out << freqs[i] << endl;
    }
    for (int i = 0; i < temp.size(); i++) {
        tree.encode(temp[i], bos);
    }
    // bug?
    bos.flush();
    out.close();
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

    trueCompression(inFileName, outFileName);
    //     if (isAsciiOutput)
    //         pseudoCompression(inFileName, outFileName);
    //     else
    //         trueCompression(inFileName, outFileName);
}