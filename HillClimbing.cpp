#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Params.h"

using namespace std;

int getCost(Params p,int* stateVector){
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
	int excessTime = 0;
	int valueDeficit=0;
	if (totalTime-p.deadline>0){
		excessTime = totalTime-p.deadline;
	}

	if (p.targetVals-valSum>0){
		valueDeficit = p.targetVals-valSum;
	}

	return (excessTime+valueDeficit);
}
int* findBestNeighbor(Params p, int* stateVector){
	int lowestCost = getCost(p,stateVector);
	
	int* optimalVector = new int[p.tCount];
	int* tempVector = new int[p.tCount];
	for (int i=0;i<p.tCount;++i){
		for (int j=i+1;j<p.tCount;++j){
			for (int i = 0; i<p.tCount;++i){
				tempVector[i] = stateVector[i];
			}
			

			int temp = tempVector[i];
			tempVector[i] = tempVector[j];
			tempVector[j] = temp;
			
			int cost = getCost(p,tempVector);

			if (cost<lowestCost){
				lowestCost = cost;
				for (int k=0;k<p.tCount;++k){
					optimalVector[k] = tempVector[k];
				}
			}
		}
	}
	for (int i=0;i<p.pCount;++i){
		for (int j=0;j<(p.tCount+1);++j){
			for (int i = 0; i<p.tCount;++i){
				tempVector[i] = stateVector[i];
			}
			tempVector[j] ++;
			tempVector[j] = tempVector[j] % (p.pCount+1);
			
			int cost = getCost(p,tempVector);
			if (cost<=lowestCost){
				lowestCost = cost;
				for (int k=0;k<p.tCount;++k){
					optimalVector[k] = tempVector[k];
				}
			}
		}
	}	
	
	return(optimalVector);
}

string HillClimber(Params tree,int* stateVector){
	int* result = new int[tree.pCount];
	int it = 0;
	string returnString = "no solution";
	//int initCost = getCost(tree,stateVector);
	while(true){
	
	result = findBestNeighbor(tree,stateVector);
	int newCost = getCost(tree,result);
	
	bool same =true;
	for (int i=0;i<tree.tCount;++i){
		if (stateVector[i]!=result[i]){
			same = false;
		}
	}
	if (newCost ==0){
		string successString = to_string(result[0]);
		for (int i=1;i<tree.tCount;++i){
			successString = successString+" " +to_string(result[i]);
		}
		return(successString);
	}
	if ( it>10||same){
		break;
	}
	for (int i=0;i<tree.tCount;++i){
		stateVector[i] =result[i];
	}
	}
	return(returnString);

}

int main(int argc,char* argv[]){
	char* fileName  = argv[1];
	//bool visited[4][2];
    static int totalTime, targetVals;
	Params tree(fileName);
	bool goal = false;
	int depthLevel=1;
	int* stateVector = new int[tree.tCount];	
	int seed = time(NULL);
    srand(seed);
	for (int i=0;i<tree.tCount;++i){
		stateVector[i]=rand() % (tree.pCount+1);
	}
	string finalString = HillClimber(tree, stateVector);
	cout<<finalString<<endl;
	

    return(0);
}
