#!/bin/bash

#if macOS
# qmake -project
# qmake -spec macx-clang
# make

unamestr=`uname`

qmake -project "QT += widgets"

if [[ $unamestr == "Darwin" ]]; then
	qmake -spec macx-clang
	echo "I am Darwin"
elif [[ $unamestr == "Linux" ]]; then
	qmake -makefile
fi


#PRO_FILE="`find . -name "*.pro"`"
#echo -e "\nQT += widgets gui\nCONFIG += console\n" >> $PRO_FILE
# echo -e 'QT += widgets gui\nCONFIG += console' | cat - $PRO_FILE > temp && mv temp $PRO_FILE