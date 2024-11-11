#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>

using namespace std;

class Graph {

    int vertices;
    vector<set<int>> adjList;
    
    public:
    Graph(int v, int e) : vertices(v){
        adjList.resize(v);
        for(int i = 0; i< v; i++){
            adjList[i] = set<int>();
        }
        int edgesAdded = 0;
        srand(time(0)); 

        while(edgesAdded< e){
            int u = rand() % vertices;
            int v = rand() % vertices;
            if (u != v && adjList[u].find(v) == adjList[u].end()) {
                adjList[u].insert(v);
                adjList[v].insert(u);
                edgesAdded++;
            }
        }
    }

 void printGraph() {
        cout  << vertices << endl;
        for (int i = 0; i < vertices; i++) {
            cout << i << " ";
            for (int neighbor : adjList[i]) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    };
};


int main(int argc, char * argv[]){
    Graph  i = Graph(atoi(argv[1]),atoi(argv[2]));
    i.printGraph();
}