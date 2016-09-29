#ifndef PARAMS_H
#define PARAMS_H


using namespace std;

class Params{
public:
	vector<float> lengths,speeds;
	int deadline,targetVals,tCount,pCount;

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
    //cout<<"saving speeds"<<endl;
    while ( iss2 >> s){
   	//cout<<atof(s.c_str())<<endl;
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

#endif