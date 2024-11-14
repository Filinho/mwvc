#include "State.hpp"

class Solver{
    public:
    
    Solver(){};

    int randomEdge(State & solution){
        // pick random edge
        int nEdges = instance.nEdges;
        int count = 0;
        vector<set<int>> auxAdjList = instance.adjList;
        while(nEdges > 0){
            int v1 = rand() % instance.nVertex;
            int v2;

            if(auxAdjList[v1].size()){   
                v2 = rand() % auxAdjList[v1].size();
                if (v2 >= 0 && v2 < auxAdjList[v1].size()) {
                auto it = auxAdjList[v1].begin();
                advance(it, v2);
                v2 = *it;
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
        

        for(int i : solution.selected){
            if(i) count++;
        } 
        return count;
    }
};

