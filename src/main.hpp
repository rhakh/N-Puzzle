#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>

enum verboseLevel_e { NONE, SERVER, ALGO };

#define NP_VBL_NONE		0x0
#define NP_VBL_SERVER	0x1
#define NP_VBL_PATH		0x2
#define NP_VBL_RESULT	0x4
#define NP_VBL_ALL		0x7

extern std::string	fileName;
extern int	verboseLevel;
extern int	optimisationByTime;

#endif // MAIN_HPP


/*
	Code conventions:
		1. Everything in camelCase
		2. 'using' / 'using namespace' can be applied in scope of function
		3. Short alias for this namespace -> namespace pt = boost::property_tree;
		4. Names of enumes and defines starts with 'NP_'

	TODO:
		+1. Makefile for macos
		+2. -std=c++11 in Makefile ?
		-3. Start new thread to solve puzzle, and send signal if client
			was send a stop message : server is not asynchronus
		-4. Use unsigned int in everywhere in NPuzzleSolver / doesn't needed
		+5. Make an API of NPuzzleSolver, means in header files must be
			everything what needs for NPuzzleSolver
		-6. Think about using enum as argument in NPuzzleSolver class / doesn't needed now
		+7. Use incline function in NPuzzleSolver.cpp instead of SWAP / was redesined
		+8. Leaks in NPsolver / resolved
		+9. Add -O2 flag to Makefile
		+10. Includes must be in cpp files, not in hpp
		*-11. Use multithreads to find solution in NPuzzleSolver.cpp / this doesn't increase speed
		+12. Put to NPuzzleSolver opennodes, closednodes, momory used
		-13. rvalue reference, use it to return value from function
		-14. Add mutex to NPuzzleSolver: stop task only when task is running
		+15. Snail solution, discovery
		-16. Asynchronus response from server
		+17. Figure out how to build finishState
		+18. Fix manhattanDistance for snail solution
		+19. Redesign class State
		+20. Abort when solving 3*3 puzzle / was fixed
		+21. Rename HAMMILTON_DISTANCE to MISPLACED_TILES
		-22. Usetwo dimension array in State.cpp
		?23. rewrite default GET method for server
		+24. Delete all debug print
		25. validate map for numbers (clone numbers, empty numbers)
		+26. bug in N_MAXSWAP / was fixed
		27. Check amount of numbers in line, when read from file
		+28. Strange flag parsing behavior / was fixed
*/


/*

Open issue ("#23" for example)
Locally create branch from desired branch, as usual I name branch i{issue_number} (i23)
Once, i've done fixes for issue I create pull request and in the pull request body write "fixes #23"
This automatically closes "issue #23"

*/


/*
 * How to profile N-Puzzle
 * $ g++ -std=c++11 -O2 -pg ...
 * $ ./a.out
 * $ gprof a.out > prof.txt
 * g++ -std=c++11 -I $HOME/.brew/Cellar/boost/1.67.0_1/include -L $HOME/.brew/Cellar/boost/1.67.0_1/lib  -lboost_system  -lboost_thread-mt
 */
