#include "ArrayPriorityQueue.h"
 
static const int INIT_SIZE = 10;
ArrayPriorityQueue::ArrayPriorityQueue() {
    data = new PQEntry[INIT_SIZE];
    usedSize=0;
    allocatedSize=INIT_SIZE;
 
}
 
ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete[] data;
 
}
 
void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    for(int i=0; i< usedSize; i++){
        PQEntry entry = data[i];
        if(entry.value==value){
            if(entry.priority < newPriority){
                throw "Error: Entries priority is already lower";
            }
            entry.priority = newPriority;
            return;
        }
    }
    string exceptionMessage = "Element not found in Array";
    throw exceptionMessage;
 
}
 
void ArrayPriorityQueue::clear() {
    delete[] data;
    data = new PQEntry[INIT_SIZE];
    usedSize=0;
    allocatedSize=INIT_SIZE;
 
}
 
string ArrayPriorityQueue::dequeue() {
    string errorMessage = "Array is empty";
    if(usedSize==0) throw errorMessage;
    int lowestPriority;
    string correctString;
    int index;
    for(int i=0; i<usedSize; i++){
        PQEntry entry = data[i];
        int priority = entry.priority;
        string value = entry.value;
        if(i==0){
            lowestPriority=priority;
            correctString = value;
            index=i;
        }
        if(priority<lowestPriority){
            lowestPriority=priority;
            correctString=value;
            index=i;
        }
    }
    
    for(int i=index; i< usedSize-1; i++){
        data[i]= data[i+1];
    }
    usedSize--;
 
    return correctString;
}
 
void ArrayPriorityQueue::enqueue(string value, int priority) {
    PQEntry entry(value, priority);
    data[usedSize++]=entry;
    if(usedSize==allocatedSize) doubleAllocation();
 
}
 
void ArrayPriorityQueue::doubleAllocation(){
    allocatedSize *=2;
    PQEntry* newData = new PQEntry[allocatedSize];
    for(int i=0; i<usedSize; i++){
        newData[i]=data[i];
    }
    delete[] data;
    data=newData;
}
 
bool ArrayPriorityQueue::isEmpty() const {
    return usedSize==0;
}
 
string ArrayPriorityQueue::peek() const {
    string errorMessage = "Array is empty";
    if(usedSize==0) throw errorMessage;
    int lowestPriority;
    string correctString;
    for(int i=0; i<usedSize; i++){
        PQEntry entry = data[i];
        int priority = entry.priority;
        string value = entry.value;
        if(i==0){
            lowestPriority=priority;
            correctString = value;
        }
        if(priority<lowestPriority){
            lowestPriority=priority;
            correctString=value;
        }
    }
    return correctString;
}
 
int ArrayPriorityQueue::peekPriority() const {
    string errorMessage = "Array is empty";
    if(usedSize==0){
        throw errorMessage;
        return 0;
    }
    int lowestPriority;
    for(int i=0; i<usedSize; i++){
        PQEntry entry = data[i];
        int priority = entry.priority;
        string value = entry.value;
        if(i==0){
            lowestPriority=priority;
        }
        if(priority<lowestPriority){
            lowestPriority=priority;
        }
    }
    return lowestPriority;
}
 
int ArrayPriorityQueue::size() const {
    return usedSize;
}
 
ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    int size = queue.size();
    PQEntry* entry = queue.data;
    out << "{";
    for(int i=0; i< size-1; i++){
        out<<entry[i] << ",";
    }
    out<<entry[size-1];
    out <<"}";
    return out;
}
