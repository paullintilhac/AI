#ifndef REVIEWER_H
#define REVIEWER_H
using namespace std;
class Reviewer{
public:
	double cost,RT,RF;
	int state; //0=not included, 1=approved,2=rejected
	Reviewer(double c, double rt,double rf): cost(c),RT(rt),RF(rf){
		state =0;
	};


};
#endif