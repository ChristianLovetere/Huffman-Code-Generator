//Christian Lovetere
//U46489387
//A series of files that utilize trees, stacks, maps, iterators, and a heap queue to compress text using huffman coding. a map is filled with each character that occurs in the
//string as well as how often they occur. This information is used to compress the string into binary. The binary code can be used with the serialized tree to decompress the 
//information into the original text it came as.


#include "HuffmanTree.hpp"

std::string HuffmanTree::compress(const std::string inputStr){
    std::map<char, size_t> mapOfChars;
    std::map<char, size_t>::iterator i;

    size_t amount[256] = {0}; //array of size_t with 256 entries, one for each ascii character. all are equal to 0 to begin.

    for (int i = 0; inputStr[i] != '\0'; i++) { //increment the amount of any given character found by...
        amount[(int)inputStr[i]]++; //... increasing the number at the location in the array that corresponds to the ascii value of any given character
    }
    for(int i = 0; i < 256; i++) { //add a pair to the map for each character that was found atleast once
        if(amount[i] != 0){
            mapOfChars.insert(std::pair<char,size_t>((char)i, amount[i]));
        }
    }

    HeapQueue<HuffmanNode*, HuffmanNode::Compare> huffQueue; //init a heapqueue that stores info from the map. the key is the node and the value is the frequency.
    for (i = mapOfChars.begin(); i != mapOfChars.end(); ++i) {
        HuffmanNode* temp = new HuffmanNode(i->first, i->second); //create temp node
        n++;                                                      //increment size variable
        huffQueue.insert(temp);                                   //add the node to the heapqueue
    }

    while (huffQueue.size() != 1){  //while the queue has 2 or more entries, create a new node that holds the summed frequency of the minimum two entries, remove these entries from the queue, make the summed node the parent of those two, and put this node in the queue instead.
        HuffmanNode* left = huffQueue.min();
        huffQueue.removeMin();
        HuffmanNode* right = huffQueue.min();
        huffQueue.removeMin();
        size_t totalFreq = (left->getFrequency() + right->getFrequency());
        HuffmanNode* parent = new HuffmanNode('\0', totalFreq, NULL, left, right);
        left->parent = parent;
        right->parent = parent;
        huffQueue.insert(parent);
    }

    root = huffQueue.min(); //now that the queue only has one entry, make this entry the root.

    std::map<char, std::string> mapOfBinaryCodes; //init a map with char keys and string values to associate each character with its binary code for its spot in the tree
    std::string BinCode = "";
    std::string outputStr = "";

    for (i = mapOfChars.begin(); i != mapOfChars.end(); ++i) {
        findCode(huffQueue.min(), mapOfBinaryCodes, BinCode); //go to findcode for explanation
    }

    for (auto i : inputStr) { //add all the codes to the output string
        outputStr += mapOfBinaryCodes[i];
    }
    return outputStr; //return output string as one long code of all the leaf nodes' individual codes
}

std::string HuffmanTree::serializeTree() const{
    if (root == NULL) //if there's no tree, just return
        return "";
        
    std::string outputString = ""; 
    serialize(root, outputString); //go to serialize for explanation
    return outputString;
}

std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree){ //use the serialized tree as well as the compressed code to decompress the text
    std::string outputString = "";
    std::stack<HuffmanNode*> nodeStack;

    for(std::string::const_iterator i = serializedTree.begin(); i != serializedTree.end(); i++){ //analyze all the tree items
        if(*i == 'L'){ //if its a leaf
            i++; //go to next item
            HuffmanNode *temp = new HuffmanNode(*i,0); // make a huff node with the current value
            nodeStack.push(temp); //put it on the stack
        }
        else { //if its a branch
            HuffmanNode* right = nodeStack.top(); //the right child of this node is the item on the top of the node stack
            nodeStack.pop();
            HuffmanNode* left = nodeStack.top(); //the left child of this node is the item on the top of the node stack
            nodeStack.pop();
            HuffmanNode* branch = new HuffmanNode('\0', 0, NULL, left, right); //create a branch
            nodeStack.push(branch); //push it onto the stack
        }
    }

    HuffmanNode* root = nodeStack.top(); //the root is the last thing in the stack
    nodeStack.pop(); //clear the stack

    HuffmanNode* current = root; //current acts as a cursor, starts at the root
    for (auto character : inputCode) {
        if (character == '0') //if character is a 0, go left, if not, go right.
            current = current->left;
        else current = current->right;

        if (current->isLeaf()) { //when finding leaves, add their character to the output string
            outputString += current->getCharacter();
            current = root; //then return to the root and begin again
        }
    }
    return outputString; //return the finished output string
}

void HuffmanTree::findCode(const HuffmanNode* inputNode, std::map<char, std::string> & codeMap, std::string code) { //create the binary code for a character
        if (inputNode == NULL)
            return;

        if(inputNode->isLeaf()) // call recursively until the node in question is a leaf
            codeMap[inputNode->getCharacter()] = code; //if it is a leaf, put the binary code for this specific node into the map for its corresponding character

        findCode(inputNode->left, codeMap, code + "0");     //going left in the tree adds a 0 to the binary code
        findCode(inputNode->right, codeMap, code + "1");    //going right in the tree adds a 1 to the binary code
}

void HuffmanTree::serialize(const HuffmanNode* inputNode, std::string &outputString) const { //serialize recursively, in post-order
    if (inputNode == NULL) 
        return;

    serialize(inputNode->left, outputString);
    serialize(inputNode->right, outputString);

    //if the node is a leaf
    if (inputNode->isLeaf())
        outputString = outputString + "L" + inputNode->getCharacter(); //add L and the character to mark leaves

    //if the node is a branch
    if(inputNode->isBranch())
        outputString += "B"; //add B to mark branches
}