#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc,char* argv[]){
	string fileName  = string(argv[1]);
	ifstream infile (fileName.c_str());
	string str;
	int id = 0;
	while (getline(infile, str)) {
    vector<string> result;
    istringstream iss(str);
        cout<<iss<<endl;

    for(string s; iss >> s; )
    result.push_back(s);
	cout<<iss<<endl;
    }
    return(0);
}