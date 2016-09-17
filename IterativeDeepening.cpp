#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Params.h"
using namespace std;

int main(int argc,char* argv[]){
	char* fileName  = argv[1];
	
	Params tree(fileName);
	bool goal = false;
	int depthLevel=1;
	//cout<<"num processes: "<<tree.pCount<<", num tasks: "<<tree.tCount<<", deadline: "<<tree.deadline<<", value minimum: "<<tree.targetVals<<endl;
    while(!goal){
    
    depthLevel++;
    }

    return(0);
}