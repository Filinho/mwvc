#pragma once

#include "State.hpp"
#include "include/pcg_random.hpp"
#include <random>
#include <chrono>
#include <climits>
#include <deque>
#include <vector>
#include <cmath>

using namespace chrono;

class Solver{
  public:
    
  Solver(){};

  static void vertexRatioHeapifyvector(vector<pair<unsigned, double>>& arr, int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;


    if (left < n && arr[left].second > arr[smallest].second)
      smallest = left;


    if (right < n && arr[right].second > arr[smallest].second)
      smallest = right;

    if (smallest != i) {
      swap(arr[i], arr[smallest]);

      vertexRatioHeapifyvector(arr, n, smallest);
    }
  }

  static void vertexRatioHeapSort(vector<pair<unsigned, double>>& arr) {

    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
      vertexRatioHeapifyvector(arr, n, i);

    for (int i = n - 1; i > 0; i--) {

      swap(arr[0], arr[i]);
      vertexRatioHeapifyvector(arr, i, 0);
    }
  }

  int randomEdge(State & solution, pcg32& rng){

    chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();
        
      // pick random edge
    int nEdges = instance.nEdges;
    vector<set<int>> auxAdjList = instance.adjList;
    while(nEdges > 0){
      unsigned v1 = rng(instance.nVertex);
      unsigned v2;

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
        
    solution.calcCost();

    return solution.cost;
  }

  int ratioEdge(State & solution){

    chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();

    vector<set<int>> auxAdjList = instance.adjList;

    vector<pair<unsigned, double>> vertexRatioOrder;

    for (unsigned i = 0; i < instance.nVertex; ++i) {
      if (instance.adjList[i].size() == 0) continue;
      vertexRatioOrder.push_back(pair<unsigned, double>(i, (double)instance.weight[i] / (double)instance.adjList[i].size()));
    }

    vertexRatioHeapSort(vertexRatioOrder);

    int nEdges = instance.nEdges;
    int auxI=0;

    while (nEdges > 0) {
      int v = vertexRatioOrder[auxI].first;
      if (!auxAdjList[v].size()) {
        ++auxI;
        continue;
      }
      solution.selected[v] = 1;

      for (int j : auxAdjList[v]) {
        auxAdjList[j].erase(v);
        nEdges--;
      }

      auxAdjList[v].clear();
      auxI++;
    }

    solution.timeSpent = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count();

    solution.calcCost();

    return solution.cost;
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

  static int swapAdjacentNeighboor(State & solution, State & newSolution, unsigned index, unsigned x){

    newSolution = solution;
 
    newSolution.selected[index] = 0;

    for(int a: instance.adjList[index]){
      newSolution.selected[a] = 1;
    }
      
    return  newSolution.calcCost();

  }

  static int swap2VertexNeighboor(State & solution, State & newSolution, unsigned v1, unsigned v2){

    State aux;
    if(v1==v2) return swapAdjacentNeighboor(solution, newSolution, v1,-1);
    swapAdjacentNeighboor(solution, aux, v1,-1);
    swapAdjacentNeighboor(aux, newSolution, v2,-1);

    return newSolution.calcCost();

  }

  // has reached local minima?
  bool localSearch(unsigned opN, State & solution, pcg32 & generator){
    vector<State> candidates;

    void(*neighboorhood)(State&, vector<State> &, pcg32&);
    if (opN == 1) {
      neighboorhood = &Solver::generateNeighboorhoodAdjacent;
    }
    else {
      neighboorhood = &Solver::generateNeighboorhoodRandom;
    }

    chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();

    while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count() < 30000) {
      neighboorhood(solution, candidates, generator);
      State min = candidates[0];
      for(unsigned i = 1 ; i < candidates.size(); ++i){
        if (candidates[i].cost < min.cost) {
          min = candidates[i];
        }
      }

      if (min.cost >= solution.cost) {
        solution.timeSpent = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count();
        return true;
      }
      //cout << solution.cost << endl;
      solution = min;
    }
    solution.timeSpent = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count();

    return false;
  }

  bool localSearchFirstImprv(unsigned opN, State& solution, pcg32& rng) {
    int (*neighbor)(State&, State&, unsigned, unsigned);
    if (opN == 1) {
      neighbor = &Solver::swapAdjacentNeighboor;
    }
    else {
      neighbor = &Solver::swap2VertexNeighboor;
    }

    chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();
    unsigned iter = 0;
    unsigned maxIter = opN == 1 ? instance.nVertex : instance.nVertex * instance.nVertex;

    while (iter < maxIter && chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count() < 30000) {
      unsigned nSelected = solution.nSelected();
      unsigned i = rng(nSelected);
      unsigned j = rng(nSelected);

      i = solution.fromSelectedToIndex(i);

      if (opN == 2) j = solution.fromSelectedToIndex(j);

      State newNeighbor;
      neighbor(solution, newNeighbor, i, j);

      if (newNeighbor.cost < solution.cost) {
        iter = 0;
        solution = newNeighbor;
      }
      ++iter;
    } 

    solution.timeSpent = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count();

    if (iter >= maxIter) return true;

    return false;
  }

	bool VNS(State & solution, int time, pcg32& rng, unsigned localSearchType){
    int op = 1;
    int loopLock= 0;
    bool (Solver::*genericLocalSearch)(unsigned, State&, pcg32&);

    if (localSearchType == 1) {
      genericLocalSearch = &Solver::localSearchFirstImprv;
    }
    else {
      genericLocalSearch = & Solver::localSearch;
    }

    chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();

    while(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count() < time){
      loopLock = 0;

      while(loopLock < 2 && chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count() < time){

        vector<State> starts;

        if(op == 1) Solver::generateNeighboorhoodAdjacent(solution, starts, rng);
        else Solver::generateNeighboorhoodRandom(solution, starts, rng);

        int i =rng(starts.size());
        State current = starts[i];

        (this->*genericLocalSearch)(op, current, rng);

        if(current.cost < solution.cost){
          //loopLock = 0;
	        solution = current;
        }
        else{
          op = op == 1 ? 2 : 1;
          ++loopLock;
        }
	    }
    }

    return true;
   }


  static void generateNeighboorhoodAdjacent(State & solution,vector<State> & neighboorhood, pcg32 & generator){
    neighboorhood.clear();
    for(unsigned i = 0 ; i < instance.nVertex; i++){
      State aux;
      if(solution.selected[i] == 1){
        swapAdjacentNeighboor(solution,aux,i,-1);
        neighboorhood.push_back(aux);
      }
    }
  }

  static void generateNeighboorhoodRandom(State & solution, vector<State> & neighboorhood,pcg32 & generator){

    neighboorhood.clear();
    State aux;
    unsigned i= generator(solution.nSelected());
    unsigned j= generator(solution.nSelected());
    swap2VertexNeighboor(solution,aux,i,j);
    neighboorhood.push_back(aux);
    for(i = 0 ; i < instance.nVertex; i++){
      if(solution.selected[i] == 1){

        j = generator(instance.nVertex);

        if(solution.selected[j] == 1){

          swap2VertexNeighboor(solution,aux,i,j);
          neighboorhood.push_back(aux);
        }
      }
    }
  }

	bool tabuSearch(State & currentSolution, void(*neighboorhood)(State &, vector<State> &,pcg32 & ), deque<State> & tabuList, unsigned tenure, pcg32 & generator){

      State neighboor;
      vector<State> candidates;
      neighboorhood(currentSolution,candidates,generator);
      State::heapSort(candidates);
      bool isPresent = false;
      for(unsigned i = 0 ; i < candidates.size(); i++){

        for(State s : tabuList){

            isPresent = isPresent || candidates[i].compare(s);
            if(isPresent) break;
        }
        if(!isPresent){
          tabuList.push_back(candidates[i]);
          currentSolution = candidates[i];
          break;
        }
      }
      if(tabuList.size() > tenure) tabuList.pop_front();

      return true;
    }

  bool tabu(State & solution,void(*neighboorhood)(State &, vector<State> &,pcg32 & ), unsigned tenure, unsigned time){
		deque<State> tabuList;
        pcg32 rng(instance.nVertex);
        chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();
        State currentState = solution;
        while(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count() < time ){

            tabuSearch(currentState,neighboorhood,tabuList,tenure,rng);
            if(currentState.cost < solution.cost) solution = currentState;
        }
        return true;
    }

  double simulatedAnnealing(int opN, State & solution,pcg32 & generator, double cooling, double initialTemp,int equilibrium,int time){

      int (*neighboorhood)(State &, State &, unsigned, unsigned);

      if(opN == 1) neighboorhood = & Solver::swapAdjacentNeighboor;
      else neighboorhood = & Solver::swap2VertexNeighboor;

      State current = solution;

      double temp = initialTemp;
      //cout << solution.cost << endl;
      
      int e = 0;
      chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();
      while(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count() < time){
        e = 0;
        while(e < equilibrium && chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count() < time){
          unsigned i = generator(current.nSelected());
          unsigned j = generator(current.nSelected());

          i = current.fromSelectedToIndex(i);

          if (opN == 2) j = current.fromSelectedToIndex(j);

          State aux;
          neighboorhood(current,aux,i,j);
          int delta = aux.cost - current.cost;
          double prob = pow(M_E, (-delta / temp));
          double dist = (double)generator(INT_MAX) / (double)INT_MAX;

          if (prob > 1 || prob > dist) {
            current = aux;
            //cout << "aceita" << prob << " " << dist << " " << temp << " " << current.cost << " " << aux.cost ;
            if (aux.cost < solution.cost) {
              solution = aux;
              //cout << " melhora";
            }
            //cout << endl;
          }else{
            //cout << "nao aceita" << prob << " " << dist << endl; 
          }

          e++;
        }
        
        temp *= cooling;
      }

      solution.calcCost();
      
      return temp;
    }

};

