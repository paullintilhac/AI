
class FrontEnd{
public:
	vector<Atom> atoms;
	FrontEnd(char* fileName){
	ifstream infile2(fileName);
	string str2;
	getline(infile2, str2);
	stringstream ss1(str2);
	string s;
	vector<string> nodeList;
	while (ss1 >> s)
	    {
	    nodeList.push_back(s);
	        if (ss1.peek() == ',')
	            ss1.ignore();
	}
	getline(infile2, str2);
	stringstream ss2(str2);
	
	vector<string> treasureList;
	while (ss2 >> s)
	    {
	    treasureList.push_back(s);
	}
	getline(infile2, str2);
	int maxSteps = atoi(str2.c_str());
	vector<vector<int>> edges; //[currentNode][adjacentNode]
	vector<vector<int>> treasures;
	vector<vector<int>> tolls;
	vector<vector<Atom>> atAtoms; //[time][node]
	vector<vector<Atom>> hasAtoms; //[time][treasure]
	vector<vector<Atom>> availAtoms; //[time][treasure]

	int count = 0;


	while(getline(infile2,str2)){
		stringstream ss3(str2);
		string node;
		ss3>>node;
		string i;
		vector<int> adjacentNodes;
		vector<int> nodeTreasures;
		vector<int> nodeTolls;
		ss3>>i;

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
	for (int i=0;i<edges.size();++i){
		for (int j=0;j<edges[i].size();++j){
			cout<<nodeList[edges[i][j]]<<" ";
		}
		cout<<endl;
	}

	//create set of "At" atoms for each node and time
	for (int i=0;i<=maxSteps;++i){
		vector<Atom> nodes;
		for (int j=0;j<nodeList.size();++j){
			string atomString = "At("+nodeList[j]+","+to_string(i)+")";
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
			string atomString = "Has("+treasureList[j]+","+to_string(i)+")";
			Atom thisAtom(i,treasureList[j],"","Has",atomString,++count);
			hasTreasures.push_back(thisAtom);
			atoms.push_back(thisAtom);
			atomString = "Available("+treasureList[j]+","+to_string(i)+")";
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
		for (int j=0;j<atAtoms[i].size()-1;++j){

			//iterate over all OTHER nodes
			for (int k=j+1;k<atAtoms[i].size()-1;++k){
				//category 1
				clauses+="-"+to_string(atAtoms[i][j].index)+" -"+to_string(atAtoms[i][k].index)+"\n";
			}

			//for propositions involving T-1 steps, cause they relate t to t+1 or t-1
			if (i<maxSteps){

				//category 3
				if (edges[j].size()>0){
				clauses+="-"+to_string(atAtoms[i][j].index);
				//iterate over all adjacent nodes
				for (int k=0;k<edges[j].size();++k){
					clauses+=" "+to_string(atAtoms[i+1][edges[j][k]].index);
				}
				clauses+="\n";
				}

				//category 4
				if (tolls[j].size()>0){
				clauses+="-"+to_string(atAtoms[i+1][j].index);
				//iterate over all tolls associated with this node
				for (int k=0;k<tolls[j].size();++k){
					clauses+=" "+to_string(hasAtoms[i][tolls[j][k]].index);
				}
				clauses+="\n";
				}

				//category 5
				//iterate over all treasures associated with this node
				for (int k=0;k<treasures[j].size();++k){
				clauses+="-"+to_string(availAtoms[i][treasures[j][k]].index)+" -"+to_string(atAtoms[i+1][j].index)+" "+to_string(hasAtoms[i+1][treasures[j][k]].index)+"\n";
				}

				//category 7
				//iterate over all treasures associated with this node
				for (int k=0;k<treasures[j].size();++k){
					//for each treasure, iterate over all other nodes
					for (int l=0;l<atAtoms[i].size();++l){
					if (j!=l){
						clauses+="-"+to_string(availAtoms[i][treasures[j][k]].index)+" -"+to_string(atAtoms[i+1][l].index)+" "+to_string(availAtoms[i+1][treasures[j][k]].index)+"\n";
					}
				}
				}

				//category 10
				//iterate over each toll associated with this node
				for (int k=0;k<tolls[j].size();++k){
					//iterate over all other nodes for this node and treasure
					for (int l=0;l<atAtoms[i].size();++l){
					if (j!=l){
						clauses+="-"+to_string(hasAtoms[i][tolls[j][k]].index)+" -"+to_string(atAtoms[i+1][l].index)+" "+to_string(hasAtoms[i+1][tolls[j][k]].index)+"\n";

					}
					}
				}

			}

			//no more restiction for time to be <Tmax
			//category 6
			if (tolls[j].size()>0){
			clauses+="-"+to_string(atAtoms[i][j].index);
			//iterate over all tolls associated with this node
			for (int k=0;k<tolls[j].size();++k){
				clauses+=" -"+to_string(hasAtoms[i][tolls[j][k]].index);
			}
			clauses+="\n";
			}
		}
		//done iterating over nodes

		//category 2
		for (int j=0;j<hasAtoms[i].size();++j){
			clauses+="-"+to_string(hasAtoms[i][j].index)+" -"+to_string(availAtoms[i][j].index)+"\n";
		}
		if (i<maxSteps){
			//category 8
			for (int j=0;j<availAtoms[i].size();++j){
				clauses+=to_string(availAtoms[i][j].index)+" -"+to_string(availAtoms[i+1][j].index)+"\n";
			}
			//category 9
			for (int j=0;j<availAtoms[i].size();++j){
				clauses+=to_string(availAtoms[i][j].index)+" "+to_string(hasAtoms[i][j].index)+" -"+to_string(hasAtoms[i+1][j].index)+"\n";
			}
		}

	}

	clauses+=to_string(atAtoms[0][0].index)+"\n";
	for (int i=0;i<availAtoms[0].size();++i){
		clauses+=to_string(availAtoms[0][i].index)+"\n";
	}
	clauses+=to_string(atAtoms[maxSteps][atAtoms[0].size()-1].index)+"\n";

	clauses+="0\n";
	cout<<"clauses: "<<clauses<<endl;

	ofstream os("clauses");
	if (! os) { std::cerr<<"Error writing to ..."<< endl; } else {
	os << clauses;
	}
	
}
};