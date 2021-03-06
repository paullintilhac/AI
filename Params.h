#ifndef PARAMS_H
#define PARAMS_H

#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

class Params{
public:
	vector<float> lengths,speeds;
	int deadline,targetVals,tCount,pCount;
	Params(char* fileName){

	//read first line, lengths of tasks
	ifstream infile (fileName	);
	string str;
	int id = 0;
	getline(infile, str);
    
    tCount = 0;
    pCount=0;
    istringstream iss(str);
    string s;
    while ( iss >> s){
	    tCount++;
	    lengths.push_back(atof(s.c_str()));
	}

	//read second line, process speeds
	getline(infile, str);
    istringstream iss2(str);
    while ( iss2 >> s){
	    pCount++;
	    speeds.push_back(atof(s.c_str()));
	}
	
	//read third line, deadline and value target
	getline(infile, str);
	istringstream iss3(str);
	float deadline,targetVals;
	iss3>>deadline;
	this->deadline = deadline;
	iss3>>targetVals;
	this->targetVals = targetVals;

	}
};


#endif