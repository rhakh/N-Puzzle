/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NButton.cpp
 * Author: rhakh
 * 
 * Created on November 14, 2017, 11:55 PM
 */

#include <qt5/QtWidgets/qpushbutton.h>

#include "NButton.h"
#include "main.hpp"

NButton::NButton(const QString &text, int _numb, int _ser_numb)
	: QPushButton(text), ser_numb(_ser_numb) {
	this->numb = _numb;
	connect(this, SIGNAL(clicked()), this, SLOT(click()));
}

NButton::~NButton() {
}

#define swap_int(a, b) \
	(a) = (a) ^ (b); \
	(b) = (a) ^ (b); \
	(a) = (a) ^ (b); \

void NButton::swapButtonsFields(NButton* a, NButton* b) {
	std::cout << "A: numb = " << a->numb << ", ser_num = " << a->ser_numb << std::endl;
	std::cout << "B: numb = " << b->numb << ", ser_num = " << b->ser_numb << std::endl;
	std::cout << std::endl;
	
	swap_int(a->numb, b->numb);
	
	b->setText(QString::number(b->numb));
	b->setEnabled(true);
	b->setCheckable(false);
	b->setChecked(false);
	
	a->setEnabled(false);
	a->setText("");
	a->setCheckable(true);
	a->setChecked(true);
}

void NButton::click() {
	int		current;
	int		x;
	int		y;
	int		top, right, bottom, left;
	/* |   |TOP|   |
	 * |LEF|CUR|RIG|
	 * |   |BOT|   |
	 */
	
	current = this->ser_numb;
	x = current % n_size;
	y = current / n_size;
	top = x + ((y - 1) * n_size);
	right = (x + 1 == n_size) ? (BIG_VAL) : ((x + 1) + (y * n_size));
	bottom = x + ((y + 1) * n_size);
	left = (x - 1 < 0) ? (BIG_VAL) : ((x - 1)  + (y * n_size));
	if ((top >= 0 && top < dsize) && (buttons[top]->numb == dsize)) {
		swapButtonsFields(this, buttons[top]);
	}
	else if ((right >= 0 && right < dsize) && (buttons[right]->numb == dsize)) {
		swapButtonsFields(this, buttons[right]);
	}
	else if ((bottom >= 0 && bottom < dsize) && (buttons[bottom]->numb == dsize)) {
		swapButtonsFields(this, buttons[bottom]);
	}
	else if ((left >= 0 && left < dsize) && (buttons[left]->numb == dsize)) {
		swapButtonsFields(this, buttons[left]);
	}	
}
