#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Params.h"

using namespace std;

string DFS(Params p,int level, int branch,int maxLevel, int* stateVector){
	// note the level is 1-indexed
	stateVector[level-1] = branch; //update state vector

	//calculate value and time of current state vector
	float valSum = 0;
	float* totalTimes = new float[p.pCount];
		for (int j=0;j<p.tCount;++j){
			if (stateVector[j]>0){
				totalTimes[stateVector[j]-1] += (float)p.lengths[j]/(float)p.speeds[stateVector[j]-1];
				valSum+=p.lengths[j];	
			}
		}
	float totalTime = 0;
	for (int i=0; i<p.pCount;++i){
		if (totalTimes[i]>totalTime){
			totalTime = totalTimes[i];
		}
	}
	
	//check if goal state has been reached
	if (totalTime<p.deadline && valSum >= p.targetVals){
		string returnString="";
		for (int i=0;i<p.tCount;++i){
			returnString =returnString+ to_string(stateVector[i]);
		}
		return(returnString);
	}

	//now iterate through children recursively
	for (int i=0;i<p.pCount+1;++i){
		if (level <maxLevel){
			string childReturnString = DFS(p,level+1,i,maxLevel,stateVector);
			//if the result was success, we need to exit inside loop
			if (childReturnString !="no solution"){
				return(childReturnString);
			}
		}
	}
	//entire tree has been searched without goal state, meaning no solution
	return("no solution");

}

int main(int argc,char* argv[]){
	char* fileName  = argv[1];
    static int totalTime, targetVals;
	Params tree(fileName);
	bool goal = false;
	int depthLevel=1;
	int* stateVector = new int[tree.tCount];
	string returnString = "";

	//iterate over different maximum depths
	for (int j = 1;j<=tree.tCount;++j){
		//reset state vector at the start of each DFS
		for (int i=0;i<tree.tCount;++i){
			stateVector[i]=0;
		}
		returnString = DFS(tree,0,0,j,stateVector);
		//if goal is reached, exit before full depth
		if (returnString!="no solution"){
			break;
		}
	}
	cout<<returnString<<endl;


    return(0);
}