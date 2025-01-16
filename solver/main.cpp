#include "solver.hpp"

//#include <boost/program_options.hpp>
//namespace po = boost::program_options;
//using namespace boost;

int main(int argc, char* argv[]) {

	//po::options_description desc("Options");
	//desc.add_options()
	//	("help", "show help")
	//	("instPath", "instance")
	//	("searchType", po::value<string>()->default_value("SA"), "search type: SA - Simulated Annealing, VNS - variable neighborhood search, LS1 - local search with remove 1 insert adjacent nighborhood, LS2 - local search with remove 2 insert adjacent neighborhood")
	//	("initialSolution", po::value<string>()->default_value("randomEdge"), "used initial solution: randomEdge, ratioEdge")
	//	("maxMilli", po::value<unsigned>()->default_value(300000), "maximum time in milliseconds")
	//	("seed", po::value<unsigned>()->default_value(13), "random number generator seed")
	//	//("tenure", po::value<unsigned>()->default_value(8), "tabu tenure")
	//	("coolingfactor", po:value<double>()->default_value(0.9), "cooling factor of temperature in SA")
	//	("initialTemp", po:value<double>()->default_value(300.0), "initial temperature in SA")
	//	;

	//unsigned neighborhood = atoi(argv[2]);
	unsigned neighborhood = 2; 
	unsigned initialTemp = atoi(argv[2]);
	double coolingFactor = atof(argv[3]);
	unsigned searchType = 2;

	Graph g;
	pcg32 rng(321);
	instance.parse(argv[1]);
	State s1;
	State s2;
	Solver solver;
	//int rn = solver.randomEdge(s1, rng);
	int ratio = solver.ratioEdge(s2);
	/*cout << "Instance " << instance.weigthSum() <<endl;
	cout << argv[1] << endl;*/
	/*if (!s1.verify()) {
		cout << "rnInvalid ";
	}
	else {
	 	cout << "RandomEdge: " << rn << " Time(ms): " << s1.timeSpent << endl;
	}*/
	if (!s2.verify()) {
	 	cout << "ratioInvalid" << endl;
	}/*
	else {
		cout << "RatioEdge: " << ratio << " Time(ms): " << s2.timeSpent << endl;
	}*/
	//s1.toString();
  //solver.tabu(s1,Solver::generateNeighboorhoodRandom,20,30000);

	State s = s2;
	switch (searchType) {
		case 0:
			solver.localSearch(neighborhood, s, rng);
			break;
		case 1:
			solver.localSearchFirstImprv(neighborhood, s, rng);
			break;
		case 2:
			solver.simulatedAnnealing(neighborhood, s, rng, coolingFactor, initialTemp, instance.nVertex, 30000);
			break;
		case 3:
			solver.VNS(s, 30000, rng);
			break;
	}

	if (!s.verify()) {
		cout << "Solucao invalida!!!" << endl;
	}
	else {
		cout << argv[1] << " " << s.cost << " " << s.timeSpent <<  endl;
		//cout << s.cost << endl;
		//s1.toString();
	}

}