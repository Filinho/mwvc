#include "solver.hpp"


int main(int argc, char* argv[]) {

	Graph g;

	instance.parse(argv[1]);
	State s1;
	State s2;
	Solver solver;
	 int rn = solver.randomEdge(s1);
	 int ratio = solver.ratioEdge(s2);
	cout << argv[1] << endl;
	 if (!s1.verify()) {
	 	cout << "rnInvalid ";
	 }
	 else {
	 	cout << rn << " " << s1.timeSpent << " ";
	 }
	 if (!s2.verify()) {
	 	cout << "ratioInvalid" << endl;
	 }
	 else {
	 	cout << ratio << " " << s2.timeSpent << endl;
	 }
}