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

/* TODO: Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(string inFileName, string outFileName) {
    ifstream myfile;
    HCTree tree;
    FileUtils fu;
    unsigned char nextChar;
    int nextByte, j;
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
void trueDecompression(string inFileName, string outFileName) {}

/* TODO: Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    const int NUM_ARG = 3;
    FileUtils fu;

    if (argc != NUM_ARG) {
        cout << "Invalid number of arguments:" << argc << ".\n" << endl;
        return -1;
    }
    // check if argument is vaild
    if (!fu.isValidFile(argv[1])) return -1;

    pseudoDecompression(argv[1], argv[2]);
    return 0;
}
