#include "LinkedPriorityQueue.h"
 
LinkedPriorityQueue::LinkedPriorityQueue() {
    frontNode=NULL;
 
}
 
LinkedPriorityQueue::~LinkedPriorityQueue() {
    //Deletes the list. Functions the same as clear list.
    if(frontNode!=NULL) {
        ListNode* previousNode;
        while(true){
            previousNode=frontNode;
            frontNode=frontNode->next;
            delete previousNode;
            if(frontNode==NULL) break;
        }
    }
 
 
}
//Changes an objects priority. Throws error if element is not in array or already is more urgent.
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* tempNode = frontNode;
    ListNode* previousNode = NULL;
    while(tempNode!=NULL){
        if(value==tempNode->value){
            if(newPriority > tempNode->priority){ //Checks if element already has higher priority.
                throw "Priority  is already more urgent";
                return;
            }else{//executed if there is a valid changePriority call.
                if(previousNode!=NULL) previousNode->next=tempNode->next;
                delete tempNode;
                this->enqueue(value, newPriority);
                return;
            }
        }
        //moves on to searching the next element.
        previousNode=tempNode;
        tempNode=tempNode->next;
    }
    throw "Value is not in queue";
}
//Deletes every element from the list one by one.
void LinkedPriorityQueue::clear() {
    if(frontNode==NULL) return;
    ListNode* previousNode;
    while(true){
        previousNode=frontNode;
        frontNode=frontNode->next;
        delete previousNode;
        if(frontNode==NULL) break;
    }
 
}
//Dequeues an element and adjust the pointers accordingly.
string LinkedPriorityQueue::dequeue() {
    if(frontNode==NULL){
        throw "The queue is empty";
    }
    ListNode* tempNode = frontNode;
    frontNode = frontNode->next;
    string value = tempNode->value;
    delete tempNode;
    return value;
}
 
void LinkedPriorityQueue::enqueue(string value, int priority) {
    //If the queue is empty, adds the first node.
    if(frontNode==NULL){
        frontNode = new ListNode(value, priority);
        return;
    }
    ListNode* tempNode = frontNode; //Node that will follow our new node. (new one will point to this).
    string tempValue = frontNode->value; //initialized to front Node, will change to help track location.
    int tempPriority = frontNode->priority; //same as above
    ListNode* previousNode=NULL;//node preceding the new node.
 
    //Searches the linked list to find the appropriate location.
    while(tempNode!=NULL && tempPriority <= priority){ //Conditions: while not at the end of the list or above the priority.
        if(tempPriority==priority && value <= tempValue) break; //Used for tiebreaker with priorities.
        previousNode=tempNode;//Sets up previous node before moving onto next.
        tempNode=tempNode->next;
        if(tempNode==NULL){ //If we reach the end of the list here we create a new node and return.
            ListNode* newNode = new ListNode(value, priority);
            previousNode->next=newNode; //Makes previous end point to new end.
            return;
        }
        //Moves the value and priority if we haven't reached the correct point.
        tempValue=tempNode->value;
        tempPriority=tempNode->priority;
    }
 
 
    //Creates a new Node that points at the correct location after the while loop.
    ListNode* newNode = new ListNode(value, priority, tempNode);
    if(previousNode==NULL){ //Checks if new Node is supposed to be at the front.
        frontNode=newNode;
    } else{
        previousNode->next=newNode;
    }
 
}
//Returns if the list is empty.
bool LinkedPriorityQueue::isEmpty() const {
    return frontNode==NULL;
}
//Peeks the top element. Throws an exception if empty.
string LinkedPriorityQueue::peek() const {
    if(frontNode!=NULL){
        return frontNode->value;
    } else{
        throw "Queue is Empty";
    }
}
//Peeks the top priority. Throws exception if empty.
int LinkedPriorityQueue::peekPriority() const {
    if(frontNode!=NULL){
        return frontNode->priority;
    } else{
        throw "Queue is Empty";
    }
}
//Gets the size of the list.
int LinkedPriorityQueue::size() const {
    ListNode* tempNode = frontNode;
    int counter =0;
    while(tempNode!=NULL){
        counter++;
        tempNode=tempNode->next;
    }
    return counter;
}
 
//Prints the list.
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode* temp= queue.frontNode;
    out << "{";
    while(temp!=NULL){
        out << "\"" << temp->value << "\"" << ":" << temp->priority;
        temp=temp->next;
        if(temp!=NULL) out << ",";
    }
    out <<"}";
    return out;
}
