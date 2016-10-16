#!/bin/sh 
echo "TEST____________________________________"; 
LD_PRELOAD=./libmalloc.so 
echo "\n\n\n"
echo "UNIT"
./unit
echo "\n\n\n"
echo "memoryprint"
./memoryprint
echo "\n\n"
echo "advanced realloc"
echo "test 1"
./advancedrealloc
echo "\n\n"
echo "test 2"
./realloc_test
echo "\n\n"
echo "free && recycle"
echo "test 1"
./recycle
echo "\n\n"
echo "test 2"
./recycle2
