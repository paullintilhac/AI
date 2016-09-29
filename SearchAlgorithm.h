#ifndef SEARCHALGORITHM_H
#define SEARCHALGORITHM_H

#include "Params.h"

class SearchAlgorithm{
	public:
	Params* p;
	SearchAlgorithm(Params* par) {
		p = par;
	}
	virtual string doSearch(){
		return("search function has not been overriden in derived class");
	};
	float* evaluateState(int* stateVector){
		float valSum = 0;
		float* totalTimes = new float[p->pCount];
		for (int j=0;j<p->tCount;++j){
			if (stateVector[j]>0){
				totalTimes[stateVector[j]-1] += (float)p->lengths[j]/(float)p->speeds[stateVector[j]-1];
				valSum+=p->lengths[j];	
			}
		}

	float totalTime = 0;
	for (int i=0; i<p->pCount;++i){
		if (totalTimes[i]>totalTime){
			totalTime = totalTimes[i];
		}
	}
	float* result=new float[2];
	//cat examplecout<<"total time: "<<totalTime<<", valSum: "<<valSum<<endl;
	result[0] = totalTime;
	result[1] = valSum;
	return(result);
	}
};

class HC : public SearchAlgorithm {
public:
	int rr;
	HC(Params* p,int r) : SearchAlgorithm(p){
		rr = r;
	}
	float getCost(int* stateVector);
	int* findBestNeighbor(int* stateVector);
	string HillClimber(int* stateVector);
	string doSearch();
	
};

class ID : public SearchAlgorithm {
public:
	ID(Params* p) : SearchAlgorithm(p){}
	string DFS(Params* p,int level, int branch,int maxLevel, int* stateVector);
	string doSearch();
};

#endif