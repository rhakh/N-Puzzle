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

#endif // MAIN_HPP
