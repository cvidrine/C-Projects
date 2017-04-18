// This class plays the game of Life! This is an extension that includes graphics (with a random color for the cells), random world generation, and a method to restart the game without quitting.
//Within the random world generation, the user has the option to choose a probability for the likelyhood of each empty cell creating life.
//
 
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
#include "random.h"
using namespace std;
void createGrid(Grid<char>& grid, LifeGUI& graphics);
void createRandomGrid(Grid<char>& grid, LifeGUI& graphics);
void introMessage();
void drawGrid(Grid<char>& grid, LifeGUI& graphics);
void updateGrid(Grid<char>& grid, int numFrames, LifeGUI& graphics);
void nextGeneration(Grid<char>& grid, LifeGUI& graphics);
void wrapAround(Grid<char>& grid, int i, int j, int& neighbors);
int checkNeighbors(Grid<char>& grid, int row, int col);
int main() {
    // TODO: Finish the program!
    introMessage();
    Grid <char> gameBoard;
    LifeGUI graphics;
    createGrid(gameBoard, graphics);
    drawGrid(gameBoard, graphics);
    string userInput;
    while(true){
        userInput = getLine("a)nimate, t)ick, r)estart, q)uit? ");
        toLowerCase(userInput);
        if(userInput!="a" && userInput!="t" && userInput!="q" && userInput !="r"){
            cout << "Input a valid choice." << endl;
            continue;
        }
        if(userInput=="a"){
            int numFrames = getInteger("How many frames? ");
            updateGrid(gameBoard, numFrames, graphics);
 
        } else if (userInput=="t"){
            updateGrid(gameBoard, 1, graphics);
 
        } else if(userInput=="q"){
            break;
        } else if (userInput=="r"){
            clearConsole();
            introMessage();
            createGrid(gameBoard, graphics);
            drawGrid(gameBoard, graphics);
            continue;
        }
    }
    cout << "Have a nice Life!" << endl;
    return 0;
}
//displays the intro directions.
void introMessage(){
    cout << "Welcome to the CS 106B Game of life" << endl;
    cout << "A simulation of the lifecycle of a bacteria colony." << endl;
    cout <<"Cells (X) live and die by the following rules: " << endl;
    cout <<"- A cell with 1 or fewer neighbors dies." << endl;
    cout <<"- Locations with 2 neighbors remain stable." << endl;
    cout <<"- Locations with 3 neighbors will create life." << endl;
    cout <<"- A cell with 4 or more neighbors dies." << endl;
}
 
//Creates the grid based on whatever file the user inputted.
void createGrid(Grid<char>& grid, LifeGUI& graphics){
    ifstream filestream;
    string randomTest = getLine("type \"random\" to get a random world, or anything else to move on to file input instead: ");
    if(randomTest=="random"){
        createRandomGrid(grid, graphics);
    } else{
        promptUserForFile(filestream, "Input file: ");
        string rows, columns;
        getline(filestream, rows);
        getline(filestream, columns);
        int r = stringToInteger(rows);
        int c = stringToInteger(columns);
        grid.resize(r, c);
        graphics.resize(r, c);
        int i =0;
        int j =0;
        string line;
        while(getline(filestream, line)){
            if(line.find(" ") != -1) break;
            for(char c: line){
                grid[i][j] = c;
                j++;
            }
            j=0;
            i++;
        }
        filestream.close();
    }
}
 
void createRandomGrid(Grid<char>& grid, LifeGUI& graphics){
    int rows = randomInteger(1, 75);
    int columns = randomInteger(1, 75);
    bool alive;
    grid.resize(rows, columns);
    graphics.resize(rows, columns);
    double lifeChance = (double)getInteger("How likely should each cell be to contain life initially? ");
    while(lifeChance>=100 || lifeChance<=0){
        cout << "Please enter a valid probability." << endl;
        lifeChance=(double)getInteger("How likely should each cell be to contain life initially? ");
    }
    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            alive = randomChance(lifeChance/100);
            if(alive){
                grid[i][j]='X';
            } else{
                grid[i][j]='-';
            }
            graphics.drawCell(i, j, alive);
        }
    }
}
 
//Draws the inputted grid in the console.
void drawGrid(Grid<char>& grid, LifeGUI& graphics){
    bool alive;
    for(int i=0; i<grid.numRows(); i++){
        for(int j=0; j<grid.numCols(); j++){
            cout << grid[i][j];
            if(grid[i][j]=='X'){
                alive=true;
            }else{
                alive=false;
            }
            graphics.drawCell(i, j, alive);
        }
        cout << endl;
    }
}
//Updates the grid however many generations the user asked for, with 50 milliseconds of pause in between.
void updateGrid(Grid<char>& grid, int numFrames, LifeGUI& graphics){
    for(int i=0; i<numFrames; i++){
        clearConsole();
        nextGeneration(grid, graphics);
        drawGrid(grid, graphics);
        pause(50);
    }
}
//Creates the next Generation of Life, does not print anything just adjusts the grid.
void nextGeneration(Grid<char>& grid, LifeGUI& graphics){
    Grid<char> newGrid(grid.numRows(), grid.numCols());
    bool alive;
    for(int i=0; i<grid.numRows(); i++){
        for(int j=0; j<grid.numCols(); j++){
            int numNeighbors=0;
            numNeighbors = checkNeighbors(grid, i, j);
            if(numNeighbors<2 || numNeighbors>3){
                newGrid[i][j]='-';
                alive=false;
            } else if(numNeighbors==2){
                newGrid[i][j]=grid[i][j];
                if(grid[i][j]=='X'){
                    alive=true;
                } else{
                    alive=false;
                }
            } else{
                newGrid[i][j]='X';
                alive=true;
            }
            graphics.drawCell(i, j, alive);
        }
    }
    grid=newGrid;
}
 
//Returns how many neighbors a given square has.
int checkNeighbors(Grid<char>& grid, int row, int col){
    int neighbors =0;
    for(int i=row-1; i<row+2; i++){
        for(int j=col-1; j<col+2; j++){
            if(grid.inBounds(i,j) && grid[i][j]=='X'){
                neighbors++;
            } else if(i<0 || i==grid.numRows() || j<0 || j==grid.numCols()){
                wrapAround(grid, i, j, neighbors);
            }
        }
    }
    if(grid[row][col]=='X') neighbors--;
    return neighbors;
}
//Checks the wrap Around and adds a neighbor if its a wrap around.
void wrapAround(Grid<char>& grid, int i, int j, int& neighbors){
    if(i<0){
        if(j>0 && j<grid.numCols()){
            if(grid[grid.numRows()-1][j]=='X') neighbors++;
        }
    } else if(i==grid.numRows()){
        if(j>0 && j<grid.numCols()){
            if(grid[0][j]=='X') neighbors++;
        }
    } else if(j<0){
        if(i>0 && i<grid.numRows()){
            if(grid[i][grid.numCols()-1]=='X') neighbors++;
        }
    } else{
        if(i>0 && i<grid.numRows()){
            if(grid[i][0]=='X') neighbors++;
        }
    }
}
