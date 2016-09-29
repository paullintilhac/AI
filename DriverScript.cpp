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
#include "Params.h"
#include "SearchAlgorithm.h"

using namespace std;


int main(int argc,char* argv[]){

	char* fileName  = argv[2];
	string algo = string(argv[1]);

	Params* p= new Params(fileName);
	SearchAlgorithm* al;
	if (algo =="-h"){
		al = new HC(p);
	} else if(algo == "-i"){
		al = new ID(p);
	} else{
		return(1);
	}
	string returnString=al->doSearch();
	cout<<returnString<<endl;

    return(0);
}