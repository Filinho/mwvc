#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class Graph {
public:
  int nVertex;
  int nEdges;
  vector<int> weight;
  vector<set<int>> adjList;
  
  void parse(const string& filePath) {
    nEdges = 0;
    ifstream streamFile;
    streamFile.open(filePath);
    streamFile >> nVertex;
    weight.resize(nVertex);
    adjList.resize(nVertex);
    int auxNEdges;
    int auxVertex;
    for (unsigned i = 0; i < nVertex; ++i) {
      adjList[i] = set<int>();
      
      streamFile >> auxNEdges;
      streamFile >>weight[i];

      nEdges += auxNEdges;
      for (unsigned j = 0; j < auxNEdges; ++j) {
        streamFile >> auxVertex;
        adjList[i].insert(auxVertex);
      }
    }
    nEdges /= 2;
  }

  void printGraph() {
    cout << nVertex << endl;
    for (int i = 0; i < nVertex; i++) {
      cout << adjList[i].size() << " " << weight[i]  << " " ;
      for (int neighbor : adjList[i]) {
        cout << neighbor << " ";
      }
      cout << endl;
    }
  }
};

Graph instance;