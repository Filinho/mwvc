#include "State.hpp"


int main(int argc, char* argv[]) {

	Graph g;

	instance.parse(argv[1]);

	instance.printGraph();
	State s;
	s.selected = {true, true, false, false, false};
	cout << s.verify();
}