#include "Compressor.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>

using namespace std;

Compressor::Compressor()
{
}

Compressor::~Compressor()
{
}

string Compressor::readEntireFile(const string &filePath, bool &success)
{
    ifstream inFile(filePath, ios::binary);
    if (!inFile.is_open())
    {
        success = false;
        return "";
    }

    ostringstream buffer;
    buffer << inFile.rdbuf();
    success = true;
    return buffer.str();
}

unordered_map<unsigned char, int> Compressor::calculateFrequencies(const string &data)
{
    unordered_map<unsigned char, int> frequencies;
    for (unsigned char ch : data)
    {
        frequencies[ch]++;
    }
    return frequencies;
}

bool Compressor::compressFile(const string &inputPath, const string &outputPath)
{
    cout << "Reading file...\n";
    bool opened = false;
    string data = readEntireFile(inputPath, opened);

    if (!opened)
    {
        cout << "Error: Could not open input file \"" << inputPath
             << "\". Please check that the file path is correct.\n";
        return false;
    }

    if (data.empty())
    {
        cout << "Error: Input file is empty. There is nothing to compress.\n";
        return false;
    }

    cout << "Calculating character frequencies...\n";
    unordered_map<unsigned char, int> frequencies = calculateFrequencies(data);

    cout << "Building Huffman Tree...\n";
    huffmanTree.buildTree(frequencies);

    cout << "Generating Huffman Codes...\n";
    unordered_map<unsigned char, string> codes = huffmanTree.getCodes();

    cout << "Compressing file...\n";
    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open())
    {
        cout << "Error: Could not create output file \"" << outputPath << "\".\n";
        return false;
    }

    // 1) Save the tree shape so decompression can rebuild the exact same codes.
    huffmanTree.serializeTree(outFile);

    // 2) Save the original character count, so decompression knows exactly
    //    how many characters to decode (and can ignore left-over padding bits).
    uint64_t originalSize = data.size();
    outFile.write(reinterpret_cast<const char *>(&originalSize), sizeof(originalSize));

    // 3) Build the full bit-stream by looking up each character's code,
    //    then pack it 8 bits at a time into real bytes.
    unsigned char currentByte = 0;
    int bitCount = 0;

    for (unsigned char ch : data)
    {
        const string &code = codes[ch];
        for (char bit : code)
        {
            currentByte = static_cast<unsigned char>((currentByte << 1) | (bit - '0'));
            bitCount++;
            if (bitCount == 8)
            {
                outFile.put(static_cast<char>(currentByte));
                currentByte = 0;
                bitCount = 0;
            }
        }
    }
    // Flush any remaining bits, padded on the right with zeros.
    if (bitCount > 0)
    {
        currentByte = static_cast<unsigned char>(currentByte << (8 - bitCount));
        outFile.put(static_cast<char>(currentByte));
    }
    outFile.close();

    // Report statistics.
    ifstream check(outputPath, ios::binary | ios::ate);
    streampos compressedSize = check.tellg();
    check.close();

    double originalBytes = static_cast<double>(data.size());
    double compressedBytes = static_cast<double>(compressedSize);
    double ratio = (1.0 - (compressedBytes / originalBytes)) * 100.0;

    cout << "\n--------------------------------------------------\n";
    cout << "Compression completed successfully!\n\n";
    cout << "Original Size    : " << data.size() << " bytes\n";
    cout << "Compressed Size  : " << static_cast<long long>(compressedSize) << " bytes\n";
    cout << "Compression Ratio: " << ratio << "%\n";
    cout << "--------------------------------------------------\n";

    return true;
}

bool Compressor::decompressFile(const string &inputPath, const string &outputPath)
{
    cout << "Reading compressed file...\n";
    ifstream inFile(inputPath, ios::binary);
    if (!inFile.is_open())
    {
        cout << "Error: Could not open compressed file \"" << inputPath << "\".\n";
        return false;
    }

    cout << "Rebuilding Huffman Tree...\n";
    huffmanTree.deserializeTree(inFile);
    HuffmanNode *root = huffmanTree.getRoot();

    if (root == nullptr || !inFile)
    {
        cout << "Error: The compressed file is invalid or corrupted.\n";
        return false;
    }

    uint64_t originalSize = 0;
    inFile.read(reinterpret_cast<char *>(&originalSize), sizeof(originalSize));
    if (!inFile)
    {
        cout << "Error: The compressed file is invalid or corrupted.\n";
        return false;
    }

    cout << "Decoding data...\n";

    // Read every remaining byte of the file as encoded bit-stream data.
    ostringstream restBuffer;
    restBuffer << inFile.rdbuf();
    string encodedBytes = restBuffer.str();

    string decodedData;
    decodedData.reserve(originalSize);

    HuffmanNode *current = root;
    uint64_t decodedCount = 0;

    for (unsigned char byte : encodedBytes)
    {
        for (int bitIndex = 7; bitIndex >= 0 && decodedCount < originalSize; bitIndex--)
        {
            int bit = (byte >> bitIndex) & 1;
            current = (bit == 0) ? current->getLeft() : current->getRight();

            if (current == nullptr)
            {
                cout << "Error: Decompression failed (corrupted data).\n";
                return false;
            }

            if (current->isLeaf())
            {
                decodedData += static_cast<char>(current->getCharacter());
                decodedCount++;
                current = root;
            }
        }
        if (decodedCount >= originalSize)
        {
            break;
        }
    }

    if (decodedCount != originalSize)
    {
        cout << "Error: Decompression failed (unexpected end of data).\n";
        return false;
    }

    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open())
    {
        cout << "Error: Could not create output file \"" << outputPath << "\".\n";
        return false;
    }
    outFile.write(decodedData.data(), static_cast<streamsize>(decodedData.size()));
    outFile.close();

    cout << "\n--------------------------------------------------\n";
    cout << "Decompression completed successfully!\n";
    cout << "--------------------------------------------------\n";

    return true;
}
