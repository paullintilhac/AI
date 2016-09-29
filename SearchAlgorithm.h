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
	int* evaluateState(int* stateVector){
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
	int* result=new int[2];
	result[0] = (int)totalTime;
	result[1] = (int)valSum;
	return(result);
	}
};

class HC : public SearchAlgorithm {
public:
	HC(Params* p) : SearchAlgorithm(p){}
	getCost(int* stateVector){
		int* eval =new  int[2];
		eval = evaluateState(stateVector);
		int totalTime = eval[0];
		int valSum = eval[1];
		int excessTime = 0;
		int valueDeficit=0;
		if (totalTime-p->deadline>0){
			excessTime = totalTime-p->deadline;
		}

		if (p->targetVals-valSum>0){
			valueDeficit = p->targetVals-valSum;
		}

		return (excessTime+valueDeficit);
	}
	int* findBestNeighbor( int* stateVector){
		int lowestCost = getCost(stateVector);
		
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
				
				int cost = getCost(tempVector);

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
				
				int cost = getCost(tempVector);
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

		string HillClimber(int* stateVector){
			int* result = new int[p->pCount];
			int it = 0;
			string returnString = "No solution";
			//int initCost = getCost(p,stateVector);
			while(true){
			
			result = findBestNeighbor(stateVector);
			int newCost = getCost(result);
			
			bool same =true;
			for (int i=0;i<p->tCount;++i){
				if (stateVector[i]!=result[i]){
					same = false;
				}
			}
			if (newCost ==0){
				string successString = to_string(result[0]);
				for (int i=1;i<p->tCount;++i){
					successString = successString+" " +to_string(result[i]);
				}
				return(successString);
			}
			if ( it>10||same){
				break;
			}
			for (int i=0;i<p->tCount;++i){
				stateVector[i] =result[i];
			}
			}
			return(returnString);
		}
		string doSearch(){
			int* stateVector = new int[p->tCount];	
			int seed = time(NULL);
		    srand(seed);
			for (int i=0;i<p->tCount;++i){
				stateVector[i]=rand() % (p->pCount+1);
			}
			string finalString = HillClimber( stateVector);
			//cout<<finalString<<endl;
			return(finalString);
		}
};
class ID : public SearchAlgorithm {
public:
	ID(Params* p) : SearchAlgorithm(p){}
	string DFS(Params* p,int level, int branch,int maxLevel, int* stateVector){
		// note the level is 1-indexed
		stateVector[level-1] = branch; //update state vector

		//calculate value and time of current state vector
		int* eval = new int[2];
		eval = (evaluateState(stateVector));
		int totalTime = eval[0];
		int valSum = eval[1];

		//check if goal state has been reached
		if (totalTime<p->deadline && valSum >= p->targetVals){
			string returnString=to_string(stateVector[0]);
			for (int i=1;i<p->tCount;++i){
				returnString =returnString+ " "+to_string(stateVector[i]);
			}
			return(returnString);
		}

		//now iterate through children recursively
		for (int i=0;i<p->pCount+1;++i){
			if (level <maxLevel){
				string childReturnString = DFS(p,level+1,i,maxLevel,stateVector);
				//if the result was success, we need to exit inside loop
				if (childReturnString !="No solution"){
					return(childReturnString);
				}
			}
		}
		//entire p has been searched without goal state, meaning no solution
		return("No solution");
	}


	string doSearch(){
		int* stateVector = new int[p->tCount];
		string returnString = "";
		//iterate over different maximum depths
		for (int j = 1;j<=p->tCount;++j){
		//reset state vector at the start of each DFS
			for (int i=0;i<p->tCount;++i){
				stateVector[i]=0;
			}
			returnString = DFS(p,0,0,j,stateVector);
			//if goal is reached, exit before full depth
			if (returnString!="No solution"){
				break;
			}
		}
	//cout<<returnString<<endl;
	return(returnString);
	}

};

#endif