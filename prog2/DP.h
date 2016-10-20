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

		clauses = c;
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
	string run_dp(int depth,vector<vector<int>> sentences,vector<int> states){

			for (int i=0;i<2;++i){
			if (success){
			string returnString="";
			for (int i=0;i<states.size();++i){
				string TF = "";
				if (states[i]>0){
					TF = "T";
				} else{
					TF = "F";
				}
				returnString +=to_string(i+1)+" "+TF+"\n";
			}
			returnString +="0";
			return(returnString);
			}
			cout<<"depth: "<<depth<<", i: "<<i<<endl;
			while(true){
				vector<int> literalUpdates = checkForUnits(sentences);
				vector<int> pureUpdates = checkForPureLiterals(sentences);
				if (literalUpdates.size()==0 && pureUpdates.size()==0){
					break;
				}
				for (int i=0;i<literalUpdates.size();++i){
					states[abs(literalUpdates[i])-1] = literalUpdates[i];
					sentences = reduce(literalUpdates[i],sentences,abs(literalUpdates[i])-1);

					//cout<<"sentences[0][0]: "<<sentences[0][0]<<endl;
					if (sentences[0][0]==0){
						return("0");
					}
					if (sentences.size()==0){
						cout<<"sentence[0] size: "<<sentences[0].size()<<endl;
						success = true;
						cout<<"success, returning reslt"<<endl;
						string returnString="";
						for (int i=0;i<states.size();++i){
						string TF = "";
						if (states[i]>0){
							TF = "T";
						} else{
							TF = "F";
						}
						returnString +=to_string(i+1)+" "+TF+"\n";
						}
						returnString +="0";
						return(returnString);
					}
					if (true){
					cout<<"updated literals after eliminating singletons: "<<endl;
					for (int i=0;i<literalSet.size();++i){
						cout<<states[i]<<" ";
					}
					cout<<endl;
					}
				}
				for (int i=0;i<pureUpdates.size();++i){
					states[abs(pureUpdates[i])-1] = pureUpdates[i];
					sentences = reduce(pureUpdates[i],sentences,abs(pureUpdates[i])-1);
					if (sentences[0][0]==0){
						return("0\n");
					}
					if (sentences.size()==0){
						cout<<"sentence[0] size: "<<sentences[0].size()<<endl;
						success = true;
						cout<<"success, returning reslt"<<endl;
						string returnString="";
						for (int i=0;i<states.size();++i){
						string TF = "";
						if (states[i]>0){
							TF = "T";
						} else{
							TF = "F";
						}
						returnString +=to_string(i+1)+" "+TF+"\n";
						}
						returnString +="0";
						return(returnString);
					}
					if (true){
					cout<<"updated literals after eliminating pure literals: "<<endl;
					for (int i=0;i<literalSet.size();++i){
						cout<<states[i]<<" ";
					}
					cout<<endl;
					}
				}
				
			}
			
			
			cout<<"litralSet[depth]: "<<literalSet[depth]<<endl;
			if (i==0){
			states[depth] =literalSet[depth];
			sentences = reduce(literalSet[depth],sentences,depth);
			if (sentences[0][0]==0){
						return("0\n");
			}
			} else{
			states[depth] =-literalSet[depth];
			sentences = reduce(-literalSet[depth],sentences,depth);
			if (sentences[0][0]==0){
					return("0\n");
			}

			}
			if (true){
			cout<<"updated literals: "<<endl;
			for (int i=0;i<literalSet.size();++i){
				cout<<states[i]<<" ";
			}
			}
			cout<<endl;
			int nextOpenDepth = -1;
			for (int i=depth+1;i<literalSet.size();++i){
				if (states[i]==0){
					nextOpenDepth = i;
					break;
				}
				
			}
			if (sentences.size()==0){
				//cout<<"sentence[0] size: "<<sentences[0].size()<<endl;
				if ((sentences[0].size())>0){
				success = true;
				cout<<"success, returning reslt"<<endl;
				string returnString="";
				for (int i=0;i<states.size();++i){
					string TF = "";
					if (states[i]>0){
						TF = "T";
					} else{
						TF = "F";
					}
					returnString +=to_string(i+1)+" "+TF+"\n";
				}
				returnString +="0\n";
				return(returnString);
			}else{
				cout<<"FAILED"<<endl;
				return("");
			}
			}
			if (nextOpenDepth<literalSet.size()&&success==false)
			run_dp(nextOpenDepth, sentences,states);
			
		}
		
		return("0\n");
	};

	vector<int> checkForUnits(vector<vector<int>> c){
		//cout<<"checking for units "<<endl;
		vector<int> literalUpdates;
		for (int i=0;i<c.size();++i){
			if (c[i].size()==1 && c[i][0]!=0){
				cout<<"found unit "<<c[i][0]<<endl;
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
		cout<<"lit: "<<lit<<endl;
		for (int j=0;j<c.size();++j){
			//cout<<"c[j].size(): "<<c[j].size()<<endl;
			for (int k=0;k<c[j].size();++k){
				//cout<<"j: "<<j<<", k: "<<k<<", c[j][k]: "<<c[j][k]<<", lit: "<<lit<<endl;

				if (c[j][k]==lit){ //if literal apears in clause, clause is true, set to 0
					cout<<"eliminating clause in success..."<<endl;
					c[j] = {0};
					break;
				}
				if (c[j][k]==-lit){
					if (c[j].size()>1){
					//cout<<"eliminating literal: "<<c[j][k]<<endl;
					c[j][k]=0; //0 element means true
					} else{

					//cout<<"eliminating clause in failure: "<<j<<endl;

					return{{0}};
					}
				}
			}
		}
		vector<vector<int>>::iterator i = c.begin();
		while (i!=c.end()){
			if (i->size()==1 && (*i)[0]==0){
				i = c.erase(i);
			} else{
			vector<int>::iterator j = (*i).begin();
			while(j!=(*i).end()){
				if ((*j)==0){
					j=(*i).erase(j);
				} else{
					j++;
				}
			}
			i++;
			}
			
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
		return(c);
	}
};