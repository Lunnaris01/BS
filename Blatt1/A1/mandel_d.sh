#!/bin/bash
if [ "$#" -eq 3 ]; then
if [ "$1" -eq "$1" ] 2> /dev/null; then
if [ "$2" -eq "$2" ] 2> /dev/null; then
gcc -o mandel mandel.c
./mandel $1 $2 > $3
else
echo "Parameter 2 missing or not a number"
fi
else
echo "Parameter 1 missing or not a number"
fi
else
echo "Please provide 2 whole positive Numbers as Parameter 1 and 2 and an output file as parameter 3!"
fi