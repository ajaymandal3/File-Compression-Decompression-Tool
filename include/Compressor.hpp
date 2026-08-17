#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <string>
#include <unordered_map>
#include "HuffmanTree.hpp"

using namespace std;

// Compressor ties everything together: reading files, calculating
// frequencies, driving the HuffmanTree, packing/unpacking bits, and
// writing files to disk. This is the class main.cpp actually talks to.
class Compressor
{
private:
    HuffmanTree huffmanTree;

    // Counts how many times each byte value occurs in "data".
    unordered_map<unsigned char, int> calculateFrequencies(const string &data);

    // Reads an entire file (in binary mode) into a string.
    // Sets success = false if the file could not be opened.
    string readEntireFile(const string &filePath, bool &success);

public:
    Compressor();
    ~Compressor();

    // Full pipeline: read -> frequencies -> tree -> codes -> encode -> write.
    // Returns true on success, false if any error occurred.
    bool compressFile(const string &inputPath, const string &outputPath);

    // Full pipeline: read tree -> read data -> decode -> write original file.
    // Returns true on success, false if any error occurred.
    bool decompressFile(const string &inputPath, const string &outputPath);
};

#endif
