/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "main.hpp"

/* Init buttons and add them to layout*/
void
init_buttons(void)
{
	/* TODO: set text size for button and style button*/
	for (int i = 0; i < 16; i++) {
		buttons[i] = new QPushButton(QString::number(i + 1));
		buttons[i]->setMinimumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
		buttons[i]->setMaximumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
		grid->addWidget(buttons[i], i / 4, i % 4);
	}
	buttons[15]->setEnabled(false);
	buttons[15]->setText("");
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
	init_buttons();
	label->setMargin(15);
	layout->addItem(grid);
	layout->addWidget(label);

	window->resize(MAIN_WIN_SIZE_W, MAIN_WIN_SIZE_H);
	window->setMaximumSize(QSize(MAIN_WIN_SIZE_W, MAIN_WIN_SIZE_H));
	window->setMinimumSize(QSize(MAIN_WIN_SIZE_W, MAIN_WIN_SIZE_H));
	window->setLayout(layout);
}