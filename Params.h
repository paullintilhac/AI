#ifndef PARAMS_H
#define PARAMS_H


using namespace std;

class Params{
public:
	vector<float> lengths,speeds;
	int deadline,targetVals,tCount,pCount;
	//static bool visited[4][2];
	static int active[3];

	Params(char* fileName){
	
	
	ifstream infile (fileName	);
	string str;
	int id = 0;
	getline(infile, str);
    
    tCount = 0;
    pCount=0;
    istringstream iss(str);
    string s;
    while ( iss >> s){
    tCount++;
    lengths.push_back(atof(s.c_str()));
	}
	getline(infile, str);
    istringstream iss2(str);
    while ( iss2 >> s){
    pCount++;
    speeds.push_back(atof(s.c_str()));
	}
	
	getline(infile, str);
	istringstream iss3(str);
	float deadline,targetVals;
	iss3>>deadline;
	this->deadline = deadline;
	iss3>>targetVals;
	this->targetVals = targetVals;

	}
};

//bool Params::visited[4][2] = {{0,0},{0,0},{0,0},{0,0}};
int Params::active[3] = {0,0,0};

#endif