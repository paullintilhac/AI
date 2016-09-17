#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Params.h"
using namespace std;

int DFS(Params p,int level, int branch){
	visited[level][i] = true;
	for (int i=0;i<2;++i){
		if (level !=4 && !visited[level+1][i]){
			DFS(p,level+1,i);
		}
	}
}

int main(int argc,char* argv[]){
	char* fileName  = argv[1];
	//bool visited[4][2];

	Params tree(fileName);
	
	bool goal = false;
	int depthLevel=1;
	//cout<<"num processes: "<<tree.pCount<<", num tasks: "<<tree.tCount<<", deadline: "<<tree.deadline<<", value minimum: "<<tree.targetVals<<endl;


    return(0);
}