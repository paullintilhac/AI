#ifdef ID_H
#define ID_H

class ID : public SearchAlgorithm {
public:
	ID(Params* p) : SearchAlgorithm(p){}
	string DFS(Params* p,int level, int branch,int maxLevel, int* stateVector);
	string doSearch();


};

#endif