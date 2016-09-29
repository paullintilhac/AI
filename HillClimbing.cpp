#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Params.h"

using namespace std;

string HillClimbing(Params p){


}

int main(int argc,char* argv[]){
	char* fileName  = argv[1];
	//bool visited[4][2];
    static int totalTime, targetVals;
	Params tree(fileName);
	bool goal = false;
	int depthLevel=1;
	int* stateVector = new int[tree.tCount];	
	
	for (int i=0;i<tree.tCount;++i){
		stateVector[i]=rand() % (tree.pCount+1);
	}

    return(0);
}

int* findBestNeighbor(Params p, int* stateVector){
	int lowestCost = getCost(p,stateVector);
	int* optimalVector = new int[p.tCount];
	for (int i=0;i<p.tCount;++p){
		for (int j=i+1;j<p.tCount;++p){
			int* tempVector = new int[p.tCount];
			int temp = tempVector[i];
			tempVector[i] = tempVector[j]
			tempVector[j] = temp;
			int cost = getCost(p,tempVector);
			if (cost<lowestCost){
				lowestCost = cost;
				optimalVector = tempVector;
			}
		}
	}

	for (int i=0;i<p.pCount;++i){
		for (int j=0;j<p.tCount;++j){
			int* tempVector = new int[p.tCount];
			tempVector[j] ++;
			tempVector[j] = tempVector[j] % p.pCount;
			int cost = getCost(p,tempVector);
			if (cost<lowestCost){
				lowestCost = cost;
				optimalVector = tempVector;
			}
		}
	}

}

int getCost(Params p,int* stateVector){
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
	return (max(totalTime -p.deadline,0)+max(p.targetVals-valSum,0))
}