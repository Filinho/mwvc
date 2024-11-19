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
        

        for(int i : solution.selected){
            if(i) count += instance.weight[i];
        } 
        return count;
    }

    int ratioEdge(State & solution){
        vector<int> order;
        vector<set<int>> auxAdjList = instance.adjList;
        order.resize(instance.nVertex);
        order[0] = 0;
     
        for(int i = 1 ; i< instance.nVertex;i++){
            
            int j = i -1;
            while( j >=0  && (float) instance.weight[i]/instance.adjList[i].size() < (float)  instance.weight[order[j]]/instance.adjList[order[j]].size()){
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
        int count = 0;
        for(int i = 0 ; i< instance.nVertex;i++){
            if(solution.selected[i] == 1) count += instance.weight[i];
        }
        return count;
    }

};

