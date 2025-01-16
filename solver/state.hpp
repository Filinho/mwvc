#pragma once

#include "Instance.hpp"

class State{
  

public:
    vector<bool> selected; 
    unsigned timeSpent;
    int cost;
    State(){
        selected.resize(instance.nVertex);
        timeSpent = 0;
    }

    bool verify(){
        Graph copy = instance;

        for(unsigned i = 0; i < instance.nVertex ; i++){
            if(selected[i]){
                for(int j : copy.adjList[i]){
                    copy.adjList[j].erase(i);
                }   
                copy.adjList[i].clear();
            }
        }

        for(unsigned i = 0; i < instance.nVertex ; i++){
            if(copy.adjList[i].size() != 0) return false;
        }
        return true;
    }
    int calcCost(){
        int cost = 0;
        for(unsigned i = 0 ; i<selected.size();i++){
            if(selected[i]) cost+= instance.weight[i];
        }
        this->cost = cost;
        return cost;
    }
    int nSelected(){
        int cost = 0;
        for(unsigned i = 0 ; i<selected.size();i++){
            if(selected[i]) cost+= 1;
        }
        return cost;
    }
    void toString(){
        
        for(unsigned i = 0 ; i<selected.size();i++){
            if(selected[i]) cout << i << " ";
        }
        
        cout << endl << calcCost() << endl;
    }
    bool compare(State s1){
      for(unsigned i = 0 ; i<selected.size();i++){
        if(selected[i] != s1.selected[i]) return false;
      }
      return true;
    }

    static void heapify(vector<State> &arr, int n, int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;


        if (left < n && arr[left].cost > arr[smallest].cost)
            smallest = left;


        if (right < n && arr[right].cost > arr[smallest].cost)
            smallest = right;

        if (smallest != i) {
            swap(arr[i], arr[smallest]);

            heapify(arr, n, smallest);
        }
    }

    static void heapSort (vector<State> & arr){

        int n = arr.size();

        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        for (int i = n - 1; i > 0; i--) {

            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }

    unsigned fromSelectedToIndex(int nSelected) {
      unsigned i = 0;
      while (nSelected >= 0) {
        if (selected[i++]) --nSelected;
      }

      return i-1;
    }
};
