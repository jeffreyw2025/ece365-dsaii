#ifndef _GRAPH_H
#define _GRAPH_H

# include <vector>
# include <list>
# include <string>
# include "hash.h"
# include "heap.h"

class Graph{
    public:
        // Constructor
        Graph();

        // Attempts to insert a vertex with a given name into the graph. On failure to push list or add to hashtable, returns -1.
        int insertVertex(std::string& name);

        /* Attempts to insert an edge with initial vertex start and terminal vertex end with a certain weight. 
        On failure to add vertex, returns -1. */
        int insertEdge(std::string& start, std::string& end, int weight);

        // Runs through the process of executing Dijkstra's Algorithm
        int dijkstrasAlgorithm();

    private:
        // Private classes for vertices and edges

        // Forward declaration of edge needed for adjacency lists
        class edge;

        // In addition to a name and list of adjacenct vertices, vertices support several variables useful in running Dijkstra's algorithm
        class vertex{
            public:
                std::string name;
                std::list<edge*> adjacencylist;
                bool isKnown{false}; // Checks if known by Dijkstra's algorithm
                int dv{0}; // Distance to vertex in Dijkstra's algorithm
                vertex *pv; // Previous vertex in Dijkstra's algorithm
                vertex(std::string n){
                    name = n;
                }
        };

        // Edges have a integer weight and destination vertex, the source vertex is the vertex containing the edge in its adjacency list
        class edge{
            public:
                int weight;
                vertex *destination;
                edge(int w, vertex *d){
                    weight = w;
                    destination = d;
                }
        };

        // Graph private members
        std::list<vertex*> vertexList;
        hashTable vertexTable;
};

#endif