#include "HuffmanNode.hpp"

using namespace std;

// Leaf node constructor: character is real, no children.
HuffmanNode::HuffmanNode(unsigned char ch, int freq)
    : character(ch), frequency(freq), left(nullptr), right(nullptr)
{
}

// Internal node constructor: built by combining two smaller nodes.
HuffmanNode::HuffmanNode(int freq, HuffmanNode *leftChild, HuffmanNode *rightChild)
    : character(0), frequency(freq), left(leftChild), right(rightChild)
{
}

// Recursively deleting left/right means deleting the root of the tree
// automatically destroys every node beneath it. This is a simple and
// classic example of recursion through a destructor.
HuffmanNode::~HuffmanNode()
{
    delete left;
    delete right;
}

unsigned char HuffmanNode::getCharacter() const
{
    return character;
}

int HuffmanNode::getFrequency() const
{
    return frequency;
}

HuffmanNode *HuffmanNode::getLeft() const
{
    return left;
}

HuffmanNode *HuffmanNode::getRight() const
{
    return right;
}

bool HuffmanNode::isLeaf() const
{
    return left == nullptr && right == nullptr;
}
