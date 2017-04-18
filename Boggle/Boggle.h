#ifndef _boggle_h
#define _boggle_h
 
#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "bogglegui.h"
using namespace std;
 
class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();
    Lexicon getGuessedWords();
    int getNumWordsHuman();
    int getNumWordsComputer();
    friend ostream& operator<<(ostream& out, Boggle& boggle);
 
private:
    Grid<char> gameBoard;
    Lexicon dictionary;
    Lexicon guessedWords;
    int humanScore=0;
    int computerScore=0;
    bool wordSearchHelper(string& word, string boardResults, int charIndex, int startRow, int startCol, Grid<bool>& visited);
    int computerSearcher(string boardResults, int startRow, int startCol, Grid<bool>& visited);
    Grid<char> getBoard();
    Set<string> computerWords;
 
};
 
#endif 
