#include <stdio.h>
#include <vector>
#include <random>
#include <set>
#include "include/pcg_random.hpp"

using namespace std;

// generate graph G with argv[1] vertex, argv[2] edges and wheights of type argv[3]
int main(int argc, char* argv[]) {

	// generate random seed if available
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	// create random engine
	pcg32 rng(seed_source);

	unsigned n = atoi(argv[1]);				// n is number of vertex of graph G
	unsigned m = atoi(argv[2]);				// m is number of edges of graph G
	unsigned type = atoi(argv[3]);		// type is the way of set vertex wheights of grph G
		
	unsigned v1 = 0;									// v1 & v2 are a pair of vertex of graph G
	unsigned v2 = 0;
	vector<unsigned> deg(n+1, 0);			// deg[v] is degree of vertex v of graph G
	set<pair<unsigned, unsigned>> E;	// E is set of edges of graph G

	// choose a random pair of vertex [1, n] and add an edge between these two. Do it until graph G has m edges
	while (E.size() < m) {
		v1 = rng(n) + 1;
		v2 = rng(n) + 1;

		// add an edge v1-v2 if v1 != 2 and the edge v1-v2 doesn't exists in graph G
		if (v1 != v2 && E.find(pair<unsigned, unsigned>(v2, v1)) == E.end() && E.find(pair<unsigned, unsigned>(v1, v2)) == E.end()) {
			++deg[v1];
			++deg[v2];
			E.insert(pair<unsigned, unsigned>(v1,v2));
		}
	}

	vector<unsigned> w;

	// set wheights of vertex of G
	if (type == 1) {
		// type 1: select a random wheight in the uniform distribution from the interval [20, 120]
		uniform_int_distribution<int> uniformDistWheights(20, 120);
		for (unsigned i = 0; i < n; ++i) {
			w.push_back(uniformDistWheights(rng));
		}
	}
	else {
		// type 2: select a random wheight int the uniform distribution from the interval [1, deg[v]^2]
		for (unsigned i = 0; i < n; ++i) {
			unsigned top = (deg[i + 1] * deg[i + 1]);
			uniform_int_distribution<int> uniformDistWheights(1, top);
			w.push_back(uniformDistWheights(rng));
		}
	}

	/* prints the graph in the format:
	* n m								 : where n and m are the number of vertex and edges respectively
	* w1 w2 w3 w4 ... wn : where wi is the wheight f vertex i
	* e1
	* e2
	* e3
	* e4
	* .
	* .
	* .
	* em								 : where ei is edge i of graph G and ei is represented for two numbers i and j wich represents two vertex of graph G
	*/

	cout << n << " " << m << endl;

	for (unsigned i = 0; i < n; ++i) {
		cout << w[i] << " ";
	}
	cout << endl;

	for (auto it : E) {
		cout << it.first << " " << it.second << endl;
	}

	cout << endl;

}