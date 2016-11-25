#include <stdio.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include "Clauses.h"
#include "Util.h"
using namespace std;


class DP{
public:
	vector<vector<int> > clauses;
	vector<vector<int> > simpClauses;
	vector<int> literalSet;
	vector<int> literalValues;
	vector<Atom> atoms;
	bool DEBUG;
	DP(vector<vector<int> > c,vector<Atom> atoms, bool d){ 
		this->atoms = atoms;
		clauses  = c;
		simpClauses = c;
		DEBUG=false;
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

	Clauses run_dp(int depth,Clauses clauses,int branch){

			//if branch is 0, set literal to positive; if branch is 1, set to negative
			if (branch==0){
			clauses = reduce(literalSet[depth],clauses,depth);
			
			} else{
			clauses = reduce(-literalSet[depth],clauses,depth);
			}
			
			//iterate using pure literal and unit reduction as much as possible
			while(true){
				vector<int> literalUpdates = checkForUnits(clauses.sentences);
				vector<int> pureUpdates = checkForPureLiterals(clauses.sentences);
				//if no more "low hanging fruit", move on
				if (literalUpdates.size()==0 && pureUpdates.size()==0){
					break;
				}
				
				for (int j=0;j<literalUpdates.size();++j){
					clauses = reduce(literalUpdates[j],clauses,abs(literalUpdates[j])-1);

					//check after each reduction whether status has changed
					if (clauses.status == "fail"){
						clauses.returnString = "0\n";

					return(clauses);
					}
					if (clauses.status == "success"){
						return(clauses);
					}
					
				}

				for (int j=0;j<pureUpdates.size();++j){

					clauses = reduce(pureUpdates[j],clauses,abs(pureUpdates[j])-1);
					//check after each reduction whether status has changed
					if (clauses.status == "fail"){
						clauses.returnString = "0\n";
						return(clauses);
					}
					if (clauses.status == "success"){
						return(clauses);
					}
					
				}
			}		
			
			//iterate over child nodes
			for (int i=0;i<2;++i){
			//for debugging
			if (DEBUG){
			cout<<"updated literals: "<<endl;
			for (int j=0;j<literalSet.size();++j){
				cout<<clauses.states[j]<<" ";
			}
			cout<<endl;
			}
			//note that next literal may have already been set by unit or pure literal reduction
			//therefore child node is not at depth+1, but at the next depth with literal set to 0 (unset)
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
		
		return(clauses);
	};

	vector<int> checkForUnits(vector<vector<int> > c){
		vector<int> literalUpdates;
		for (int i=0;i<c.size();++i){
			if (c[i].size()==1 && c[i][0]!=0){
				if (DEBUG){
				if (c[i][0]<0)
					cout<<"found unit: ~"<<atoms[abs(c[i][0])-1].name<<endl;
				if (c[i][0]>0)
					cout<<"found unit: "<<atoms[abs(c[i][0])-1].name<<endl;
				}
				literalUpdates.push_back(c[i][0]);
			}
		}
		return literalUpdates;
	};

	vector<int> checkForPureLiterals(vector<vector<int> > c){
		vector<int> polarity;
		vector<bool> pure;
		vector<int> pureUpdates;
		for (int i=0;i<literalSet.size();++i){
			polarity.push_back(0);
			pure.push_back(false);
		} 

		//check for pure literals -- runs in polynomial (~n^2) time
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
				if (DEBUG){
				if (polarity[i]<0)
					cout<<"found pure literal: ~"<<atoms[i].name<<endl;
				if (polarity[i]>0)
					cout<<"found pure literal: "<<atoms[i].name<<endl;
				}
				}
		}
		return(pureUpdates);
	};

	//reducer takes literal value and state info and returns updated state
	Clauses reduce(int lit,Clauses clauses,int depth){
		vector<vector<int> > c  = clauses.sentences;
		clauses.states[depth] = lit;
		if (DEBUG){
		if (lit<0)
		cout<<"literal: ~"<<atoms[abs(lit)-1].name<<endl;
		if (lit>0)
		cout<<"literal: "<<atoms[abs(lit)-1].name<<endl;
		}
		for (int j=0;j<c.size();++j){
			for (int k=0;k<c[j].size();++k){

				//if literal apears in clause, clause is true, set to 0
				//zero clauses are eliminated in cleanup step
				//(don't want to eliminate while evaluating because indexing gets difficult)
				if (c[j][k]==lit){ 
					if (DEBUG){
					string clauseString="";
					for (int l=0;l<c[j].size();++l){
						//cout<<"c[j][l]: "<<c[j][l]<<endl;
						clauseString+=atoms[abs(c[j][l])-1].name+" ";
					}
					clauseString+="\n";
					cout<<"successfully eliminating clause "<<clauseString<<endl;
					}

					vector<int> temp;
					temp.push_back(0);
					c[j] = temp;

					break;
				}
				//if negative of literal appears in clause, there are two cases
				if (c[j][k]==-lit){

					//case 1: clause has size>1, in which case we just set literal to zero
					// to be eliminated in cleanup
					if (c[j].size()>1){

					if (DEBUG){
					if (lit<0)
						cout<<"eliminating literal: "<<atoms[abs(lit)-1].name<<endl;
					if (lit>0)
						cout<<"eliminating literal: ~"<<atoms[abs(lit)-1].name<<endl;
					}
					c[j][k]=0; 
					} 
					//case 2: clause has size 1, in wich case we have an inconistency. set fail flag
					else{
					if (DEBUG){
					string clauseString="";
					for (int l=0;l<c[j].size();++l){
						clauseString+=atoms[abs(c[j][l])-1].name+" ";
					}
					clauseString+="\n";
					cout<<"inconsistency in clause "<<clauseString<<endl;
					}
					clauses.status = "fail";
					return(clauses);
					}
				}
			}
		}

		//cleanup step.
		vector<vector<int> >::iterator i = c.begin();
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
		if (DEBUG){
		cout<<"updated clause set: "<<endl;
		for (int i=0; i<c.size();++i){
			cout<<"clause # "<<i<<": ";
			for (int j=0;j<c[i].size();++j){
			if (c[i][j]<0)
			cout<<"~"<<atoms[abs(c[i][j])-1].name<<" ";
			if (c[i][j]>0)
			cout<<atoms[abs(c[i][j])-1].name<<" ";
			}
			cout<<endl;
		}
		}
		return(clauses);
	}
};
