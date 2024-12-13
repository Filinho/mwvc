#pragma once

#include "Instance.hpp"

class State{
  

public:
    vector<bool> selected; 
    unsigned timeSpent;
    State(){
        selected.resize(instance.nVertex);
        timeSpent = 0;
    }

    bool verify(){
        Graph copy = instance;

        for(int i = 0; i < instance.nVertex ; i++){
            if(selected[i]){
                for(int j : copy.adjList[i]){
                    copy.adjList[j].erase(i);
                }   
                copy.adjList[i].clear();
            }
        }

        for(int i = 0; i < instance.nVertex ; i++){
            if(copy.adjList[i].size() != 0) return false;
        }
        return true;
    }
    int calcCost(){
        int cost = 0;
        for(int i = 0 ; i<selected.size();i++){
            if(selected[i]) cost+= instance.weight[i];
        } 
        return cost;
    }
    int nSelected(){
        int cost = 0;
        for(int i = 0 ; i<selected.size();i++){
            if(selected[i]) cost+= 1;
        }
        return cost;
    }
    void toString(){
        
        for(int i = 0 ; i<selected.size();i++){
            if(selected[i]) cout << i << " ";
        }
        
        cout << endl << calcCost() << endl;
    }
};

