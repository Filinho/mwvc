#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cassert>

using namespace std;

class Graph {
public:
  int nVertex;
  int nEdges;
  vector<int> weight;
  vector<set<int>> adjList;

  void parse(const string& filePath) {
    ifstream streamFile;
    unsigned auxWeight;
    unsigned auxV1;
    unsigned auxV2;
    streamFile.open(filePath);

    streamFile >> nEdges;
    streamFile >> nVertex;
    
    weight.resize(nVertex);
    adjList.resize(nVertex);

    for (unsigned i = 0; i < nVertex; ++i) {
      streamFile >> auxWeight;
      weight[i] = auxWeight;
      adjList[i] = set<int>();
    }

    for (unsigned i = 0; i < nEdges; ++i) {
      
      streamFile >> auxV1;
      streamFile >> auxV2;
      assert(adjList[auxV1 - 1].find(auxV2 - 1) == adjList[auxV1 - 1].end());
      assert(adjList[auxV2 - 1].find(auxV1 - 1) == adjList[auxV2 - 1].end());
      adjList[auxV1-1].insert(auxV2-1);
      adjList[auxV2-1].insert(auxV1-1);
    }
  }

  void printGraph() {
    cout << nVertex << endl;
    for (int i = 0; i < nVertex; i++) {
      cout << weight[i]  << " " ;
      for (int neighbor : adjList[i]) {
        cout << neighbor << " ";
      }
      cout << endl;
    }
  }
};

Graph instance;