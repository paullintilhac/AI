#include <stdio.h>
#include <string>
#include <math.h>
#include <stdlib.h>
using namespace std;


class DP{
public:
	vector<vector<int> > clauses;
	vector<vector<int> > simpClauses;
	vector<int> literalSet;
	vector<int> literalValues;
	bool success = false;
	DP(vector<vector<int> > c){
		cout<<"size of clause set in constructor: "<<c.size()<<endl;

		clauses = c;
		simpClauses = c;
		for (int i=0;i<clauses.size();++i){
			cout<<"clauses[i][]: "<<endl;

			for (int j=0;j<clauses[i].size();++j){
				bool newLiteral = true;
				cout<<clauses[i][j]<<" ";
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
	vector<int> run_dp(int depth,vector<vector<int>> sentences,vector<int> states){

		for (int i=0;i<2;++i){
			if (success){
			return(states);
			}
			cout<<"depth: "<<depth<<", i: "<<i<<endl;
			while(true){
				vector<int> literalUpdates = checkForUnits(sentences);
				vector<int> pureUpdates = checkForPureLiterals(sentences);
				cout<<"pureUpdates.size(): "<<pureUpdates.size()<<endl;
				if (literalUpdates.size()==0 && pureUpdates.size()==0){
					break;
				}
				for (int i=0;i<literalUpdates.size();++i){
					states[abs(literalUpdates[i])-1] = literalUpdates[i];
					sentences = reduce(literalUpdates[i],sentences,abs(literalUpdates[i])-1);
					cout<<"updated literals after eliminating singletons: "<<endl;
					for (int i=0;i<literalSet.size();++i){
						cout<<states[i]<<" ";
					}
					cout<<endl;
				}
				for (int i=0;i<pureUpdates.size();++i){
					states[abs(pureUpdates[i])-1] = pureUpdates[i];
					sentences = reduce(pureUpdates[i],sentences,abs(pureUpdates[i])-1);
					cout<<"updated literals after eliminating pure literals: "<<endl;
					for (int i=0;i<literalSet.size();++i){
						cout<<states[i]<<" ";
					}
					cout<<endl;
				}
				if (sentences.size()==0){
				success = true;
				cout<<"success, returning reslt"<<endl;
				return(states);
				}
			}
			
			

			if (i==0){
			states[depth] =literalSet[depth];
			sentences = reduce(literalSet[depth],sentences,depth);
			
			} else{
			states[depth] =-literalSet[depth];
			sentences = reduce(-literalSet[depth],sentences,depth);
			}
			
			cout<<"updated literals: "<<endl;
			for (int i=0;i<literalSet.size();++i){
				cout<<states[i]<<" ";
			}
			cout<<endl;
			int nextOpenDepth = -1;
			for (int i=depth+1;i<literalSet.size();++i){
				if (states[i]==0){
					nextOpenDepth = i;
					break;
				}
				
			}
			cout<<"simplClauses.size(): "<<sentences.size()<<endl;
			if (sentences.size()==0){
				success = true;
				cout<<"success, returning reslt"<<endl;
				return(states);
			}
			if (nextOpenDepth<literalSet.size()&&success==false)
			run_dp(nextOpenDepth, sentences,states);
			
		}
		vector<int> fail = {-1};
		return(fail);
	};

	vector<int> checkForUnits(vector<vector<int>> c){
		//cout<<"checking for literals "<<endl;
		vector<int> literalUpdates;
		for (int i=0;i<c.size();++i){
			if (c[i].size()==1 && c[i][0]!=0){
				//cout<<"found pure literal: "<<c[i][0]<<endl;
				literalUpdates.push_back(c[i][0]);
			}
		}
		
		return literalUpdates;
	};

	vector<int> checkForPureLiterals(vector<vector<int>> c){
		vector<int> polarity;
		vector<bool> pure;
		vector<int> pureUpdates;

		for (int i=0;i<literalSet.size();++i){
			//cout<<"i: "<<i<<endl;
			polarity.push_back(0);
			pure.push_back(false);
		} 


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
		for (int i=0;i<pure.size();++i){
			if (pure[i]){
				pureUpdates.push_back(polarity[i]*(i+1));
			}
		}
		return(pureUpdates);
	};

	vector<vector<int>> reduce(int lit,vector<vector<int>> c,int depth){
		//cout<<"lit: "<<lit<<endl;
		for (int j=0;j<c.size();++j){
			//cout<<"c[j].size(): "<<c[j].size()<<endl;
			for (int k=0;k<c[j].size();++k){
				//cout<<"j: "<<j<<", k: "<<k<<", c[j][k]: "<<c[j][k]<<", lit: "<<lit<<endl;

				if (c[j][k]==lit){ //if literal apears in clause, clause is true, set to 0
					//cout<<"eliminating clause..."<<endl;
					c[j] = {0};
				}
				if (c[j][k]==-lit){
					if (c[j].size()>1){
					//cout<<"eliminating literal: "<<c[j][k]<<endl;
					c[j].erase(c[j].begin()+k); //0 element means true
					} else{
					//cout<<"eliminating clause: "<<j<<endl;
					c[j] = {0};
					}
				}
			}
		}
		for (int i=0;i<c.size();++i){
			if (c[i].size() ==1 && c[i][0]==0){
				c.erase(c.begin()+i);
			}
			
		}
		

		//cout<<"updated clause set: "<<endl;
		//for (int i=0; i<c.size();++i){
		//	for (int j=0;j<c[i].size();++j){
		//		cout<<c[i][j]<<" ";
		//	}
		//				cout<<endl;
		//}

		return(c);
	}
};