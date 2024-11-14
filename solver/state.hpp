#include "Instance.hpp"

class State{
  

public:
    vector<bool> selected;  
    State(){
        selected.resize(instance.nVertex);
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

    void toString(){
        for(int i = 0 ; i<selected.size();i++){
            if(selected[i]) cout << i << " ";
        } 
        cout << endl;
    }
};

