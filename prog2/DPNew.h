#include <stdio.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include "Clauses.h"
using namespace std;


class DP{
public:
	vector<vector<int> > clauses;
	vector<vector<int> > simpClauses;
	vector<int> literalSet;
	vector<int> literalValues;
	DP(vector<vector<int> > c){ 
		clauses  = c;
		simpClauses = c;
		for (int i=0;i<clauses.size();++i){

			for (int j=0;j<clauses[i].size();++j){
				bool newLiteral = true;
				for (int k=0;k<literalSet.size();++k){
					if (abs(clauses[i][j])==abs(literalSet[k]))
						newLiteral=false;
				}
				if (newLiteral)
					literalSet.push_back(literalSet.size()+1);
			}
		}
		for (int i=0;i<literalSet.size();++i){
			literalValues.push_back(0); // 0 means that the literal has not been assigned a value
		}	
	};

	//bool run_dp(int depth, int branch, vector<int> values){
	Clauses run_dp(int depth,Clauses clauses,int branch){

			cout<<"DEPTH AT TOP: "<<depth<<", branch: "<<branch<<endl;
			if (branch==0){
			clauses = reduce(literalSet[depth],clauses,depth);
			
			} else{
			clauses = reduce(-literalSet[depth],clauses,depth);
			}

			bool scan=true;
			while(scan){
				vector<int> literalUpdates = checkForUnits(clauses.sentences);
				vector<int> pureUpdates = checkForPureLiterals(clauses.sentences);
				if (literalUpdates.size()==0 && pureUpdates.size()==0){
					scan=false;
				}
				if (scan){
				for (int j=0;j<literalUpdates.size();++j){
					clauses = reduce(literalUpdates[j],clauses,abs(literalUpdates[j])-1);

					//cout<<"sentences[0][0]: "<<sentences[0][0]<<endl;
					if (clauses.status == "fail"){
						//cout<<"no solution after literal"<<endl;
						scan=false;
						clauses.returnString = "0\n";

					return(clauses);
					}
					if (clauses.status == "success"){
						//cout<<"returning with success"<<endl;
						return(clauses);
					}
					if (true){
					for (int k=0;k<literalSet.size();++k){
						cout<<clauses.states[k]<<" ";
					}
					cout<<endl;
					}
				}

				for (int j=0;j<pureUpdates.size();++j){

					clauses = reduce(pureUpdates[j],clauses,abs(pureUpdates[j])-1);

					if (clauses.status == "fail"){
						//cout<<"no solution after pure"<<endl;
						scan = false;
						clauses.returnString = "0\n";
						return(clauses);
					}
					if (clauses.status == "success"){
						//cout<<"returning with success"<<endl;
						return(clauses);
					}
					if (true){
					//cout<<"updated literals after eliminating pure literals: "<<endl;
					for (int k=0;k<literalSet.size();++k){
						cout<<clauses.states[k]<<" ";
					}
					cout<<endl;
					}
				}
				}
			}		

			for (int i=0;i<2;++i){
			//cout<<"DEPTH: "<<depth<<", i: "<<i<<", status: "<<clauses.status<<endl;	
			
			if (true){
			//cout<<"updated literals: "<<endl;
			for (int j=0;j<literalSet.size();++j){
				cout<<clauses.states[j]<<" ";
			}
			cout<<endl;
			}
			int nextOpenDepth = -1;
			for (int j=depth+1;j<literalSet.size();++j){
				if (clauses.states[j]==0){
					nextOpenDepth = j;
					break;
				}
				
			}
			//as long as it hasn't reached the end, bontinue calling recursively
			if (nextOpenDepth<literalSet.size()){
			Clauses childClauses =run_dp(nextOpenDepth, clauses,i);
				//only receive state from child if it was a success 
				//so that other children start with clean slate
				if (childClauses.status=="success")
				clauses = childClauses;		
			}
		}
		//cout<<"returning failure"<<endl;
		return(clauses);
	};

