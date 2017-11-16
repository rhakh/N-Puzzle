#!/bin/bash

qmake -project
qmake -makefile
PRO_FILE="`find . -name "*.pro"`"
echo -e "\nQT += widgets gui\nCONFIG += console\n" >> $PRO_FILE
