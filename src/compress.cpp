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
using namespace std;
/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(string inFileName, string outFileName) {
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
void trueCompression(string inFileName, string outFileName) {}

/* TODO: Main program that runs the compress */
int main(int argc, char* argv[]) {
    const int NUM_ARG = 3;
    FileUtils fu;

    if (argc != NUM_ARG) {
        cout << "Invalid number of arguments:" << argc << ".\n" << endl;
        return -1;
    }
    // check if argument is vaild
    if (!fu.isValidFile(argv[1])) return -1;
    // check if the file is empty
    // if (fu.isEmptyFile(argv[1])) return -1;

    pseudoCompression(argv[1], argv[2]);
}