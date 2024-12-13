#include "solver.hpp"


int main(int argc, char* argv[]) {

	Graph g;

	instance.parse(argv[1]);
	State s1;
	State s2;
	Solver solver;
	 int rn = solver.randomEdge(s1);
	 int ratio = solver.ratioEdge(s2);
	 cout << "Instance" << endl;
	cout << argv[1] << endl;
	 if (!s1.verify()) {
	 	cout << "rnInvalid ";
	 }
	 else {
	 	cout << "RandomEdge: " << rn << " Time(ms): " << s1.timeSpent << endl;
	 }
	 if (!s2.verify()) {
	 	cout << "ratioInvalid" << endl;
	 }
	 else {
	 	cout << "RatioEdge: " << ratio << " Time(ms): " << s2.timeSpent << endl;
	 }
	 s2.toString();
	s1.selected = s2.selected;
	solver.localSearch(s2,2);
	s2.toString();

	solver.localSearch(s1,1);
	s1.toString();

}