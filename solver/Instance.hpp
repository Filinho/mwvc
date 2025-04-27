#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cassert>
#include <unordered_map>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>

using namespace std;

class Graph {
public:
  unsigned nVertex;
  unsigned nEdges;
  vector<int> weight;
  vector<set<unsigned>> adjList;
  unordered_map<unsigned, pair<unsigned, unsigned>> edgeVertexs;
  unordered_map<pair<unsigned, unsigned>, unsigned, boost::hash<pair<unsigned, unsigned>>> vertexEdge;
  vector<bool> fixedSolution;

  unsigned get_edge(const unsigned v1, const unsigned v2) {
    if (v1 < v2) {
      return vertexEdge[pair<unsigned, unsigned>(v1, v2)];
    }

    return vertexEdge[pair<unsigned, unsigned>(v2, v1)];
  }

  unsigned remove_edge(const unsigned v1, const unsigned v2) {
    unsigned edge;

    if (v1 < v2) {
      edge = vertexEdge[pair<unsigned, unsigned>(v1, v2)];
      vertexEdge.erase(pair<unsigned, unsigned>(v1, v2));
    }
    else {
      edge = vertexEdge[pair<unsigned, unsigned>(v2, v1)];
      vertexEdge.erase(pair<unsigned, unsigned>(v2, v1));
    }

    return edge;
  }

  void add_edge(const unsigned v1, const unsigned v2, const unsigned idx) {

    if (v1 < v2) {
      vertexEdge[pair<unsigned, unsigned>(v1, v2)] = idx;
    }
    else {
      vertexEdge[pair<unsigned, unsigned>(v2, v1)] = idx;
    }
  }

  void parse(const string& filePath) {
    ifstream streamFile;
    unsigned auxWeight;
    unsigned auxV1;
    unsigned auxV2;
    streamFile.open(filePath);

    streamFile >> nEdges;
    streamFile >> nVertex;

    weight.resize(nVertex);
    adjList.resize(nVertex, set<unsigned>());

    for (unsigned i = 0; i < nVertex; ++i) {
      streamFile >> auxWeight;
      weight[i] = auxWeight;
    }

    for (unsigned i = 0; i < nEdges; ++i) {

      streamFile >> auxV1;
      streamFile >> auxV2;
      assert(adjList[auxV1 - 1].find(auxV2 - 1) == adjList[auxV1 - 1].end());
      assert(adjList[auxV2 - 1].find(auxV1 - 1) == adjList[auxV2 - 1].end());
      adjList[auxV1 - 1].insert(auxV2 - 1);
      adjList[auxV2 - 1].insert(auxV1 - 1);

      edgeVertexs[i] = pair<unsigned, unsigned>(auxV1 - 1, auxV2 - 1);
      add_edge(auxV1 - 1, auxV2 - 1, i);
    }

    assert(edgeVertexs.size() == nEdges);
  }

 /* void reduce_graph() {
    unordered_set<unsigned> vertexInGraph;
    vector<unsigned> weightN(nVertex, 0);
    fixedSolution.resize(nVertex, false);

    for (auto v : adjList) {
      vertexInGraph.insert(v.first);
      for (auto adj : v.second) {
        weightN[v.first] += weight[adj];
      }
    }

    while (true) {
      vector<unsigned> toBeRemoved, toBeRemovedCase1, toBeRemovedCase2;
      bool hasChanged = false;
      for (auto curV : vertexInGraph) {

        if (!adjList[curV].size()) {
          toBeRemoved.push_back(curV);
        }
        else if (adjList[curV].size() == 1 && weight[curV] < weightN[curV]) {
          toBeRemovedCase2.push_back(curV);
          hasChanged = true;
        }
        else if (adjList[curV].size() > 1 && weight[curV] > weightN[curV]) {
          toBeRemovedCase1.push_back(curV);
          hasChanged = true;
        }
      }

      for (unsigned r : toBeRemovedCase1) {
        for (auto adj : adjList[r]) {
          for (auto adj2 : adjList[adj]) {
            if (adj2 == r) continue;
            weightN[adj2] -= weight[adj];
            adjList[adj2].erase(adj);
            edgeVertexs.erase(remove_edge(adj, adj2));
          }
          fixedSolution[adj] = true;
          vertexInGraph.erase(adj);
          edgeVertexs.erase(remove_edge(r, adj));
          adjList.erase(adj);
        }
        vertexInGraph.erase(r);
        adjList.erase(r);
      }
      for (unsigned r : toBeRemovedCase2) {
        for (auto adj : adjList[r]) {
          weightN[adj] -= weight[r];
          edgeVertexs.erase(remove_edge(r, adj));
          adjList[adj].erase(r);
        }
        fixedSolution[r] = true;
        vertexInGraph.erase(r);
        adjList.erase(r);
      }
      for (auto r : toBeRemoved) {
        vertexInGraph.erase(r);
        adjList.erase(r);
      }

      if (!hasChanged) break;
    }

    nVertex = vertexInGraph.size();
    nEdges = edgeVertexs.size();
  }*/

  void printGraph() {
    cout << nVertex << endl;
    for (unsigned i = 0; i < nVertex; i++) {
      cout << weight[i] << " ";
      for (int neighbor : adjList[i]) {
        cout << neighbor << " ";
      }
      cout << endl;
    }
  }

  unsigned weigthSum() {
    unsigned sum = 0;
    for (unsigned i = 0; i < nVertex; ++i) {
      sum += weight[i];
    }
    return sum;
  }
};

Graph instance;