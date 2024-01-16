# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <fstream>
# include <ctime>
# include <cctype>
# include "hash.h"
# include "heap.h"
# include "graph.h"

// Graph Class
// By: Jeffrey Wong
/* This file is used to implement a version of a directed graph
particularly equipped to run Dijkstra's algorithm. Includes a list
of vertices, and edges are stored in vertices' adjacency lists.*/

Graph::Graph():vertexTable(5){};

int Graph::insertVertex(std::string& name){
    vertex *v = new vertex(name);
    if(vertexTable.insert(name, v)){
        return -1;
    }
    vertexList.push_back(v);
    return 0;
}

int Graph::insertEdge(std::string& start, std::string& end, int weight){
    if(!vertexTable.contains(start)){
        if(insertVertex(start)){
            return -1;
        }
    }
    if(!vertexTable.contains(end)){
        if(insertVertex(end)){
            return -1;
        }
    }
    bool status;
    vertex *startvertex = (vertex *)vertexTable.getPointer(start, &status);
    if(!status){
        return -1;
    }
    vertex *endvertex = (vertex *)vertexTable.getPointer(end, &status);
    if(!status){
        return -1;
    }
    edge *e = new edge(weight,endvertex);
    startvertex->adjacencylist.push_back(e);
    return 0;
}

int Graph::dijkstrasAlgorithm(){
    heap vertexHeap(vertexList.size() * 2); // Heap for processing the vertices

    // Get initial vertex
    std::string initialName;
    do{
        std::cout << "Enter name of starting vertex: ";
        std::cin >> initialName;
    }while(!vertexTable.contains(initialName));
    time_t t1 = clock();

    // Initialize all vertices as unknown and with infinite distance, and add them to the heap
    for(std::list<vertex*>::iterator it = vertexList.begin(); it != vertexList.end(); it++){
        (*it)->isKnown = false;
        (*it)->dv = INT32_MAX/2; // Want half of maximum to prevent overflow, half of max is ~1.07b > 1 billion (max path length)
        (*it)->pv = nullptr;
        vertexHeap.insert((*it)->name, (*it)->dv, *it);
    }
    // Set initial vertex info
    vertex *initialVertex = (vertex*)vertexTable.getPointer(initialName);
    initialVertex -> dv = 0;
    vertexHeap.setKey(initialName, 0);

    vertex* currentVertex;
    int currentdv;
    while(!vertexHeap.deleteMin(nullptr, &currentdv, &currentVertex)){
        currentVertex->isKnown = true;
        for(std::list<edge*>::iterator jt = currentVertex->adjacencylist.begin(); jt != currentVertex->adjacencylist.end(); jt++){
            if(currentdv + (*jt)->weight < (*jt)->destination->dv){
                (*jt)->destination->dv = currentdv + (*jt)->weight;
                (*jt)->destination->pv = currentVertex;
                vertexHeap.setKey((*jt)->destination->name, (*jt)->destination->dv);
            }
        }
    }

    time_t t2 = clock();
    double time_diff = ((double) (t2-t1))/CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << time_diff << "\n";

    // Exporting to file
    std::string outputFile;
    std::cout << "Enter name of output file: ";
    std::cin >> outputFile;
    std::ofstream output(outputFile);
    if(!output){
        std::cerr << "Invalid filename\n";
        return -1;
    }
    for(std::list<vertex*>::iterator it = vertexList.begin(); it != vertexList.end(); it++){
        output << (*it)->name << ": ";
        if((*it)->dv >= INT32_MAX/2){
            output << "NO PATH\n";
        }
        else{
            output << (*it)->dv << " [";
            std::list<vertex*> path;
            currentVertex = (*it);
            do{
                path.push_front(currentVertex);
                currentVertex = currentVertex->pv;
            }while(currentVertex != nullptr);
            for(std::list<vertex*>::iterator jt = path.begin(); jt != path.end(); jt++){
                output << (*jt)->name;
                // Check to see if we are at the last node in path- if yes we cap off the line, otherwise we keep going
                if((++jt) == path.end()){
                    output << "]\n";
                }
                else{
                    output << ", ";
                }
                jt--;
            }
        }
    }
    return 0;
}