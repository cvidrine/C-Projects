
//This code is for ngrams, a program that takes in a file and can create writing in a similar style to the file.
 
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "tokenscanner.h"
#include "stack.h"
#include "hashmap.h"
#include "vector.h"
#include "random.h"
 
using namespace std;
void introMessage();
void createMap(HashMap<Vector<string>, Vector<string>>& inputMap, ifstream& filestream, int n);
void generateWords(HashMap<Vector<string>, Vector<string>>& inputMap, int n);
void vectorPrint(Vector<string>& window);
//Sets up program.
int main() {
    HashMap<Vector<string>, Vector<string>> inputMap;
    introMessage();
    ifstream filestream;
    promptUserForFile(filestream, "Input file name? ");
    int n=-1;
    while(n<=0) n = getInteger("Value of N?");
    createMap(inputMap, filestream, n);
    filestream.close();
    generateWords(inputMap, n);
    //cout << inputMap.toString() <<endl;
 
    cout << "Exiting." << endl;
    return 0;
}
//Has the introductory message for the user.
void introMessage(){
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
}
//Takes in a HashMap, the user input file, and the int n that the user passed in to the program. Uses these to build the Hashmap
//that maps previous word chains to words that follow.
void createMap(HashMap<Vector<string>, Vector<string>>& inputMap, ifstream& filestream, int n){
    string word;
    Vector<string> temp;
    while(filestream >> word){
        temp.add(word);
    }
    for(int i=0; i<temp.size(); i++){
        string str = "";
        int count=0;
        Vector<string> valueSet;
        Vector<string> key;
        for(int j=i; j<i+n-1; j++){
            if(j<temp.size()){
                key+=temp[j];
            } else{
                key+=temp[count++]; //used for the wraparound.
 
            }
 
        }
        //If statement to check if it needs to wrap around or not.
        if(i+n-1<temp.size()){
            if(inputMap.containsKey(key)){
                valueSet= inputMap.get(key);
                valueSet+=temp[i+n-1];
                inputMap.put(key, valueSet);
            } else{
                valueSet.add(temp[i+n-1]);
                inputMap.put(key, valueSet);
            }
        }else{
            if(inputMap.containsKey(key)){
                valueSet= inputMap.get(key);
                valueSet+=temp[count];
                inputMap.put(key, valueSet);
            } else{
                valueSet.add(temp[count]);
                inputMap.put(key, valueSet);
            }
        }
    }
 
}
//Asks the user for how many words to generate, then randomly generates that many words by using a sliding window to get the key, then choosing a random
//value from that given key. Using the random value and the key, the window slides down one word and gets a new random value from the new key.
void generateWords(HashMap<Vector<string>, Vector<string>>& inputMap, int n){
    while(true){
        int wordsMax = getInteger("# of random words to generate (0 to quit)");
        if(wordsMax==0) break;
        if(wordsMax<n){
            cout << "Please enter a number of words greater than " << n << endl;
            continue;
        }
        cout <<"...";
        Vector<Vector<string>> keys = inputMap.keys();
        Vector<string> window = keys[randomInteger(0, keys.size()-1)]; //finds a random key for the initial value of the window.
        vectorPrint(window);
        int numWords = window.size(); //counter for how many words have been used.
        while(numWords<wordsMax){
            int randomValue = randomInteger(0, inputMap.get(window).size()-1);
            Vector<string> currentVector = inputMap.get(window);
            string newWord=currentVector.get(randomValue);
            window+=newWord; //Gets a random value from the vector that currently occupies the window.
            window.remove(0); // removes the first value of the window.
            numWords++;
            cout << window[window.size()-1]+ " ";
        }
        cout << "..." << endl;
        cout << endl;
    }
}
 
//Used to print an entire vector without brackets.
void vectorPrint(Vector<string>& window){
    for(string str: window){
        cout << str + " ";
    }
}
