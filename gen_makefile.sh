#!/bin/bash

qmake -project
qmake -makefile
PRO_FILE="`find . -name "*.pro"`"
echo -e "\nQT += widgets gui\n" >> $PRO_FILE
