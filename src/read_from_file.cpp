#include <iostream>
#include <fstream> // To use ifstream
#include <vector>
#include <regex>
#include <boost/lexical_cast.hpp>

/*
parse int in string
int main(int argc, char **argv)
{
    using boost::lexical_cast;
    using boost::bad_lexical_cast;

    std::vector<int> args;

	for (int i = 0; i < argc; i++) {
        try {
            args.push_back(lexical_cast<int>(argv[i]));
        }
        catch(bad_lexical_cast &) {
            args.push_back(0);
        }
    }

	std::cout << "Numbers: " << std::endl;
	for (auto number : args) {
		std::cout << number << ", ";
	}
	std::cout << std::endl;
	return (0);
}
*/

/*
read number from file
using namespace std;

int main(int argc, char **argv)
{
    std::vector<int> numbers;
    ifstream inputFile(argv[1]);        // Input file stream object

    // Check if exists and then open the file.
    if (inputFile.good()) {
        // Push items into a vector
        int current_number = 0;
        while (inputFile >> current_number){
            numbers.push_back(current_number);
        }

        // Close the file.
        inputFile.close();

        // Display the numbers read:
        cout << "The numbers are: ";
        for (int count = 0; count < numbers.size(); count++){
            cout << numbers[count] << " ";
        }

        cout << endl;
    }else {
        cout << "Error!";
        _exit(0);
    }

    return 0;
}
*/

int main(int argc, char **argv)
{
	std::vector<int>	resultVector;
	std::ifstream		file(argv[1]);
	std::string			line;

	while(std::getline(file, line)) {
		// split line by whitespaces
		std::istringstream iss(line);
		std::vector<std::string> result {
			std::istream_iterator<std::string>(iss), {}
		};

		try {
			for (auto it : result) {
				if (it.find('#') == std::string::npos) {
					// it - is norm value , add it to resultVector
					resultVector.push_back(boost::lexical_cast<int>(it));
				}
			}
        }
        catch(boost::bad_lexical_cast &) {
			// throw exception badValue
        }
    }

	std::cout << "resultVector " << std::endl;
	for (auto it : resultVector)
		std::cout << it << ", ";
	std::cout << std::endl;
	return (0);
}
