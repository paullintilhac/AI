####

instructions for compiling: 
run the command

g++ -std=g++11 -o SEARCH *.cpp

then, in order to run the program, run the command:

./SEARCH -<i|h>[# restarts] <inputfile>

where i stands for iterative deepening, and h stands for hill climbing. Note that the 
[#restarts] parameter is required for the hill-climbing algorithm.

Example: 

./SEARCH -h10 example

######
