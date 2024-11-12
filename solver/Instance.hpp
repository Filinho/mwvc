#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class Graph {

  int nVertex;
  vector<set<int>> adjList;

public:
  
  void parse(const string& filePath) {
    ifstream streamFile;
    streamFile.open(filePath);
    streamFile >> nVertex;
    adjList.resize(nVertex);
    int nEdges;
    int auxVertex;
    for (unsigned i = 0; i < nVertex; ++i) {
      adjList[i] = set<int>();
      streamFile >> nEdges;
      for (unsigned j = 0; j < nEdges; ++j) {
        streamFile >> auxVertex;
        adjList[i].insert(auxVertex);
      }
    }
  }

  void printGraph() {
    cout << nVertex << endl;
    for (int i = 0; i < nVertex; i++) {
      cout << adjList[i].size() << " ";
      for (int neighbor : adjList[i]) {
        cout << neighbor << " ";
      }
      cout << endl;
    }
  }
};