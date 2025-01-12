#pragma once

#include "State.hpp"
#include "include/pcg_random.hpp"
#include <random>
#include <chrono>
#include <climits>
#include <deque>
#include <vector>


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

    static int swapAdjacentNeighboor(State & solution, State & newSolution, int index){

      newSolution = solution;
 
      newSolution.selected[index] = 0;

      for(int a: instance.adjList[index]){
        newSolution.selected[a] = 1;
      }
      
       return  newSolution.calcCost();

    }

    static int swap2VertexNeighboor(State & solution, State & newSolution,int v1, int v2){

      State aux;
      swapAdjacentNeighboor(solution, aux, v1);
      swapAdjacentNeighboor(aux, newSolution, v2);

     return newSolution.calcCost();

    }

    bool localSearch(void(*neighboorhood)(State &, vector<State> &,pcg32 &),State & solution, pcg32 & generator){
      vector<State> candidates;
      neighboorhood(solution, candidates, generator);
      State min = candidates[0];
      for(int i = 1 ; i < candidates.size(); ++i){
        if(candidates[i].cost < min.cost) min = candidates[i];
      }
      if(min.cost < solution.cost){
        solution = min;
        return true;
      }
      return false;
    }

	bool VNS(State & solution, int time){
         int op = 1;
         int loopLock= 0;
         pcg32 rng(instance.nVertex);
         chrono::high_resolution_clock::time_point tpStart = chrono::high_resolution_clock::now();
         while(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count() < time){
           loopLock = 0;
           while(loopLock < 2 && chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - tpStart).count() < time){
		   	vector<State> starts;
            if(op == 1) Solver::generateNeighboorhoodAdjacent(solution, starts, rng);
           	else Solver::generateNeighboorhoodRandom(solution, starts, rng);
           	int i =rng(starts.size());
           	State current = starts[i];
            if(op == 1) localSearch(Solver::generateNeighboorhoodAdjacent,current,rng);
           	else localSearch(Solver::generateNeighboorhoodRandom,current,rng);
           	if(current.cost < solution.cost){
            	 	loopLock = 0;
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
          for(int i = 0 ; i < instance.nVertex; i++){
            State aux;
            if(solution.selected[i] == 1){
            	swapAdjacentNeighboor(solution,aux,i);
                neighboorhood.push_back(aux);
            }
          }
   	}

    static void generateNeighboorhoodRandom(State & solution, vector<State> & neighboorhood,pcg32 & generator){

      neighboorhood.clear();
      State aux;
      int i= generator(solution.nSelected());
      int j= generator(solution.nSelected());
      swap2VertexNeighboor(solution,aux,i,j);
      neighboorhood.push_back(aux);
      for(i = 0 ; i < instance.nVertex; i++){
        if(solution.selected[i] == 1){

              j = generator(instance.nVertex);

            if(solution.selected[j] == 1 && i != j){

              swap2VertexNeighboor(solution,aux,i,j);
              neighboorhood.push_back(aux);
            }
        }
      }
    }

	bool tabuSearch(State & currentSolution, void(*neighboorhood)(State &, vector<State> &,pcg32 & ), deque<State> & tabuList, int tenure, pcg32 & generator){

      State neighboor;
      vector<State> candidates;
      neighboorhood(currentSolution,candidates,generator);
      State::heapSort(candidates);
      bool isPresent = false;
      for(int i = 0 ; i < candidates.size(); i++){

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

    bool tabu(State & solution,void(*neighboorhood)(State &, vector<State> &,pcg32 & ), int tenure, int time){
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

};

