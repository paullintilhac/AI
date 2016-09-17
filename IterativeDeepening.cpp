#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Params.h"
using namespace std;

//need to generalize to arbitrary number of tasks and processes

int DFS(Params p,int level, int branch, int depthLevel){
    int totalTime = 0;
    int targetVals = 0;
    for (int i=0;i<sizeof(p.active)/sizeof(p.active[0]);++i){
        cout<<"p.lengths[i]: "<<p.lengths[i]<<endl;
        if (p.active[i]==1){
            totalTime += p.lengths[i]/p.speeds[0];
            targetVals += p.lengths[i];
        }
        if (p.active[i]==2){
            totalTime += p.lengths[i]/p.speeds[1];
            targetVals += p.lengths[i];
        }
    }
    cout<<"total time: "<<totalTime<<", targetVals: "<<targetVals<<endl;
    if (totalTime<p.deadline && targetVals > p.targetVals){
        cout<<"reached goal, returning"<<endl;
        return(1);
    }
	for (int i=0;i<2;++i){
        p.active[level]= i+1;
		if (level !=depthLevel){
            DFS(p,level+1,i,depthLevel);
            p.active[level+1] = 0;
		}
	}    
    return(0);
}

int main(int argc,char* argv[]){
	char* fileName  = argv[1];
	//bool visited[4][2];
    static int totalTime, targetVals;
	Params tree(fileName);
	bool goal = false;
	int depthLevel=1;
    int success = 0;
    int depth = 0;
    while(!success){
        cout<<"depth: "<<depth<<endl;
        success = DFS(tree,0,0,depth++);
        if (depth >4){
            break;
        }
    }
    for (int i=0;i<3;i++){
        cout<<tree.active[i]<<endl;
    }
	//cout<<"num processes: "<<tree.pCount<<", num tasks: "<<tree.tCount<<", deadline: "<<tree.deadline<<", value minimum: "<<tree.targetVals<<endl;


    return(0);
}