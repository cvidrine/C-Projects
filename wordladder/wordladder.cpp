//This code is for word ladder, a program that takes in two words of the same length and finds the shortest valid "word ladder"
//between them. Word ladders are made of changing the word one letter at a time into another valid english word.
 
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "lexicon.h"
#include "filelib.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
using namespace std;
void introMessage();
void readDictionary(Set<string>& dictionary);
void executeLadder(string wordOne, string wordTwo, Set<string>& dictionary);

//Sets up the program and gets user input. Checks for invalid user inputs before running.
int main() {
    introMessage();
    Set<string> dictionary;
    readDictionary(dictionary);
    
    while(true){
    string wordOne= getLine("Word #1 (or Enter to quit): ");
    if(wordOne=="") break;
    string wordTwo = getLine("Word #2 (or Enter to quit): ");
    if(wordTwo=="") break;
    wordOne = toLowerCase(wordOne);
    wordTwo = toLowerCase(wordTwo);
    cout<<"A ladder from " <<wordTwo << " back to " <<wordOne <<endl;
    
    
    if(!dictionary.contains(wordOne) || !dictionary.contains(wordTwo)){
        cout << "The two words must be found in the dictionary." << endl;
        continue;
    } else if(wordOne.length()!=wordTwo.length()){
        cout <<"The two words must be the same length" << endl;
        continue;
    } else if(wordOne==wordTwo){
        cout << "The two words must be different" <<endl;
        continue;
    }
    
    executeLadder(wordOne, wordTwo, dictionary);
    cout <<endl;
    }
 
    cout << "Have a nice day." << endl;
    return 0;
}
 
//Prints intro text.
void introMessage(){
    cout << "Welcome to CS106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}
//Takes in the empty dictionary and fills it up with text from a file.
void readDictionary(Set<string>& dictionary){
    ifstream filestream;
    string line;
    promptUserForFile(filestream, "Dictionary file name? ");
    while(getline(filestream, line)){
        dictionary.add(line);
    }
    filestream.close();
}
 
//Takes in the two words and the dictionary, executes ladder by creating new stacks for every possible word ladder and putting them in a queue.
//Stops executing when one of the stacks ends up at the first word.
void executeLadder(string wordOne, string wordTwo, Set<string>& dictionary){
    Queue<Stack<string>> wordQueue;
    Stack<string> currentStack;
    currentStack.push(wordTwo);
    wordQueue.enqueue(currentStack);
    Set<string> guessedList;
    while(!wordQueue.isEmpty()){
        currentStack=wordQueue.dequeue();
        string topWord = currentStack.peek();
        for(int i=0; i<topWord.length(); i++){
            string newGuess=topWord;
            for(int j=0; j<26; j++){
                newGuess[i] = 'a'+j;
                if(dictionary.contains(newGuess) && !guessedList.contains(newGuess)){
                    if(newGuess==wordOne){
                        currentStack.push(newGuess);
                        cout << currentStack.toString() << endl;
                        i=topWord.length();
                        j=26;
                        wordQueue.clear();
                    } else{
                        Stack<string> newStack = currentStack;
                        newStack.push(newGuess);
                        wordQueue.enqueue(newStack);
                        guessedList.add(newGuess);
                    }
                }
            }
        }
    }
}
