#!/bin/bash

g++ -std=c++11 -o ID IterativeDeepening.cpp
#g++ -std=c++11 -o HC HillClimbing.cpp
./ID example
#./HC example
