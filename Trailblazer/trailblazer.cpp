#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
// TODO: include any other headers you need; remove this comment
 
using namespace std;
Vector<Vertex*> buildPath(Vertex* endPoint);
bool sameCluster(Vertex* start, Vertex* end, Vector<Set<Vertex*>>& clusters);
bool correctPath(BasicGraph& graph, Vertex*& start, Vertex*& end, Vector<Vertex *> &path);
 
//Sets stuff up and passes to correctPath to make the path.
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    path.add(start);
    //recursive method for creating the path
    if(correctPath(graph, start, end, path))return path;
    Vector<Vertex*> empty;
    return empty;
}
 
//Recursive algorithm for the Depth First Search.
bool correctPath(BasicGraph& graph, Vertex*& start, Vertex*& end, Vector<Vertex *>& path){
    start->setColor(GREEN);
    if(start==end) return true; //Checks if we're at correct point
    if(start->visited) return false; //Checks if the point has already been visited
    start->visited=true; //sets point equal to true
    for(Edge* edge: start->arcs){ //Gets all neighboring edges to the current Node
        Vertex* neighbor = edge->finish; //Uses edges to get neighbors
        path.add(neighbor);   //choose
        if(correctPath(graph, neighbor, end, path)) return true; //explore
        path.remove(path.size()-1); //unchoose
    }
    start->setColor(GRAY);
    return false; //If no path is found with all neighbors
}
 
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    path+=start;
    Queue<Vector<Vertex*>> queue;
    queue.enqueue(path);
 
 
    while(!queue.isEmpty()){//While there are more paths to explore
        Vector<Vertex*> currPath = queue.dequeue();
        Vertex*  currState = currPath[currPath.size()-1]; //Gets the final Vertex in a given list
        currState->setColor(GREEN);
        if(currState->visited) continue; //continues if its already visited
        currState->visited = true;
        if(currState==end) return currPath; //Returns if the correct path is found
        for(Vertex* nextState: graph.getNeighbors(currState)){ //Gets all the neighbors
            path = currPath;
            path+= nextState; //Adds the neighbor to the path
            queue.enqueue(path); //enqueues longer path
            if(nextState->getColor()!=GREEN){
                nextState->setColor(YELLOW); //Changes uncolored neighbors to yellow
            }
        }
    }
    path.clear();
    return path; //empty path
}
//Uses a linked list to efficiently implement Dijkstras Algorithm
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    for(Vertex* v: graph.getVertexSet()) v->cost = INT_MAX; //Sets the cost of every vertex to max.
 
    PriorityQueue<Vertex*> pqueue;
    start->cost=0;
    pqueue.enqueue(start, 0); //enqueues the start node with cost 0
 
    while(!pqueue.isEmpty()){
        Vertex* currentPoint = pqueue.dequeue();
        currentPoint->visited=true;
        currentPoint->setColor(GREEN);
        if(currentPoint==end) break; //Breaks the while loop if the end is found
        for(Edge* edge: currentPoint->arcs){ //gets all the neighboring edges
            Vertex* neighbor = edge->finish; //Gets vertex associated with neighboring edges
            if(neighbor->visited) continue;
 
            double cost=currentPoint->cost+edge->cost; //Creates a cost from the cost of the previous node and the connecting edge.
 
            if(cost < neighbor->cost){ //If the new cost is lower than the old cost(initially set to INT_MAX)
                neighbor->cost=cost; //Sets cost to new, lower cost
                neighbor->previous=currentPoint; //Changes the previous one to the path that has lower cost
 
                if(neighbor->getColor()==YELLOW){ //If it has already been enqueued
                    pqueue.changePriority(neighbor, cost); //Change priority to lower cost one
                } else{
                    neighbor->setColor(YELLOW);
                    pqueue.enqueue(neighbor, cost); //Enqueues new nodes.
                }
            }
 
        }
    }
 
    return buildPath(end);
}
 
//Traces backwards through the linked list to build the path
Vector<Vertex*> buildPath(Vertex* endPoint){
    Vector<Vertex*> path;
    while(endPoint!=NULL){ //While there are more elements in the path
        path.insert(0, endPoint); //Adds path vertex to path
        endPoint=endPoint->previous; //Moves endpoint back to previous node
    }
    return path;
}
 
//Uses a linked list to run aStar. (See dijkstrasAlgorithm comments for implementation details)
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
 
    for(Vertex* v: graph.getVertexSet()) v->cost = INT_MAX;
 
    PriorityQueue<Vertex*> pqueue;
    start->cost=0;
    pqueue.enqueue(start, 0);
 
    while(!pqueue.isEmpty()){
        Vertex* currentPoint = pqueue.dequeue();
        currentPoint->visited=true;
        currentPoint->setColor(GREEN);
        if(currentPoint==end) return buildPath(currentPoint);
        for(Edge* edge: currentPoint->arcs){
            Vertex* neighbor = edge->finish;
            if(neighbor->visited) continue;
            Edge* connector = graph.getEdge(currentPoint, neighbor);
            double cost=currentPoint->cost+connector->cost;
 
            if(cost < neighbor->cost){
                neighbor->cost=cost;
                neighbor->previous=currentPoint;
 
                if(neighbor->getColor()==YELLOW){
                    pqueue.changePriority(neighbor, cost+heuristicFunction(neighbor, end)); //Adds the heuristic to the cost.
                } else{
                    neighbor->setColor(YELLOW);
                    pqueue.enqueue(neighbor, cost+heuristicFunction(neighbor, end)); //Adds the heuristic to the cost.
                }
            }
 
        }
 
    }
 
    Vector<Vertex*> emptyPath;
    return emptyPath;
 
}
//Implements kruskal's algorithm to build a Minimum Spanning Tree from a given graph
Set<Edge*> kruskal(BasicGraph& graph) {
 
    Set<Edge*> mst; //will hold our minimum spanning tree
    PriorityQueue<Edge*> edges;
    for(Edge* edge: graph.getEdgeSet()) edges.enqueue(edge, edge->cost); //set of edges
    Vector<Set<Vertex*>> clusters; //Vector that holds the clusters
    for(Vertex* v: graph.getVertexSet()){ //Puts all the vertices in their own cluster
        Set<Vertex*> cluster;
        cluster.add(v);
        clusters.add(cluster);
    }
 
    while(clusters.size() > 1){ //While they aren't all connected
        Edge* nextEdge = edges.dequeue(); //Gets the next edge
        //Runs sameCluster(see Comment below). If not in same Cluster, add edge to the MST.
        if(!sameCluster(nextEdge->start, nextEdge->end, clusters))mst.add(nextEdge);
    }
    return mst;
}
//Checks if two Vertices are in the same Cluster. If not, notify the parent and continue.
bool sameCluster(Vertex* start, Vertex* end, Vector<Set<Vertex*>>& clusters){
    int firstFoundIndex;
    bool foundOne=false;
    for(int i=0; i<clusters.size(); i++){ //loops through all sets
        if(clusters[i].contains(start) || clusters[i].contains(end)){ //If it finds the start or end.
            if(foundOne){ //checks if start or end has already been found previously
                clusters[firstFoundIndex]+=clusters[i]; //Adds the found start/end to the set for the previously found start/end
                clusters.remove(i); //removes the found start/end from the cluster set
                return false;
            }
            if(clusters[i].contains(start) && clusters[i].contains(end))return true; //Returns true if both are already in the same cluster.
            foundOne=true; //If one is found but the other is not in the same cluster, is set to true
            firstFoundIndex=i; //Index of the first found Vertex.
        }
    }
    return false;
}
