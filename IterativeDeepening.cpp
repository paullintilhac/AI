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
	(stateVector)[level-1] = branch;
	//	cout<<"state vector: "<<(stateVector)[0]<<" "<<(stateVector)[1]<<" "<<(stateVector)[2]<<" "<<(stateVector)[3]<<endl;
	//cout<<"level: "<<level<<endl;
	float valSum = 0;
	float* totalTimes = new float[p.pCount];
		for (int j=0;j<p.tCount;++j){
			if (stateVector[j]>0){
				//cout<<"length: "<<p.lengths[j]<<", speed: "<<p.speeds[stateVector[j]-1]<<endl;
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
	
	//cout<<"totalTime: "<<totalTime<<", valSum: "<<valSum<<endl;
	if (totalTime<p.deadline && valSum >= p.targetVals){
		//cout<<"found goal state"<<endl;
		string returnString="";
		for (int i=0;i<p.tCount;++i){
			returnString =returnString+ to_string(stateVector[i]);
		}
		return(returnString);
	}

	for (int i=0;i<p.pCount+1;++i){
		if (level <maxLevel){
			string childReturnString = DFS(p,level+1,i,maxLevel,stateVector);
			if (childReturnString !="no solution"){
				return(childReturnString);
			}
		}
	}
	
	return("no solution");

}

int main(int argc,char* argv[]){
	char* fileName  = argv[1];
	//bool visited[4][2];
    static int totalTime, targetVals;
	Params tree(fileName);
	bool goal = false;
	int depthLevel=1;
	int* stateVector = new int[tree.tCount];
	//cout<<"tCount: "<<tree.tCount<<endl;
	string returnString = "";
	for (int j = 1;j<=tree.tCount;++j){
		//cout<<"stateVector: ";
		for (int i=0;i<tree.tCount;++i){
			stateVector[i]=0;
			//cout<<" "<<stateVector[i];
		}
		//cout<<endl;
		returnString = DFS(tree,0,0,j,stateVector);
		//cout<<"---------------"<<endl;
	}
	cout<<returnString<<endl;


    return(0);
}