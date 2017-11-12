#ifndef MAIN_HPP
#define MAIN_HPP

#include <QtGui>
#include <QtWidgets>

#define MAIN_WIN_SIZE_W 500
#define MAIN_WIN_SIZE_H 240
#define BUTTON_SIZE 50
//#include <QtWidgets/QApplication>
//#include <QtWidgets/QWidget>
//#include <qnamespace.h>

using namespace Qt;

extern QWidget		*window;
extern QPushButton	*buttons[16];
extern QBoxLayout	*layout;
extern QGridLayout	*grid;
extern QLabel		*label;


void
init_window(void);


#endif /* MAIN_HPP */
