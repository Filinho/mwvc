#pragma once

#include "State.hpp"
#include "include/pcg_random.hpp"
#include <random>
#include <chrono>
#include <climits>

class Solver{
    public:
    
    Solver(){};

    int randomEdge(State & solution){

      chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();

      // generate random seed if available
      pcg_extras::seed_seq_from<std::random_device> seed_source;

      // create random engine
      pcg32 rng(seed_source);
        
        // pick random edge
        int nEdges = instance.nEdges;
        int count = 0;
        vector<set<int>> auxAdjList = instance.adjList;
        while(nEdges > 0){
            int v1 = rng(instance.nVertex);
            int v2;

            if(auxAdjList[v1].size()){   
                v2 = rng(auxAdjList[v1].size());
                if (v2 >= 0 && v2 < auxAdjList[v1].size()) {
                    auto it = auxAdjList[v1].begin();
                    advance(it, v2);
                    v2 = *it;
                }
                if(nEdges == 1){
                    solution.selected[v1] = 1;
                    break;
                }
                for(int j : auxAdjList[v2]){
                    auxAdjList[j].erase(v2);
                    nEdges--;
                }

                for(int j : auxAdjList[v1]){
                    auxAdjList[j].erase(v1);
                    nEdges--;
                }

                auxAdjList[v1].clear();
                auxAdjList[v2].clear();
             
                solution.selected[v1] = 1;
                solution.selected[v2] = 1;
            }
        }

        solution.timeSpent = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count();
        
        for (int i = 0; i < instance.nVertex; i++) {
          if (solution.selected[i] == 1) count += instance.weight[i];
        }
        return count;
    }

    int ratioEdge(State & solution){

      chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();

      vector<int> order;
      vector<set<int>> auxAdjList = instance.adjList;
      order.resize(instance.nVertex);
      order[0] = 0;

      for(int i = 1 ; i< instance.nVertex;i++){
        if (instance.adjList[i].size() == 0) continue;
        int j = i -1;
        while( j >=0 && (float) instance.weight[i]/instance.adjList[i].size() < (float)  instance.weight[order[j]]/instance.adjList[order[j]].size()){
          order[j+1] = order[j];
          j--;
        }
        order[j+1] = i;
      }

        int nEdges = instance.nEdges;
        int auxI=0;
        while (nEdges > 0){
            int v = order[auxI];
            solution.selected[v] = 1;

            for(int j : auxAdjList[v]){
                auxAdjList[j].erase(v);
                nEdges--;
            }
            
            auxAdjList[v].clear();
            auxI++;
        }

        solution.timeSpent = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count();

        int count = 0;
        for(int i = 0 ; i< instance.nVertex;i++){
            if(solution.selected[i] == 1) count += instance.weight[i];
        }

        return count;
    }

    void makeCombiUtil(vector<vector<int> >& ans, vector<int>& tmp, int n, int left, int k)
    {
      // Pushing this vector to a vector of vector
      if (k == 0) {
        ans.push_back(tmp);
        return;
      }

      // i iterates from left to n. First time
      // left will be 1
      for (int i = left; i <= n; ++i)
      {
        tmp.push_back(i);
        makeCombiUtil(ans, tmp, n, i + 1, k - 1);

        // Popping out last inserted element
        // from the vector
        tmp.pop_back();
      }
    }

    // Prints all combinations of size k of numbers
    // from 1 to n.
    vector<vector<int> > makeCombi(int n, int k){
      vector<vector<int> > ans;
      vector<int> tmp;
      makeCombiUtil(ans, tmp, n, 1, k);
      return ans;
    }


    unsigned opt(State& solution) {

      chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();

      unsigned cost = UINT32_MAX;

      for (unsigned k = 1; k < instance.nVertex; ++k) {
        vector<vector<int>> possibleSol = makeCombi(instance.nVertex, k);
        for (unsigned i = 0; i < possibleSol.size(); ++i) {
          State auxSol;
          unsigned auxCost = 0;
          for (unsigned j = 0; j < possibleSol[i].size(); ++j) {
            //cout << possibleSol[i][j]-1 << " ";
            auxCost += instance.weight[possibleSol[i][j]-1];
            auxSol.selected[possibleSol[i][j]-1] = true;
          }
          //cout << endl;

          if (auxCost < cost && auxSol.verify()) {
            solution = auxSol;
            cost = auxCost;
          }
        }
      }

      solution.timeSpent = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count();

      return cost;
    }

    int swapAdjacentNeighboorhood(State & solution, State & newSolution, int index){
      newSolution = solution;
 
      newSolution.selected[index] = 0;

      vector<int> adj;
      
      for(int a : instance.adjList[index]){
        adj.push_back(a);
  
      }
      for(int i  = 0; i < adj.size();i++){
        newSolution.selected[adj[i]] = 1;
      }
      
      return newSolution.calcCost();
    }

    int swap2VertexNeighboorhood(State & solution, State & newSolution, pcg32 rng){

      int nSelected = solution.nSelected();

      int v1 = rng(nSelected);
      int v2 = rng(nSelected);
      int j=0;
      for(int i = 0 ; i < instance.nVertex; i++){
        if(solution.selected[i] == 1) j++;
        if(j == v1) v1=i;
        if(j == v2) v2=i;
      }
      newSolution.selected[v1] = 0;
      newSolution.selected[v2] = 0;

      vector<bool> adjAux(instance.nVertex,false);

      for(int a : instance.adjList[v1]){
        adjAux[a] = true;
      }

      for(int a : instance.adjList[v2]){
        adjAux[a] = true;
      }
      vector<int> adj;
      int nEdges = instance.adjList[v1].size();
      if(v1 != v2) nEdges += instance.adjList[v2].size();
      if(instance.adjList[v1].find(v2) != instance.adjList[v1].end()) --nEdges;

      for(int i = 0 ; i < instance.nVertex; i++){
        if(solution.selected[i] == 1) adj.push_back(i);
      }

      while(nEdges > 0){
        int v = rng(adj.size());

        newSolution.selected[adj[v]] = 1;

        if(instance.adjList[adj[v]].find(v1) != instance.adjList[adj[v]].end())  --nEdges;
        if(instance.adjList[adj[v]].find(v2) != instance.adjList[adj[v]].end() && v1!=v2)  --nEdges;
      }

      solution = newSolution;
      return solution.calcCost();
    }

    bool localSearch(State & solution, int op){
      pcg32 rng(instance.nVertex);
      State neighboor;
      
      int min = INT_MAX;
      bool improve = false;

      do{
        improve = false;
        for(int i = 0; i < instance.nVertex;i++){
          if(op ==1){
            if(swap2VertexNeighboorhood(solution,neighboor,rng) < min) {
              int aux = neighboor.calcCost();
              if( aux < min){
              solution.selected = neighboor.selected;
               min = aux;
               improve = true;
              }
            }
         }

          if(op ==2){
            if(swapAdjacentNeighboorhood(solution,neighboor,i) < min) {
              int aux = neighboor.calcCost();
              if( aux < min){
              solution.selected = neighboor.selected;
               min = aux;
               improve = true;
              }
            }
          }
        }
      }while(improve);

      return improve;
    }


};

