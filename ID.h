#ifdef ID_H
#define ID_H
#include "SearchAlgorithm.h"

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
		string returnString="";
		for (int i=0;i<p->tCount;++i){
			returnString =returnString+ to_string(stateVector[i]);
		}
		return(returnString);
	}

	//now iterate through children recursively
	for (int i=0;i<p->pCount+1;++i){
		if (level <maxLevel){
			string childReturnString = DFS(p,level+1,i,maxLevel,stateVector);
			//if the result was success, we need to exit inside loop
			if (childReturnString !="no solution"){
				return(childReturnString);
			}
		}
	}
	//entire p has been searched without goal state, meaning no solution
	return("no solution");
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
			if (returnString!="no solution"){
				break;
			}
		}
	cout<<returnString<<endl;
	return(returnString);
	}

};

#endif