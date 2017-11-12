#include "main.hpp"

int
main (int argc, char **argv) {
	QApplication	app(argc, argv);
	QWidget			*window;
	
	window = new QWidget();
	window->setWindowFlags(Window | WindowCloseButtonHint | WindowMinimizeButtonHint);
	window->setWindowTitle("N-Puzzle");
	window->resize(400, 400);
	
	window->show();
	
	return (app.exec());
}