#!/bin/sh 
echo "TEST____________________________________"; 
LD_PRELOAD=./libmalloc.so
LD_LIBRARY_PATH=.
echo "\n\n\n"
echo "UNIT"
./unit
echo "\n\n\n"
echo "memoryfootprint"
./memoryfootprint
echo "\n\n"
echo "advanced realloc"
echo "test 1"
./advancedrealloc
echo "\n\n"
echo "test 2"
./main
echo "\n\n"
echo "free && recycle"
echo "test 1"
./recycle
echo "\n\n"
echo "test 2"
./recycle-2
