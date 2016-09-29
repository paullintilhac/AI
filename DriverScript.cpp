#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "SearchAlgorithm.h"
#include "Params.h"

using namespace std;


int main(int argc,char* argv[]){

	char* fileName  = argv[2];
	string algoString = string(argv[1]);

	string algo = algoString.substr(0,2);
	string rr = algoString.substr(2,algoString.length()-2);
	if (rr.length()<1&& algo =="-h"){
		cout<<"need to specify # of restarts for hill-climbing, e.g. -h10"<<endl;
		exit(1);
	}
	int r = atof(rr.c_str());
	Params* p= new Params(fileName);
	SearchAlgorithm* al;
	if (algo =="-h"){
		al = new HC(p,r);
	} else if(algo == "-i"){
		al = new ID(p);
	} else{
		return(1);
	}
	string returnString=al->doSearch();
	cout<<returnString<<endl;

    return(0);
}