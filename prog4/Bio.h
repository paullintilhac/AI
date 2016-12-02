#ifndef BIO_H
#define BIO_H
#include <string>
#include <vector>
using namespace std;
class Bio{
public:
	vector<string> words;
	string category,name;
	int index;
	Bio(vector<string> w, int i, string c,string n): words(w),index(i),category(c),name(n){};
};

#endif