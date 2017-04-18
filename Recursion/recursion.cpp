/* 
 * List of small, seperate recursive methods.
 *
 *
 */
 
#include <math.h>
#include "recursion.h"
#include "map.h"
#include "vector.h"
#include "set.h"
#include "gwindow.h"
#include "gobjects.h"
#include "tokenscanner.h"
using namespace std;
 
//Returns the Greatest common denominator of two given ints.
int gcd(int a, int b) {
    if(b==0){
        //Base case for when solution is found.
        cout << "gcd(" << a << ", " << b <<") = " <<a<<endl;
        return a;
    } else{
        //Prints function trace and makes recursive call.
        cout << "gcd(" << a << ", " <<b <<") = gcd(" <<b <<", " <<a%b <<")" << endl;
        return gcd(b, a%b);
    }
    return 0;
}
 
//Draws a serpinskii triangle of the given order with the given side size and starting upper left corner dimensions.
void serpinskii(GWindow &w, int leftX, int leftY, int size, int order) {
    if(order==0){
      return;
    } else if (order==1){
        //Other base case, when the triangle is order one then it is drawn with given dimensions.
        w.drawLine(leftX, leftY, leftX+size, leftY);
        w.drawLine(leftX, leftY, leftX+size/2, leftY+size*sqrt(3)/2);
        w.drawLine(leftX+size, leftY, leftX+size/2, leftY+size*sqrt(3)/2);
    } else{
        //If the triangle is not order one yet, then the dimensions are adjusted and serpinskii is called again.
        serpinskii(w, leftX, leftY, size/2, order-1);
        serpinskii(w, leftX+size/2, leftY, size/2, order-1);
        serpinskii(w, leftX+size/4, leftY+size*sqrt(3)/4, size/2, order-1);
    }
}
 
//Fills commonly colored areas of the canvas with the inputted Color starting at position x, y.
int floodFill(GBufferedImage& image, int x, int y, int newColor) {
    int count=0;
    if (!image.inBounds(x,y)) return 0;     //returns 0 if the point clicked is ever out of bounds.
    int oldColor=image.getRGB(x,y);     //Gets the original color of the first square.
    if(oldColor==newColor) return 0;     //If the oldColor matches the color that it is being changed to, we automatically return n0.
    image.setRGB(x, y, newColor);
    count++;
    //Goes North if in Bounds and is still the same color.
    if(image.inBounds(x, y+1) && image.getRGB(x, y+1)== oldColor){
        count+= floodFill(image, x, y+1, newColor);
    }
    //Goes South " ".
    if(image.inBounds(x, y-1) && image.getRGB(x, y-1) == oldColor){
        count+= floodFill(image, x, y-1, newColor);
    }
    //Goes East " ".
    if(image.inBounds(x+1, y) && image.getRGB(x+1, y)== oldColor){
        count+= floodFill(image, x+1, y, newColor);
    }
    //Goes West " ".
    if(image.inBounds(x-1, y) && image.getRGB(x-1, y)== oldColor){
        count+= floodFill(image, x-1, y, newColor);
    }
    return count;
 
 
}
 
//Prints out a list of required prereqs for a given goal course.
void personalCurriculum(Map<string, Vector<string>> & prereqMap,string goal) {
    Vector<string> prereqs = prereqMap.get(goal);
    Vector<string> empty;
    empty.add("");
    if(prereqs.isEmpty()){
        //base case of a class having no prereqs.
    } else{
        //Calls function recursively for each prereq for a given course.
        for(string str: prereqs){
            personalCurriculum(prereqMap,str);
        }
    }
    //Checks if the class has already been printed to the console. Only prints it if it isn't already there.
    if(prereqMap.get(goal) != empty){
        cout << goal << endl;
        prereqMap.put(goal, empty);
    }
}
 
//Generates a random sentence from a given grammer set and inputted symbol from the set.
string generate(Map<string, Vector<string> > & grammar, string symbol) {
    string result;
    //Base case that checks if the symbol is a terminal
    if(!grammar.containsKey(symbol)){
        return symbol;
    } else{
        //Get the vector for the given symbol
       Vector<string> tempVect = grammar.get(symbol);
       //choose random element
       string symbolList = tempVect[randomInteger(0, tempVect.size()-1)];
       //get each token from random element
       TokenScanner scanner(symbolList);
       //recursive call for each element in the chosen section of the grammar.
       while(scanner.hasMoreTokens()){
           result += generate(grammar, scanner.nextToken());
       }
       return result;
    }
}
