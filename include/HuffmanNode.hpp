#ifndef HUFFMAN_NODE_HPP
#define HUFFMAN_NODE_HPP

using namespace std;

// HuffmanNode represents a single node of the Huffman binary tree.
//
// A node is either:
//   - a LEAF node   -> stores one character from the file (character + frequency)
//   - an INTERNAL node -> has two children and just stores the combined frequency
//
// Encapsulation: all data members are private. Other classes (HuffmanTree,
// Compressor) can only interact with a node through its public member
// functions (the getters and isLeaf()).
class HuffmanNode {
private:
    unsigned char character;   // the byte/character stored in a leaf node
    int frequency;              // how many times "character" appears (leaf),
                                 // or the combined frequency of both children (internal)
    HuffmanNode* left;          // left child (nullptr for leaf nodes)
    HuffmanNode* right;         // right child (nullptr for leaf nodes)

public:
    // Constructor for a LEAF node: stores an actual character + its frequency.
    HuffmanNode(unsigned char ch, int freq);

    // Constructor for an INTERNAL node: built from two existing nodes.
    // The character field is unused (set to 0) because internal nodes do
    // not represent an actual character in the file.
    HuffmanNode(int freq, HuffmanNode* leftChild, HuffmanNode* rightChild);

    // Destructor: recursively deletes children so that deleting the root
    // of a HuffmanTree automatically frees every node in the tree.
    ~HuffmanNode();

    // --- Getters (read-only access to private data) ---
    unsigned char getCharacter() const;
    int getFrequency() const;
    HuffmanNode* getLeft() const;
    HuffmanNode* getRight() const;

    // A node is a leaf if it has no children.
    bool isLeaf() const;
};

#endif
