#include "SearchAlgorithm.h"
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
float HC::getCost(int* stateVector){
	float* eval =new float[2];
	eval = evaluateState(stateVector);
	float totalTime = eval[0];
	float valSum = eval[1];
	float excessTime = 0;
	float valueDeficit=0;
	if (totalTime-p->deadline>0){
		excessTime = totalTime-p->deadline;
	}

	if (p->targetVals-valSum>0){
		valueDeficit = p->targetVals-valSum;
	}

	return (excessTime+valueDeficit);
}

int* HC::findBestNeighbor( int* stateVector){
	float lowestCost = getCost(stateVector);

	int* optimalVector = new int[p->tCount];
	int* tempVector = new int[p->tCount];
	for (int i=0;i<p->tCount;++i){
		for (int j=i+1;j<p->tCount;++j){
			for (int i = 0; i<p->tCount;++i){
				tempVector[i] = stateVector[i];
			}
			

			int temp = tempVector[i];
			tempVector[i] = tempVector[j];
			tempVector[j] = temp;
			
			float cost = getCost(tempVector);

			if (cost<lowestCost){
				lowestCost = cost;
				for (int k=0;k<p->tCount;++k){
					optimalVector[k] = tempVector[k];
				}
			}
		}
	}
	for (int i=0;i<p->pCount;++i){
		for (int j=0;j<(p->tCount+1);++j){
			for (int i = 0; i<p->tCount;++i){
				tempVector[i] = stateVector[i];
			}
			tempVector[j] ++;
			tempVector[j] = tempVector[j] % (p->pCount+1);
			
			float cost = getCost(tempVector);
			if (cost<=lowestCost){
				lowestCost = cost;
				for (int k=0;k<p->tCount;++k){
					optimalVector[k] = tempVector[k];
				}
			}
		}
	}	
		
	return(optimalVector);
}
string HC::HillClimber(int* stateVector){
	int* result = new int[p->pCount];
	int it = 0;
	string returnString = "No solution";
	int initCost = getCost(stateVector);
	float newCost;
	while(true){
	++it;
	//cout<<"----loop--------"<<endl;
	result = findBestNeighbor(stateVector);
	newCost = getCost(result);
	
	bool same =true;
	for (int i=0;i<p->tCount;++i){
		if (stateVector[i]!=result[i]){
			same = false;
		}
	}
	if (newCost ==0){
		//cout<<"GOAL STATE REACHED!"<<endl;
		string successString = to_string(result[0]);
		for (int i=1;i<p->tCount;++i){
			successString = successString+" " +to_string(result[i]);
		}
		cout<<"newCost: "<<newCost<<endl;
		return(successString);
	}
	if (newCost==initCost || it>1000 ){
		break;
	}
	for (int i=0;i<p->tCount;++i){
		stateVector[i] =result[i];
	}
	initCost = newCost;
	}
	cout<<"newCost: "<<newCost<<endl;
	return(returnString);
}

string HC::doSearch(){
	string finalString = "No solution";
	int seed = time(NULL);
	srand(seed);
	for (int i=0;i<rr;++i){
		int* stateVector = new int[p->tCount];	
	    cout<<"state vector: ";
		for (int i=0;i<p->tCount;++i){
			stateVector[i]=rand() % (p->pCount+1);
			cout<<to_string(stateVector[i])+" ";
		}
		cout<<endl;
		finalString = HillClimber( stateVector);
		cout<<finalString<<endl;
		if (finalString!="No solution"){
			break;
		}
	}
	return(finalString);
}