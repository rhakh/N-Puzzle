#include "main.hpp"

/* Main window s*/
QWidget		*window = 0;
NButton		*buttons[16] = {0};
QBoxLayout	*layout = 0;
QGridLayout	*grid = 0;
QLabel		*label;
int			n_size = 4;
int			dsize = 16;
std::vector<int>	button_values;

static int
processInput() {
	QTextStream	stream(stdin);
	QRegExp		rg("(\\ |\\t)");
	QString		line;
	QStringList	list;
	
	while ((line = stream.readLine()) != NULL) {
		std::cout << "line = " << line.toLatin1().data() << std::endl;
		if (line.at(0).toLatin1() == '#')
			continue;
		list = line.split(rg);
		if (list.size() == 1)
			n_size = list.at(0).toInt();
		else if (list.size() == n_size) {
			foreach (const QString &elem, list) {
				button_values.push_back(elem.toInt());
			}
		}
		else {
			std::cout << "Error in line: " << line.toLatin1().data() << std::endl;
			return (1);
		}
	}
	std::cout << "n_size = " << n_size << std::endl;
	for (int i = 0; i < button_values.size(); i++)
		std::cout << "v[" << i << "] = " << button_values[i] << std::endl;
	return (0);
}

int
main (int argc, char **argv) {
	QApplication	app(argc, argv);
	int				ret = 0;
	
	if (argc == 2 && !strcmp(argv[1], "-s")) {
		if (processInput())
			return (1);
	}
	if (n_size <= 5) {
		init_window();
		window->show();
		ret = app.exec();
	}
	/* TODO: Free memory */
	return (ret);
}