#include "solver.hpp"

#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace boost;

int main(int argc, char* argv[]) {

	po::options_description desc("Options");
	desc.add_options()
		("help", "show help")
		("instPath", "instance")
		("searchType", po::value<string>()->default_value("SA"), "search type: SA - Simulated Annealing, VNS - variable neighborhood search, LS - local search")
		("neighborhood", po::value<string>()->default_value("RM1"), "neighborhood that will be used: RM1 - Remove 1 vertex, RM2 - Remove 2 vertex")
		("localSearchType", po::value<string>()->default_value("FI"), "local search type: FI - First improvement, BI - Best improvement")
		//("initialSolution", po::value<string>()->default_value("randomEdge"), "used initial solution: randomEdge, ratioEdge")
		("maxMilli", po::value<unsigned>()->default_value(30000), "maximum time in milliseconds")
		("seed", po::value<unsigned>()->default_value(321), "random number generator seed")
		//("tenure", po::value<unsigned>()->default_value(8), "tabu tenure")
		("coolingFactor", po::value<double>()->default_value(0.997), "cooling factor of temperature for SA")
		("initialTemp", po::value<unsigned>()->default_value(300.0), "initial temperature for SA")
		;
	po::positional_options_description pod;
	pod.add("instPath", 1);
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(pod).run(), vm);
	po::notify(vm);

	if (vm.count("help") || !vm.count("instPath")) {
		cout << "necessary fields not present -- ";
		cout << desc << endl;
		return 1;
	}

	const string instPath = vm["instPath"].as<string>();
	string auxStr = vm["searchType"].as<string>();
	unsigned searchType;
	if (auxStr.compare("LS") == 0) {
		searchType = 0;
	}
	else if (auxStr.compare("SA") == 0) {
		searchType = 1;
	}
	else if (auxStr.compare("VNS") == 0) {
		searchType = 2;
	}
    else if(auxStr.compare("GN") == 0) {
          searchType = 3;
    }
	else {
		cout << "invalid searchType option ";
		cout << desc << endl;
		return 1;
	}

	auxStr = vm["neighborhood"].as<string>();
	unsigned neighborhood;

	if (auxStr.compare("RM1") == 0) {
		neighborhood = 1;
	}
	else if(auxStr.compare("RM2") == 0) {
		neighborhood = 2;
	}
	else {
		cout << "invalid neighborhood option";
		cout << desc << endl;
		return 1;
	}

	unsigned localSearchType;
	auxStr = vm["localSearchType"].as<string>();

	if (auxStr.compare("FI") == 0) {
		localSearchType = 1;
	}
	else if (auxStr.compare("BI") == 0) {
		localSearchType = 2;
	}
	else {
		cout << "invalid localSearchType option";
		cout << desc << endl;
		return 1;
	}

	const unsigned maxMilli = vm["maxMilli"].as<unsigned>();
	const unsigned seed = vm["seed"].as<unsigned>();
	const unsigned initialTemp = vm["initialTemp"].as<unsigned>();
	const double coolingFactor = vm["coolingFactor"].as<double>();

	pcg32 rng(seed);

	instance.parse(instPath);

	State s;
	
	Solver solver;
	solver.ratioEdge(s);

	if (!s.verify()) {
	 	cout << "ratioInvalid" << endl;
	}

	switch (searchType) {
		case 0:
			if (localSearchType == 1) {
				solver.localSearchFirstImprv(neighborhood, s, rng);
			}
			else {
				solver.localSearch(neighborhood, s, rng);
			}
			break;
		case 1:
			solver.simulatedAnnealing(neighborhood, s, rng, coolingFactor, initialTemp, instance.nVertex, maxMilli);
			break;
		case 2:
			solver.VNS(s, maxMilli, rng, localSearchType);
			break;
        case 3:
            solver.geneticAlgorithm(s, 20,5, 0.1, 60,rng);
	}

	if (!s.verify()) {
		cout << "Solucao invalida!!!" << endl;
	}
	else {
		cout << argv[1] << " " << s.cost << " " << s.timeSpent <<  endl;
		//s1.toString();
	}

}