# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <fstream>
# include <ctime>
# include <cctype>
# include <graph.h>

using namespace std;

// Dijkstra's Algorithm Program
// By: Jeffrey Wong
/* This program uses a custom graph implementation to create a graph 
especially designed to execute Dijkstra's algorithm. It can run the algorithm
from any vertex on the graph, and generates a file denoting the shortest path
to a vertex or if a path does not exist, as well as the length of the path. */

void createGraph(Graph& g){
    string inputFile;
    cout << "Enter name of graph file: ";
    cin >> inputFile;
    ifstream input(inputFile);
    string line;
    while(getline(input, line)){
        // Each line consists of three "chunks", the first being the name of the starting vertex, 
        // the second the name of the ending vertex, and the third the weight of the edge
        string start, end, wstring; // Starting and ending vertices of edge, and string form of weight
        stringstream sstr(line);
        getline(sstr, start, ' ');
        getline(sstr, end, ' ');
        getline(sstr, wstring);
        int weight = stoi(wstring); // Weight of edge as an integer
        if(g.insertEdge(start, end, weight)){
            cerr << "Error while inserting edge\n" << endl;
        }
    }
    input.close();
}

int main(){
    Graph g;
    createGraph(g);
    g.dijkstrasAlgorithm();  
    return 0;
}