	vector<int> checkForUnits(vector<vector<int>> c){
		//cout<<"checking for units "<<endl;
		vector<int> literalUpdates;
		for (int i=0;i<c.size();++i){
			if (c[i].size()==1 && c[i][0]!=0){
				//cout<<"found unit "<<c[i][0]<<endl;
				literalUpdates.push_back(c[i][0]);
			}
		}
		return literalUpdates;
	};

	vector<int> checkForPureLiterals(vector<vector<int>> c){
		//cout<<"checking for pure literals "<<endl;
		vector<int> polarity;
		vector<bool> pure;
		vector<int> pureUpdates;
		for (int i=0;i<literalSet.size();++i){
			polarity.push_back(0);
			pure.push_back(false);
		} 

		//check for purity -- runs in polynomial (~n^2) time
		for (int i=0;i<c.size();++i){
			for (int j=0;j<c[i].size();++j){
				int index = abs(c[i][j])-1;

				if (polarity[index]==0){
					pure[index]=true;
					if (c[i][j]>0){
						polarity[index]=1;
					} else{
						polarity[index]=-1;
					}
				} else{
					if ((c[i][j]>0 && polarity[index]<0) || (c[i][j]<0 && polarity[index]>0)){
						pure[index]=false;
					}
				}

			}
		}

		//add pure literals to vector containing their index, to be eliminated by reducer
		for (int i=0;i<pure.size();++i){
			if (pure[i]){
				pureUpdates.push_back(polarity[i]*(i+1));
				//cout<<"found pure literal: "<<to_string(polarity[i]*(i+1))<<endl;
			}
		}
		return(pureUpdates);
	};

	//reducer takes literal value and state info and returns updated state
	Clauses reduce(int lit,Clauses clauses,int depth){
		vector<vector<int>> c  = clauses.sentences;
		clauses.states[depth] = lit;
		//cout<<"lit: "<<lit<<endl;
		for (int j=0;j<c.size();++j){
			for (int k=0;k<c[j].size();++k){

				//if literal apears in clause, clause is true, set to 0
				//zero clauses are eliminated in cleanup step
				//(don't want to eliminate while evaluating because indexing gets difficult)
				if (c[j][k]==lit){ 
					c[j] = {0};
					break;
				}
				//if negative of literal appears in clause, there are two cases
				if (c[j][k]==-lit){

					//case 1: clause has size>1, in which case we just set literal to zero
					// to be eliminated in cleanup
					if (c[j].size()>1){
					c[j][k]=0; 
					} 
					//case 2: clause has size 1, in wich case we have an inconistency. set fail flag
					else{
					clauses.status = "fail";
					return(clauses);
					}
				}
			}
		}

		//cleanup step.
		vector<vector<int>>::iterator i = c.begin();
		while (i!=c.end()){
			//if there are clauses with just a "0" value, 
			//this was set because of successful clause elimination 
			//above. therefore we delete the clause
			if (i->size()==1 && (*i)[0]==0){
				i = c.erase(i);
				//i2 = stringClauses.erase(i2);
			} else{
			//now iterate over each non-empty clause
			vector<int>::iterator j = (*i).begin();
			while(j!=(*i).end()){
				//if literal has value 0, it was set because of 
				//successful literal elimination above. remove literal
				if ((*j)==0){
					j=(*i).erase(j);
				} else{
					j++;
				}
			}
			i++;
			}
			
		}

		//check for success, set success flag if applicable
		clauses.sentences = c;
		if (clauses.sentences.size()==0){
			clauses.status = "success";
		}
		//for debugging
		if (true){
		cout<<"updated clause set: "<<endl;
		for (int i=0; i<c.size();++i){
			cout<<"clause # "<<i<<": ";
			for (int j=0;j<c[i].size();++j){
			cout<<c[i][j]<<" ";
			}
			cout<<endl;
		}
		}
		return(clauses);
	}
};
