#ifndef HC_H
#define HC_H
#include "SearchAlgorithm.h"
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
			string returnString = "no solution";
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
			cout<<finalString<<endl;
			return(finalString);
		}
};
#endif