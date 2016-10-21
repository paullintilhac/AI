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
	vector<vector<string>> stringClauses;
	vector<int> literalSet;
	vector<int> literalValues;
	DP(vector<vector<int> > c, vector<vector<string>> stringClauses){ //stringClauses is only for debugging purposes
		clauses  = c;
		simpClauses = c;
		this->stringClauses = stringClauses;
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
						cout<<"no solution after literal"<<endl;
						scan=false;
						clauses.returnString = "0\n";

					return(clauses);
					}
					if (clauses.status == "success"){
						cout<<"returning with success"<<endl;
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
						cout<<"no solution after pure"<<endl;
						scan = false;
						clauses.returnString = "0\n";
						return(clauses);
					}
					if (clauses.status == "success"){
						cout<<"returning with success"<<endl;
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
			cout<<"DEPTH: "<<depth<<", i: "<<i<<", status: "<<clauses.status<<endl;	
			
			if (true){
			cout<<"updated literals: "<<endl;
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

			if (nextOpenDepth<literalSet.size()){
			Clauses childClauses =run_dp(nextOpenDepth, clauses,i);
				if (childClauses.status=="success"){

				clauses = childClauses;
				}
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
				//cout<<"found pure literal: "<<to_string(polarity[i]*(i+1))<<endl;
			}
		}
		return(pureUpdates);
	};

	Clauses reduce(int lit,Clauses clauses,int depth){
		vector<vector<int>> c  = clauses.sentences;
		clauses.states[depth] = lit;
		cout<<"lit: "<<lit<<endl;
		for (int j=0;j<c.size();++j){
			//cout<<"c[j].size(): "<<c[j].size()<<endl;
			for (int k=0;k<c[j].size();++k){
				//cout<<"j: "<<j<<", k: "<<k<<", c[j][k]: "<<c[j][k]<<", lit: "<<lit<<endl;

				if (c[j][k]==lit){ //if literal apears in clause, clause is true, set to 0
					cout<<"eliminating clause with success..."<<endl;
					c[j] = {0};
					break;
				}
				if (c[j][k]==-lit){
					if (c[j].size()>1){
					cout<<"eliminating literal: "<<c[j][k]<<endl;
					c[j][k]=0; 
					} else{

					cout<<"eliminating clause with failure: "<<j<<endl;
					 //return empty vector

					clauses.status = "fail";
					return(clauses);
					}
				}
			}
		}

		vector<vector<int>>::iterator i = c.begin();
		//vector<vector<string>>::iterator i2 = stringClauses.begin();
		while (i!=c.end()){
			if (i->size()==1 && (*i)[0]==0){
				i = c.erase(i);
				//i2 = stringClauses.erase(i2);
			} else{
			vector<int>::iterator j = (*i).begin();
			//vector<string>::iterator j2 = (*i2).begin();
			while(j!=(*i).end()){
				if ((*j)==0){
					j=(*i).erase(j);
					//j2=(*i2).erase(j2);
				} else{
					j++;
					//j2++;
				}
			}
			i++;
			//i2++;
			}
			
		}
		clauses.sentences = c;
		if (clauses.sentences.size()==0){
			clauses.status = "success";
		}

		if (false){
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
