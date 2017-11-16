/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NButton.h
 * Author: rhakh
 *
 * Created on November 14, 2017, 11:55 PM
 */

#ifndef NBUTTON_H
#    define NBUTTON_H
#	include <iostream>
#	include <cstdlib>
#	include <QtWidgets>

class NButton : public QPushButton {

	Q_OBJECT

private:
	int			numb;		/* text in number */
	const int	ser_numb;	/* serial number */
	
	void	swapButtonsFields(NButton *a, NButton *b);

public:
	NButton(const QString &text, int _numb, int _ser_num);
	NButton(const NButton& orig);
	virtual ~NButton();

public slots:
	void click();
	
};

#endif /* NBUTTON_H */

