#include "encoding.h"
#include "pqueue.h"
#include <string>
#include "string.h"
#include "vector.h"
#include "bitstream.h"
#include "filelib.h"
 
 
void buildQueue(const Map<int, int>& freqTable, PriorityQueue<HuffmanNode *> &queue);
void buildTree(PriorityQueue<HuffmanNode*>& queue);
void buildEncodingMap(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string &binaryCode);
void encodeCharacter(string binaryCode, obitstream& output);
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output, HuffmanNode* root);
 
const int ENDLINEVALUE =10;
 
//Builds the frequency table.
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    string line;
    int numLines=0;
    while(getline(input,line)){
        numLines++;
        for(int i=0; i<line.length(); i++){
            int charValue = line[i]; //gets every individual character.
            if(freqTable.containsKey(charValue)){
                freqTable.put(charValue, freqTable.get(charValue)+1); //Adds one to the frequency value.
            } else{
                freqTable.put(charValue, 1); //adds new characteers to the table
            }
        }
        freqTable.put(ENDLINEVALUE, numLines); //puts the end line characters.
    }
    freqTable.put(PSEUDO_EOF, 1); //Adds the end line character.
    return freqTable;
}
 
 
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> queue;
    buildQueue(freqTable, queue);
    while(queue.size()> 1){
        buildTree(queue); //Whiles there is more elements in the queue, continue building tree.
    }
    HuffmanNode* frontNode = queue.dequeue(); //grabs the top Node to return it.
    return frontNode; //returns the node.
}
 
//Builds the priority Queue
void buildQueue(const Map<int, int>& freqTable, PriorityQueue<HuffmanNode*>& queue){
    Vector<int> keys= freqTable.keys(); //gets the keys.
    for(int key: keys){ //loops through the keys and enqueues each node.
        HuffmanNode* node = new HuffmanNode(key, freqTable.get(key));
        queue.enqueue(node, node->count);
    }
}
// uses the two bottom nodes to create a node above.
void buildTree(PriorityQueue<HuffmanNode*>& queue){
    int zeroPriority = queue.peekPriority();
    HuffmanNode* zero = queue.dequeue();
    int onePriority = queue.peekPriority();
    HuffmanNode* one = queue.dequeue();
    //Parent node with combined priority of two children.
    HuffmanNode* parent = new HuffmanNode(NOT_A_CHAR, zeroPriority+onePriority, zero, one);
    queue.enqueue(parent, zeroPriority+onePriority);
}
//Initializes the map and string binaryCode, then uses a helper to build the map.
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string binaryCode;
    buildEncodingMap(encodingTree, encodingMap, binaryCode);
    return encodingMap;
}
//creates the encoding map through recursive search.
void buildEncodingMap(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string& binaryCode){
    if(encodingTree->character != NOT_A_CHAR){
        //Adds the leafs to the map.
        encodingMap.put(encodingTree->character, binaryCode);
    } else{
        //Recurses in the "zero" direction
        HuffmanNode* zero = encodingTree->zero;
        binaryCode+="0";
        buildEncodingMap(zero, encodingMap, binaryCode);
        binaryCode.erase(binaryCode.length()-1); //unchoose
        //Recurses in the "one" direction.
        binaryCode+="1";
        HuffmanNode* one = encodingTree->one;
        buildEncodingMap(one, encodingMap, binaryCode);
        binaryCode.erase(binaryCode.length()-1); //unchoose
    }
}
 
//Writes the input file into the output stream after encoding each character.
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    string line;
    string binaryCode;
    while(getline(input, line)){
        for(int i=0; i<line.length(); i++){
            binaryCode = encodingMap.get(line[i]); //get the binary code for the character.
            encodeCharacter(binaryCode, output); // encodes the character.
        }
        binaryCode = encodingMap.get(ENDLINEVALUE); //Gets the binary code for the endline value.
        encodeCharacter(binaryCode, output);
    }
    binaryCode = encodingMap.get(PSEUDO_EOF);
    encodeCharacter(binaryCode, output);
}
//Writes the given binary Code string into the bit stream output.
void encodeCharacter(string binaryCode, obitstream& output){
    for(int j=0; j<binaryCode.length(); j++){
        string temp = charToString(binaryCode[j]); //takes the char and converts it to a string
        int bitWrite = stringToInteger(temp); //conversts the string to an int
        output.writeBit(bitWrite); //writes the int to output stream.
    }
}
//Calls the helper with an additional parameter to track the root of the tree.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    decodeData(input, encodingTree, output, encodingTree);
}
//recursively explores the tree to decode the data.
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output, HuffmanNode* root){
    if(encodingTree->character==PSEUDO_EOF){ //base case (reached end of file)
        return;
    }
    if(encodingTree->isLeaf()){ //other base case(reached leaf node)
        char c = encodingTree->character;
        output<<c; //adds character to output
        decodeData(input, root, output, root); //Returns to top of tree for next bit.
    } else{
        int next = input.readBit(); //gets the next bit
        //Chooses to go either left or right.
        if(next==0){
            decodeData(input, encodingTree->zero, output, root);
        } else{
            decodeData(input, encodingTree->one, output, root);
        }
    }
}
//Compresses the given file.
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* topNode = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(topNode);
    rewindStream(input); //rewinds the stream so it can be used in the encode method.
    output << freqTable; //puts the frequency table into   the compressed file
    freeTree(topNode);
    encodeData(input, encodingMap, output); //compresses the file.
}
//Decompresses the given file.
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input>>freqTable; // gets the table out of the input stream.
    HuffmanNode* treeRoot=buildEncodingTree(freqTable); //Gets the root
    decodeData(input, treeRoot, output); //decodes the rest of the file.
    freeTree(treeRoot);
}

 
//Frees the memory for the tree.
void freeTree(HuffmanNode* node) {
    if(node==NULL) return;
    HuffmanNode* zeroNode= node->zero;
    HuffmanNode* oneNode = node->one;
    //recursively delete both children.
    freeTree(zeroNode);
    freeTree(oneNode);
    delete node; //deletes node
}
