#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

#include "main.hpp"
#include "CLI.hpp"
#include "NPuzzleSolver.hpp"

bool	CLI::isFlagSet(const std::string &flag) const {
	if (this->vm.count(flag))
		return (true);
	return (false);
}

int		CLI::getIntFlag(const std::string &flag) const {
	if (this->vm.count(flag)) {
		// trow exception if conversion failed
		return (this->vm[flag].as<int>());
	}
	throw CLI_flagNotSet();
	return (0);
}

std::string	CLI::getStringFlag(const std::string &flag) const {
	if (this->vm.count(flag)) {
		// trow exception if conversion failed
		return (this->vm[flag].as<std::string>());
	}
	throw CLI_flagNotSet();
	return (0);
}

bool	CLI::processArguments(int argc, char **argv) {
	namespace po = boost::program_options;
	try {
		po::store(po::parse_command_line(argc, argv, this->desc), this->vm);
		po::notify(this->vm);

		if (this->vm.count("verbose")) {
			verboseLevel = this->vm["verbose"].as<int>();
		}
		if (this->vm.count("help")) {
			std::cout << desc << std::endl;
		}
		if (this->vm.count("file")) {
			fileName = this->vm["file"].as<std::string>();
		}
	}
	catch (po::error &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return (false);
	}
	return (true);
}

CLI::CLI(int argc, char **argv) : desc("Options") {
	namespace po = boost::program_options;
	this->desc.add_options()
			("help,h", "Print help")
			("verbose,v", po::value<int>(), "Verbose level\n"
								"\t0 -- no prints\n"
								"\t1 -- server prints\n"
								"\t2 -- algorithm prints")
			("file,f", po::value<std::string>(), "File with map to solve")
			("exit,e", "Close programm, when map will be solved");

	if (!processArguments(argc, argv))
		throw CLI_invalidArguments();
}

CLI::~CLI() {}

static NP_retVal	solvePuzzle(const int *map, int mapSize) {
	NPuzzleSolver	solver;
	NP_retVal		result;
	clock_t			start;

	try {
		start = clock();
		solver.solve(0, 0, map, mapSize * mapSize, result);
		start = clock() - start;
		double elapsedTime = (double)start / CLOCKS_PER_SEC;
	}
	catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return (result);
}

void	CLI::startLogic() const {
	std::vector<int>	resultVector;
	std::vector<int>	map;
	std::ifstream		file(this->getStringFlag("file"));
	std::string			line;
	NP_retVal			result;
	int					mapSize;

	if (!file.good())
		throw CLI_InvalidFile();

	while(std::getline(file, line)) {
		// split line by whitespaces
		std::istringstream iss(line);
		std::vector<std::string> strings {
			std::istream_iterator<std::string>(iss), {}
		};

		try {
			for (auto str : strings) {
				if (str.find('#') == std::string::npos)
					resultVector.push_back(boost::lexical_cast<int>(str));
				else
					break;
			}
		}
		catch(boost::bad_lexical_cast &e) {
			std::cerr << e.what() << std::endl;
			throw CLI_InvalidMap();
		}
	}
	file.close();

	for (auto it : resultVector)
		std::cout << it << ", ";
	std::cout << std::endl;
	std::cout << "size = " << resultVector.size() << std::endl;

	if (resultVector.size() < 2)
		throw CLI_InvalidMap();

	mapSize = resultVector[0];
	for (unsigned i = 1; i < resultVector.size(); i++)
		map.push_back(resultVector[i]);
	result = solvePuzzle(map.data(), mapSize);
	std::cout << "SUCCESS" << std::endl;
}
