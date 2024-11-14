#include "solver.hpp"


int main(int argc, char* argv[]) {

	Graph g;

	instance.parse(argv[1]);

	instance.printGraph();
	State s;
	Solver solver;
	cout << solver.randomEdge(s) << endl;

	cout << s.verify() <<endl;
	s.toString();
}