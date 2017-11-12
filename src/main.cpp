#include "main.hpp"

/* Main window s*/
QWidget		*window = 0;
QPushButton	*buttons[16] = {0};
QBoxLayout	*layout = 0;
QGridLayout	*grid = 0;
QLabel		*label;

int
main (int argc, char **argv) {
	QApplication	app(argc, argv);
	
	init_window();
	window->show();

	/* TODO: Free memory */
	return (app.exec());
}