#include "Instance.hpp"
#include "State.hpp"


int main(int argc, char* argv[]) {

	Graph g;

	g.parse(argv[1]);

	g.printGraph();
}