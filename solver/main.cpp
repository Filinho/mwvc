#include "solver.hpp"


int main(int argc, char* argv[]) {

	Graph g;
	pcg32 rng(instance.nVertex);
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
	s1.selected = s2.selected;
	s1.toString();
    solver.simulatedAnnealing(1,s1,rng,0.9,10,5,30000);
    //solver.tabu(s1,Solver::generateNeighboorhoodRandom,20,30000);

    s1.toString();
    if (!s1.verify()) cout<<"invalido"<<endl;

}