#include "solver.hpp"


int main(int argc, char* argv[]) {

	Graph g;

	instance.parse(argv[1]);
	State s;
	Solver solver;
	cout << solver.ratioEdge(s) << endl;
	State s2;
	cout << solver.randomEdge(s2) <<endl;
	s.toString();
	s2.toString();
}