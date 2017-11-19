/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "main.hpp"

void
default_init_buttons(void)
{
	/* TODO: set text size for button and style button*/
	/* TODO: generate random map */
	n_size = 4;
	dsize = 16;
	for (int i = 0; i < dsize; i++) {
		if (i + 1 == dsize)
			buttons[i] = new NButton(QString(""), 0, i);
		else
			buttons[i] = new NButton(QString::number(i + 1), i + 1, i);
		buttons[i]->setMinimumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
		buttons[i]->setMaximumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
		grid->addWidget(buttons[i], i / n_size, i % n_size);
	}
	buttons[dsize - 1]->setEnabled(false);
	buttons[dsize - 1]->setText("");
	buttons[dsize - 1]->setCheckable(true);
	buttons[dsize - 1]->setChecked(true);
}

/* Init buttons and add them to layout*/
void
init_buttons(void)
{
	/* TODO: set text size for button and style button*/
	for (int i = 0; i < dsize; i++) {
		if (button_values[i] != 0)
			buttons[i] = new NButton(QString::number(button_values[i]), button_values[i], i);
		else {
			buttons[i] = new NButton(QString(""), button_values[i], i);
			buttons[i]->setEnabled(false);
			buttons[i]->setText("");
			buttons[i]->setCheckable(true);
			buttons[i]->setChecked(true);
		}
		buttons[i]->setMinimumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
		buttons[i]->setMaximumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
		grid->addWidget(buttons[i], i / n_size, i % n_size);
	}
}

void
init_window(void)
{
	/* TODO: parse command line arguments*/
	/* TODO: add separator line between buttons and command side*/
	
	window = new QWidget();
	/* Global layout*/
	layout = new QHBoxLayout();
	/* Layout for buttons*/
	grid = new QGridLayout();
	label = new QLabel(QString("Options"));
	/* TODO: Abort if memory not allocated */
	window->setWindowFlags(Window |
							WindowCloseButtonHint |
							WindowMinimizeButtonHint |
							MSWindowsFixedSizeDialogHint);
	window->setWindowTitle("N-Puzzle");
	
	grid->setMargin(1);
	grid->setSpacing(1);
	if (n_size)
		init_buttons();
	else
		default_init_buttons();
	label->setMargin(15);
	layout->addItem(grid);
	layout->addWidget(label);

	window->resize(MAIN_WIN_SIZE_W, MAIN_WIN_SIZE_H);
	window->setMaximumSize(QSize(MAIN_WIN_SIZE_W, MAIN_WIN_SIZE_H));
	window->setMinimumSize(QSize(MAIN_WIN_SIZE_W, MAIN_WIN_SIZE_H));
	window->setLayout(layout);
}