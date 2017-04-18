#include "HeapPriorityQueue.h"
 
static const int INIT_SIZE = 10;
 
HeapPriorityQueue::HeapPriorityQueue() {
    data= new PQEntry[INIT_SIZE];
    usedSize=0;
    allocatedSize=INIT_SIZE;
 
}
 
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] data;
 
}
 
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    //iterates through set.
    for(int i=1; i<=usedSize; i++){
        if(data[i].value==value){//checks for matching values.
            if(data[i].priority <= newPriority){
                throw "Failed to change Priority: Previous priority is already more urgent";
                return;
            }
            data[i].priority=newPriority; //Sets priority for value to new priority.
            percolateUp(i); //Bubbles data up to correct location.
            return;
        }
    }
    throw "Failed to change priority: Value not found in Queue";
    return;
 
}
 
void HeapPriorityQueue::clear() {
    delete[] data;
    usedSize=0;
    allocatedSize=INIT_SIZE;
    data=new PQEntry[INIT_SIZE];
 
}
 
string HeapPriorityQueue::dequeue() {
    if(this->isEmpty()){
        throw "Priority Queue is empty.";
        return "";
    }
    string result = data[1].value;
    data[1]=data[usedSize]; //moves bottom element to top.
    usedSize--;
    percolateDown(1); //Always is in index one to begin with.
    return result;
}
 
 
 
 
void HeapPriorityQueue:: percolateDown(int index){
    int newIndex = chooseIndex(index);
    if(newIndex==0) return; //case where both children are out of bounds (has no children).
    PQEntry priorityChild=data[newIndex];
    PQEntry parent = data[index];
    if(parent.priority < priorityChild.priority || (parent.priority == priorityChild.priority && parent.value < priorityChild.value)){
        return;
    } else{
        swapElements(newIndex, index); //swaps parent with child.
        percolateDown(newIndex);
    }
}
//Chooses which child has the lowest priority.
int HeapPriorityQueue:: chooseIndex(int index){
    int childOne = index*2;
    int childTwo = index*2+1;
    //checks if children are in bounds.
    if(childOne > usedSize) return 0; //0 is case where both are out of bounds.
    if(childTwo > usedSize) return childOne;
 
    //Creates pointers to Entries at specified indices.
    PQEntry childOneEntry = data[childOne];
    PQEntry childTwoEntry = data[childTwo];
 
    //Conditional statement to decide what int to return. Will return index of either Child one or child two.
    if(childOneEntry.priority == childTwoEntry.priority){ //Checks if priorities are equal.
        if(childOneEntry.value < childTwoEntry.value){ //compares values if priorities are equal.
            return childOne;
        } else{
            return childTwo;
        }
    } else if(childOneEntry.priority < childTwoEntry.priority){ //Compares priorities to determine what to return.
        return childOne;
    } else{
        return childTwo;
    }
}
 
void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry newEntry(value, priority);
    usedSize++;
    data[usedSize] = newEntry;
    percolateUp(usedSize);
    if(usedSize+1==allocatedSize) doubleAllocation();
}
 
void HeapPriorityQueue:: percolateUp(int index){
    if(index/2==0) return;
    PQEntry parent = data[index/2];
    PQEntry child = data[index];
    if(parent.priority < child.priority || (parent.priority == child.priority && parent.value < child.value)){
        return;
    } else{
        swapElements(index, index/2);
        percolateUp(index/2);
    }
}
 
void HeapPriorityQueue:: swapElements(int childIndex, int parentIndex){
    PQEntry tempParent = data[parentIndex];
    data[parentIndex]=data[childIndex];
    data[childIndex]=tempParent;
}
 
void HeapPriorityQueue::doubleAllocation(){
    allocatedSize *=2;
    PQEntry* newData = new PQEntry[allocatedSize];
    for(int i=0; i<usedSize; i++){
        newData[i]=data[i];
    }
    delete[] data;
    data=newData;
}
 
bool HeapPriorityQueue::isEmpty() const {
    return usedSize==0;
}
 
string HeapPriorityQueue::peek() const {
    if(this->isEmpty()){
        throw "Priority Queue is empty.";
    }
    return data[1].value;
}
 
int HeapPriorityQueue::peekPriority() const {
    if(this->isEmpty()){
        throw "Priority Queue is empty.";
    }
    return data[1].priority;
}
 
int HeapPriorityQueue::size() const {
    return usedSize;
}
 
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    int size = queue.size();
    PQEntry* entry = queue.data;
    out << "{";
    for(int i=1; i<=size; i++){
        out<<entry[i] << ",";
    }
    out<<entry[size];
    out <<"}";
    return out;
}
