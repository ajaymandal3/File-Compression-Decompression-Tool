#include "HuffmanTree.hpp"
#include <queue>
#include <vector>

using namespace std;

HuffmanTree::HuffmanTree() : root(nullptr)
{
}

// Deleting root cascades through HuffmanNode's destructor and frees the
// whole tree automatically.
HuffmanTree::~HuffmanTree()
{
    delete root;
}

void HuffmanTree::buildTree(const unordered_map<unsigned char, int> &frequencies)
{
    // A small local comparator so that priority_queue behaves as a MIN-heap
    // (by default priority_queue is a max-heap, so we flip the comparison).
    struct CompareNodes
    {
        bool operator()(HuffmanNode *a, HuffmanNode *b)
        {
            return a->getFrequency() > b->getFrequency();
        }
    };

    priority_queue<HuffmanNode *, vector<HuffmanNode *>, CompareNodes> minHeap;

    // Step 1: create one leaf node per distinct character and push it in.
    for (const auto &pair : frequencies)
    {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    // Edge case: the file contains only ONE distinct character.
    // A normal Huffman tree needs at least two leaves, so we manually wrap
    // the single leaf inside one internal node. Its code will simply be "0".
    if (minHeap.size() == 1)
    {
        HuffmanNode *only = minHeap.top();
        minHeap.pop();
        root = new HuffmanNode(only->getFrequency(), only, nullptr);
        generateCodes(root, "");
        return;
    }

    // Step 2: repeatedly take the two smallest-frequency nodes and merge
    // them into a new internal node, until only one (the root) is left.
    while (minHeap.size() > 1)
    {
        HuffmanNode *left = minHeap.top();
        minHeap.pop();
        HuffmanNode *right = minHeap.top();
        minHeap.pop();

        int combinedFrequency = left->getFrequency() + right->getFrequency();
        HuffmanNode *parent = new HuffmanNode(combinedFrequency, left, right);
        minHeap.push(parent);
    }

    root = minHeap.top();
    minHeap.pop();

    // Step 3: walk the finished tree and record the binary code for every character.
    generateCodes(root, "");
}

void HuffmanTree::generateCodes(HuffmanNode *node, const string &code)
{
    if (node == nullptr)
    {
        return;
    }
    if (node->isLeaf())
    {
        // A tree with only one unique character would produce an empty code
        // at the root; we guard against that by defaulting to "0".
        huffmanCodes[node->getCharacter()] = code.empty() ? "0" : code;
        return;
    }
    generateCodes(node->getLeft(), code + "0");
    generateCodes(node->getRight(), code + "1");
}

unordered_map<unsigned char, string> HuffmanTree::getCodes() const
{
    return huffmanCodes;
}

HuffmanNode *HuffmanTree::getRoot() const
{
    return root;
}

void HuffmanTree::writeTreeToFile(HuffmanNode *node, ofstream &outFile)
{
    if (node == nullptr)
    {
        outFile.put('X');
        return;
    }
    if (node->isLeaf())
    {
        outFile.put('1');
        outFile.put(static_cast<char>(node->getCharacter()));
    }
    else
    {
        outFile.put('0');
        writeTreeToFile(node->getLeft(), outFile);
        writeTreeToFile(node->getRight(), outFile);
    }
}

HuffmanNode *HuffmanTree::readTreeFromFile(ifstream &inFile)
{
    char marker;
    inFile.get(marker);

    if (!inFile)
    {
        // Ran out of data unexpectedly -> corrupted/invalid file.
        return nullptr;
    }
    if (marker == 'X')
    {
        return nullptr;
    }
    if (marker == '1')
    {
        char ch;
        inFile.get(ch);
        return new HuffmanNode(static_cast<unsigned char>(ch), 0);
    }
    // marker == '0' -> internal node, recurse into both children.
    HuffmanNode *left = readTreeFromFile(inFile);
    HuffmanNode *right = readTreeFromFile(inFile);
    return new HuffmanNode(0, left, right);
}

void HuffmanTree::serializeTree(ofstream &outFile)
{
    writeTreeToFile(root, outFile);
}

void HuffmanTree::deserializeTree(ifstream &inFile)
{
    root = readTreeFromFile(inFile);
}
