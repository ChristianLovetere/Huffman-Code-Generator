#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <map>
#include <vector>
#include <stack>
#include <iostream>
#include <string>

#include "HeapQueue.hpp"
#include "HuffmanBase.hpp"

class HuffmanTree {
    private:
        size_t n;
        HuffmanNode* root;
    public:
    std::string compress(const std::string inputStr);
    std::string serializeTree() const;
    std::string decompress(const std::string inputCode, const std::string serializedTree);
    void findCode(const HuffmanNode* inputNode, std::map<char, std::string> & codeMap, std::string code);
    void serialize(const HuffmanNode* node, std::string &outString) const;
};


#endif