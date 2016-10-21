#ifndef UTIL_H
#define UTIL_H

#include <string>
using namespace std;

class Atom{
public:
	string type;
	string treasure;
	string node;
	string name;

	int time;
	int index;
	Atom(int t,string treas,string n,string typ,string nam, int ind){
		time = t;
		type = typ;
		name = nam;
		index = ind;
		if (type == "Available"){
			treasure = treas;
			node="";
		}
		if (type =="Has"){
			treasure = treas;
			node="";
		}
		if (type == "At"){
			treasure="";
			node=n;
		}
	}	
};

#endif