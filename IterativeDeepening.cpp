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
	cout<<"state vector: "<<(stateVector)[0]<<" "<<(stateVector)[1]<<" "<<(stateVector)[2]<<endl;
	//cout<<"level: "<<level<<endl;
	float valSum = 0;
	float totalTime = 0;
		for (int j=0;j<p.tCount;++j){
			//cout<<"length: "<<p.lengths[j]<<", speed: "<<p.speeds[i]<<endl;\
			if (stateVector[j]>0){
				totalTime += p.lengths[j]/p.speeds[stateVector[j]-1];
				valSum+=p.lengths[j];	
			}
		}
	
	cout<<"totalTime: "<<totalTime<<", valSum: "<<valSum<<endl;
	if (totalTime<p.deadline && valSum >= p.targetVals){
		cout<<"found goal state"<<endl;
		string returnString="";
		for (int i=0;i<p.tCount;++i){
			returnString =returnString+ to_string(stateVector[i]);
		}
		return(returnString);
	}
	if (level <maxLevel){
	//cout<<"starting iteration at level "<<level+1<<endl;
	for (int i=0;i<p.pCount+1;++i){
		//cout<<"branch: "<<i<<endl;;
			DFS(p,level+1,i,maxLevel,stateVector);
		}
	}
	/*for (int i=0;i<p.tCount;++i){
		stateVector[i]=0;
	}*/
	//cout<<"complete, moving down a level"<<endl;	
	return("no solution");
}

int main(int argc,char* argv[]){
	char* fileName  = argv[1];
	//bool visited[4][2];

	Params tree(fileName);
	bool goal = false;
	int depthLevel=1;
	int* stateVector = new int[tree.pCount];
	cout<<"tCount: "<<tree.tCount<<endl;
	for (int i=0;i<tree.speeds.size();++i){
		cout<<"speed: "<<tree.speeds[i]<<endl;
	}

	string returnString = "";
	for (int j = 1;j<=1;++j){
		for (int i=0;i<tree.tCount;++i){
			stateVector[i]=0;
		}
		returnString = DFS(tree,0,0,j,stateVector);
		cout<<"---------------"<<endl;
	}
	cout<<returnString<<endl;
	//cout<<"num processes: "<<tree.pCount<<", num tasks: "<<tree.tCount<<", deadline: "<<tree.deadline<<", value minimum: "<<tree.targetVals<<endl;


    return(0);
}