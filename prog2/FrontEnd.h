#include <stdlib.h>
class FrontEnd{
public:
	vector<Atom> atoms;
	FrontEnd(char* fileName){
	ifstream infile2(fileName);
	string str2;
	//read first line and create nodeList
	getline(infile2, str2);
	stringstream ss1(str2);
	string s;
	vector<string> nodeList;
	while (ss1 >> s)
	    {
	    nodeList.push_back(s);
	       
	}

	//read second line and get treasurelist
	getline(infile2, str2);
	stringstream ss2(str2);
	
	vector<string> treasureList;
	while (ss2 >> s)
	    {
	    treasureList.push_back(s);
	}

	//read third line to get maxSteps
	getline(infile2, str2);
	int maxSteps = atoi(str2.c_str());
	vector<vector<int> > edges; //[currentNode][adjacentNode]
	vector<vector<int> > treasures;
	vector<vector<int> > tolls;
	vector<vector<Atom> > atAtoms; //[time][node]
	vector<vector<Atom> > hasAtoms; //[time][treasure]
	vector<vector<Atom> > availAtoms; //[time][treasure]

	int count = 0;
	//read remaining lines to get node incidence information
	while(getline(infile2,str2)){
		stringstream ss3(str2);
		string node;
		ss3>>node;
		string i;
		vector<int> adjacentNodes;
		vector<int> nodeTreasures;
		vector<int> nodeTolls;
		ss3>>i;
		//create 2D array with treasures at each node
		while (ss3 >> i)
	    {
	    if (i=="TOLLS"){
	    	break;
	    }
	    int ind = -1;
	    for (int j=0;j<treasureList.size();++j){
	    	if (i==treasureList[j]){
	    		ind = j;
	    	}
	    }
	    nodeTreasures.push_back(ind);
		}
		treasures.push_back(nodeTreasures);
		
		//create 2D array with tolls at each node
		while (ss3 >> i)
	    {
	    if (i=="NEXT"){
	    	break;
	    }
	    int ind = -1;
	    for (int j=0;j<treasureList.size();++j){
	    	if (i==treasureList[j]){
	    		ind = j;
	    	}
	    }
	    nodeTolls.push_back(ind);
		}
		tolls.push_back(nodeTolls);

		//create 2D list with incident nodes at each node
		while (ss3 >> i)
	    {
	    int ind = -1;
	    for (int j=0;j<nodeList.size();++j){
	    	if (i==nodeList[j]){
	    		ind = j;
	    	}
	    }
	    adjacentNodes.push_back(ind);
	        
		}
		edges.push_back(adjacentNodes);
	}

	//create set of "At" atoms for each node and time
	for (int i=0;i<=maxSteps;++i){
		vector<Atom> nodes;
		for (int j=0;j<nodeList.size();++j){
			char temp[20];
			sprintf(temp,"%s",i);
			string atomString = "At("+nodeList[j]+","+temp+")";
			Atom thisAtom(i,"",nodeList[j],"At",atomString,++count);
			nodes.push_back(thisAtom);
			atoms.push_back(thisAtom);
			//cout<<thisAtom.name<<endl;
		}
		atAtoms.push_back(nodes);
	}

	//create set of "Has" and "available" atoms for each treasure and time
	for (int i=0;i<=maxSteps;++i){
		vector<Atom> hasTreasures;
		vector<Atom> availTreasures;
		for (int j=0;j<treasureList.size();++j){
			char temp[100];
			sprintf(temp,"%s",i);
			string atomString = "Has("+treasureList[j]+","+temp+")";
			Atom thisAtom(i,treasureList[j],"","Has",atomString,++count);
			hasTreasures.push_back(thisAtom);
			atoms.push_back(thisAtom);
			sprintf(temp,"%s",i);
			atomString = "Available("+treasureList[j]+","+temp+")";
			Atom thisAtom2(i,treasureList[j],"","Available",atomString,++count);
			availTreasures.push_back(thisAtom2);
			atoms.push_back(thisAtom2);
		}
		hasAtoms.push_back(hasTreasures);
		availAtoms.push_back(availTreasures);
	}


	string clauses;
	//iterate over each time step-- all propositions iterate over time
	for (int i=0;i<=maxSteps;++i){
		//iterate over al propositions that require iteration over nodes.
		for (int j=0;j<atAtoms[i].size();++j){
			//iterate over all OTHER nodes
			for (int k=j+1;k<atAtoms[i].size();++k){
				//category 1
				char temp1[100],temp2[100];
				sprintf(temp1,"%s",atAtoms[i][j].index);
				sprintf(temp2,"%s",atAtoms[i][j].index);
				clauses+="-"+string(temp1)+" -"+string(temp2)+"\n";
			}

			//for propositions involving T-1 steps, cause they relate t to t+1 or t-1
			if (i<maxSteps){

				//category 3
				if (edges[j].size()>0){
				char temp[100];
				sprintf(temp,"%s",atAtoms[i][j].index);
				clauses+="-"+string(temp);
				//iterate over all adjacent nodes
				for (int k=0;k<edges[j].size();++k){
				char temp2[100];
				sprintf(temp2,"%s",atAtoms[i+1][edges[j][k]].index);
					clauses+=" "+string(temp2);
				}
				clauses+="\n";
				}

				//category 4
				if (tolls[j].size()>0){
				char temp[100];
				sprintf(temp,"%s",atAtoms[i+1][j].index);
				clauses+="-"+string(temp);
				//iterate over all tolls associated with this node
				for (int k=0;k<tolls[j].size();++k){
				char temp2[100];
				sprintf(temp2,"%s",hasAtoms[i][tolls[j][k]].index);
					clauses+=" "+string(temp2);
				}
				clauses+="\n";
				}

				//category 5
				//iterate over all treasures associated with this node
				for (int k=0;k<treasures[j].size();++k){
				char temp2[100],temp3[100],temp4[100];
				sprintf(temp2,"%s",availAtoms[i][treasures[j][k]].index);
				sprintf(temp3,"%s",atAtoms[i+1][j].index);
				sprintf(temp4,"%s",hasAtoms[i+1][treasures[j][k]].index);
				clauses+="-"+string(temp2)+" -"+string(temp3)+" "+string(temp4)+"\n";
				}

				//category 7
				//iterate over all treasures
				for (int k=0;k<treasureList.size();++k){
					//iterate over all other nodes
					for (int l=0;l<atAtoms[i].size();++l){
						//check if other node is home to this treasre
						bool isAHome = false;
						for (int m=0;m<treasures[l].size();++m){
							if (k==treasures[l][m])
								isAHome=true;
						}
					if (!isAHome){
						char temp1[100],temp2[100],temp3[100];
						sprintf(temp1,"%s",availAtoms[i][k].index);
						sprintf(temp2,"%s",atAtoms[i+1][l].index);
						sprintf(temp3,"%s",availAtoms[i+1][k].index);
						clauses+="-"+string(temp1)+" -"+string(temp2)+" "+string(temp3)+"\n";
					}

				}
				}


				//category 10
				//iterate over each toll associated with this node
				for (int k=0;k<treasureList.size();++k){

					//iterate over all other nodes for this node and treasure
					for (int l=0;l<atAtoms[i].size();++l){
						//check if other node is a toll for this treasure
						bool isAToll=false;
						for (int m=0;m<tolls[l].size();++m){
							if (k==tolls[l][m])
								isAToll=true;
						}
					if (!isAToll){
						char temp1[100], temp2[100],temp3[100];
						sprintf(temp1,"%s",hasAtoms[i][k].index);
						sprintf(temp2,"%s",atAtoms[i+1][l].index);
						sprintf(temp3,"%s",hasAtoms[i+1][k].index);
						clauses+="-"+string(temp1)+" -"+string(temp2)+" "+string(temp3)+"\n";
					}
					}
				}
			}

			//no more restiction for time to be <Tmax
			//category 6
			if (tolls[j].size()>0){
			char temp1[100];
			sprintf(temp1,"%s",atAtoms[i][j].index);
			clauses+="-"+string(temp1);
			//iterate over all tolls associated with this node
			for (int k=0;k<tolls[j].size();++k){
				char temp[100];
				sprintf(temp,"%s",hasAtoms[i][tolls[j][k]].index);
				clauses+=" -"+string(temp);
			}
			clauses+="\n";
			}
		}
		//done iterating over nodes

		//category 2
		for (int j=0;j<hasAtoms[i].size();++j){
			char temp1[100], temp2[100];
			sprintf(temp1,"%s",hasAtoms[i][j].index);
			sprintf(temp2,"%s",availAtoms[i][j].index);
			clauses+="-"+string(temp1)+" -"+string(temp2)+"\n";
		}
		if (i<maxSteps){
			//category 8
			for (int j=0;j<availAtoms[i].size();++j){
				char temp1[100],temp2[100],temp3[100];
				sprintf(temp1,"%s",availAtoms[i][j].index);
				sprintf(temp2,"%s",availAtoms[i+1][j].index);
				clauses+=string(temp1)+" -"+string(temp2)+"\n";
			}
			//category 9
			for (int j=0;j<availAtoms[i].size();++j){
				char temp1[100], temp2[100],temp3[100];
				sprintf(temp1,"%s",availAtoms[i][j].index);
				sprintf(temp2,"%s",hasAtoms[i][j].index);
				sprintf(temp3,"%s",hasAtoms[i+1][j].index);
				clauses+=string(temp1)+" "+string(temp2)+" -"+string(temp3)+"\n";
			}
		}

	}
	char temp[100];
	sprintf(temp,"%s",hasAtoms[0][0].index);
	clauses+=string(temp)+"\n";
	for (int i=0;i<availAtoms[0].size();++i){
		char temp2[100];
		sprintf(temp2,"%s",availAtoms[0][i].index);
		clauses+=string(temp2)+"\n";
	}
	sprintf(temp,"%s",atAtoms[maxSteps][atAtoms[0].size()-1].index);
	clauses+=string(temp)+"\n";

	clauses+="0\n";
	//cout<<"clauses: "<<clauses<<endl;

	ofstream os("clauses");
	if (! os) { std::cerr<<"Error writing to ..."<< endl; } else {
	os << clauses;
	}
	
}
};
