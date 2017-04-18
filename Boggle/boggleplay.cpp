#include "lexicon.h"
#include <string>
#include "strlib.h"
#include "Boggle.h"
#include "simpio.h"
#include "console.h"
#include "bogglegui.h"
 
string boardText();
void humanTurn(Boggle *boggle);
void computerTurn(Boggle *boggle);
void decideWinner(Boggle *boggle);
bool stringIsWrong(string userBoard);
void computerWordGraphic(Set<string> computerWords);
 
const int NUMROWS =4;
const int NUMCOLS =4;
//Initializes variables and runs the human turn, the computer turn, and then decides the winner.
void playOneGame(Lexicon& dictionary) {
    string userBoard = boardText();
    BoggleGUI::initialize(NUMROWS, NUMCOLS);
    Boggle* boggle = new Boggle(dictionary, userBoard);
    //cout << ostream& boggle;
    humanTurn(boggle);
    computerTurn(boggle);
    decideWinner(boggle);
    delete boggle;
    //ostream printBoard;
}
//Runs code for the human turn. If the guessed word is valid, search grid for guessed word. If not, error message. After every guess, prints the word Set for the
//player and the score.
void humanTurn(Boggle* boggle){
    clearConsole();
    cout << "It's your turn!" << endl;
    while(true){
       // boggle->printBoard();
        cout << *boggle;
        cout << "Your words (" <<boggle->getNumWordsHuman() <<"): " << boggle->getGuessedWords().toString() <<endl;
        cout << "Your score: " << boggle->getScoreHuman() << endl;
        BoggleGUI::setScore(boggle->getScoreHuman(), BoggleGUI::HUMAN);
        string guess = getLine("Type a word (or Enter to stop): ");
        if(guess =="") break;
        clearConsole();
        if(boggle->checkWord(guess)){
            if(boggle->humanWordSearch(guess)){
                cout << "You found a new word! "<< "\"" << toUpperCase(guess) << "\"" << endl;
                BoggleGUI::recordWord(toUpperCase(guess), BoggleGUI::HUMAN);
            }else{
                cout << "That word can't be formed on this board." << endl;
            }
        } else{
            cout <<"You must enter an unfound 4+ letter word from the dictionary." << endl;
        }
    }
}
//Runs the code for the computers turn. Finds all the remaining words on the board and prints the computers word set and score.
void computerTurn(Boggle *boggle){
    cout << "It's my turn !" << endl;
    Set<string> computerWords = boggle->computerWordSearch();
    cout << "My words (" << boggle->getNumWordsComputer() <<"): " << computerWords.toString() << endl;
    cout << "My Score: " << boggle->getScoreComputer() << endl;
    BoggleGUI::setScore(boggle->getScoreComputer(), BoggleGUI::COMPUTER);
    computerWordGraphic(computerWords);
}
//prints the list of computer words to the screen.
void computerWordGraphic(Set<string> computerWords){
    for(string str: computerWords){
        BoggleGUI::recordWord(toUpperCase(str), BoggleGUI::COMPUTER);
    }
}
//Prints a message depending on who won the game.
void decideWinner(Boggle *boggle){
    int computerScore = boggle->getScoreComputer();
    int humanScore = boggle->getScoreHuman();
    if(computerScore > humanScore){
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    } else if(computerScore==humanScore){
        cout << "We tied. How satisfying.." << endl;
    } else{
        cout << "Congratulations! You beat me at my own game." <<endl;
    }
}
//Gets the 16 letter string for a user inputted board, checks if its a valid string.
string boardText(){
    string userBoard="";
    if(!getYesOrNo("Do you want to generate a random board?")){
        while(userBoard.length()!=16 || stringIsWrong(userBoard)){
            userBoard = getLine("Enter the 16 characters for your board");
            userBoard = trim(userBoard);
        }
    }
    return userBoard;
}
//Checks if the user Board string is made up solely of letters.
bool stringIsWrong(string userBoard){
    if (userBoard=="") return true;
    for(char c: userBoard){
        if(!isalpha(c)) return true;
    }
    return false;
}
