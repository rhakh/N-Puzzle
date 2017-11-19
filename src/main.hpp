#ifndef MAIN_HPP
#define MAIN_HPP

#include <QtGui>
#include <QtWidgets>
#include <vector>
#include "NButton.h"


#define MAIN_WIN_SIZE_W 500
#define MAIN_WIN_SIZE_H 240
#define BUTTON_SIZE 50
#define BIG_VAL 98745635
//#include <QtWidgets/QApplication>
//#include <QtWidgets/QWidget>
//#include <qnamespace.h>

using namespace Qt;

extern QWidget		*window;
extern NButton		*buttons[16];
extern QBoxLayout	*layout;
extern QGridLayout	*grid;
extern QLabel		*label;
extern int			n_size;
extern int			dsize;
extern std::vector<int>		button_values;

void
init_window(void);


#endif /* MAIN_HPP */
