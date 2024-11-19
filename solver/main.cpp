#include "solver.hpp"


int main(int argc, char* argv[]) {

	Graph g;

	instance.parse(argv[1]);
	State s;
	Solver solver;
	cout << "ratio solution: " << solver.ratioEdge(s) << endl;	
	State s2;
	cout << "random solution: "<< solver.randomEdge(s2) <<endl;
	cout<< s.verify() << s.verify();
}