#include "Boggle.h"
#include "grid.h"
#include "shuffle.h"
#include <cctype>
#include "BoggleGUI.h"
 
 
// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};
 
const int NUMROWS = 4;
const int NUMCOLS = 4;
const int MIN_WORD_LENGTH = 4;
//Takes in a given dictionary and boardText and creates a Boggle board. If the board text is empty, then random text is generated using the cubes.
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    gameBoard.resize(NUMROWS, NUMCOLS);
    if(boardText!=""){
        boardText = toUpperCase(boardText);
        BoggleGUI::labelAllCubes(boardText);
        int letterIndex=0;
        for(int i=0; i<NUMROWS; i++){
            for(int j=0; j<NUMCOLS; j++){
                gameBoard[i][j] = boardText[letterIndex++];
            }
        }
    } else{
        int cubeIndex=0;
        for(int i=0; i<NUMROWS; i++){
            for(int j=0; j<NUMCOLS; j++){
                string temp = CUBES[cubeIndex];
                int charIndex = randomInteger(0, temp.length()-1);
                char faceUpLetter= temp[charIndex];
                gameBoard[i][j]=faceUpLetter;
                cubeIndex++;
            }
        }
        shuffle(gameBoard);
        for(int i=0; i<NUMROWS; i++){
            for(int j=0; j<NUMCOLS; j++){
                boardText+=gameBoard[i][j];
            }
        }
        BoggleGUI::labelAllCubes(boardText);
    }
    this->dictionary=dictionary;
    guessedWords.clear();
    computerWords.clear();
}
//return the letter at the given row and column.
char Boggle::getLetter(int row, int col) {
    if(gameBoard.inBounds(row, col)){
        return gameBoard[row][col];
    } else{
        throw 0;
    }
}
//Checks if the given user inputted word is long enough, if its already been guessed, and finally if its a valid english word.
bool Boggle::checkWord(string word) {
    toLowerCase(word);
    return word.length()>=MIN_WORD_LENGTH && !guessedWords.contains(word) && dictionary.contains(word);
}
//Function that takes in a word that the user wants to search for and has a double for loop to go through every spot on the board.
//For each spot on the board, calls the recursive helper to explore starting from that spot.
bool Boggle::humanWordSearch(string word) {
    BoggleGUI::setAnimationDelay(100);
    BoggleGUI::clearHighlighting();
    word = toUpperCase(word);
    string boardResults="";
    int charIndex=0;
    Grid<bool> visited;
    visited.resize(NUMROWS, NUMCOLS);
    for(int i=0; i<NUMROWS; i++){
        for(int j=0; j<NUMCOLS; j++){
            visited.fill(false);
            if(wordSearchHelper(word, boardResults, charIndex, i, j, visited)) {
                humanScore+= word.length()-3;
                guessedWords.add(word);
                return true;
            }
        }
    }
    return false;
}
//Recursively searches the grid starting from the given position. Takes in the goalWord, the current string of exploration, the index of the goal
//word that we're searching for, the startRow and StartCol of the given square, and a grid of booleans to see what's already been visited.
bool Boggle::wordSearchHelper(string& word, string boardResults, int charIndex, int startRow, int startCol, Grid<bool>& visited){
    BoggleGUI::setHighlighted(startRow, startCol, true);
    if(word==boardResults && !guessedWords.contains(boardResults)){
        //correct ending to recursive calls, word has been found.
        if(!visited[startRow][startCol]){
            BoggleGUI::setHighlighted(startRow, startCol, false);
        }
        return true;
        //Dead end search.
    } else if(!dictionary.containsPrefix(boardResults) || boardResults.length()>= word.length() || word[charIndex]!= gameBoard[startRow][startCol] || visited[startRow][startCol]){
        BoggleGUI::setHighlighted(startRow, startCol, false);
        return false;
    }else {
        //BoardResults is progressing correctly but hasn't hit the correct word yet. Search in all 8 directions to see if it is neighbored by the next correct letter.
        boardResults+=gameBoard[startRow][startCol];
        visited[startRow][startCol] = true;
        BoggleGUI::setHighlighted(startRow, startCol, true);
        charIndex++;
        for(int i=startRow-1; i<=startRow+1; i++){
            for(int j=startCol-1; j<=startCol+1; j++){
                if(gameBoard.inBounds(i, j)){
                    if(wordSearchHelper(word, boardResults, charIndex, i, j, visited)){
                        return true;
                    } else{
                        BoggleGUI::setHighlighted(i, j, false);
                    }
                    if(visited[i][j]){
                        BoggleGUI::setHighlighted(i, j, true);
                    }
                }
            }
        }
        boardResults.erase(boardResults.length()-1, 1);
        charIndex--;
    }
    BoggleGUI::setHighlighted(startRow, startCol, false);
    visited[startRow][startCol] = false;
    return false;
}
//gets the Human Score
int Boggle::getScoreHuman() {
    return humanScore;
}
//gets the Number of Human words.
int Boggle::getNumWordsHuman(){
    return guessedWords.size();
}
//Loops through every square on the grid and calls the helper recursive method for each one to explore.
Set<string> Boggle::computerWordSearch() {
    string boardResults="";
    Grid<bool> visited;
    visited.resize(NUMROWS, NUMCOLS);
    for(int i=0; i<NUMROWS; i++){
        for(int j=0; j<NUMCOLS; j++){
            visited.fill(false);
            computerScore+=computerSearcher(boardResults, i, j, visited);
        }
    }
    return computerWords;      
}
//Fully explores all possibilities from a given starting point, adding points and words to the computer set as it goes. Returns the final score
//from starting at any given letter.
int Boggle:: computerSearcher(string boardResults, int startRow, int startCol, Grid<bool>& visited){
    int score=0;
    if(dictionary.contains(boardResults) && !guessedWords.contains(boardResults) && !computerWords.contains(boardResults) && boardResults.length()>3){
        computerWords.add(boardResults);
        score+= boardResults.length()-3;
    } else if(visited[startRow][startCol] || !dictionary.containsPrefix(boardResults)){
        return 0;
    } else{
        boardResults+=gameBoard[startRow][startCol];
        visited[startRow][startCol] = true;
        for(int i=startRow-1; i<=startRow+1; i++){
            for(int j=startCol-1; j<=startCol+1; j++){
                if(gameBoard.inBounds(i, j)){
                    int result=computerSearcher(boardResults, i, j, visited);
                    score+=result;
                }
            }
        }
        visited[startRow][startCol]=false;
        boardResults.erase(boardResults.length()-1, 1);
    }
 
    return score;
}
//Gets the score of the Computer.
int Boggle::getScoreComputer() {
    return computerScore;
}
//Gets the Number of Words that the Computer has.
int Boggle:: getNumWordsComputer(){
    return computerWords.size();
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    Grid<char> gameBoard = boggle.getBoard();
    for(int i=0; i<NUMROWS; i++){
        for(int j=0; j<NUMCOLS; j++){
            out << gameBoard[i][j];
        }
        out << endl;
    }
    return out;
}

//returns the Game Board
Grid<char> Boggle:: getBoard(){
    return gameBoard;
}
//returns the guessed Words by the human
Lexicon Boggle::getGuessedWords(){
    return guessedWords;
}
