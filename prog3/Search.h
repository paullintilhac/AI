#ifndef SEARCH_H
#define SEARCH_H

#include "Reviewer.h"

using namespace std;

class Search{
	public: 
	vector<int> states;
	vector<Reviewer> reviewers;
	double p,S,F;
	int startDepth;
	double bestMove,maxUtil;
	Search(vector<int> s,vector<Reviewer> r,double p,double S, double F,int sd):states(s),startDepth(sd),reviewers(r),p(p),S(S),F(F){
		maxUtil=run_dfs(states,startDepth);
	};
	double run_dfs(vector<int> s, int depth){

		//branch = 0: publish. Branch=1: reject, branch=i (i>2): consult reviewer i-2
		double utility;
		
		double maxUtil =-10000000;
		if (depth<reviewers.size()){
		for (int i=1;i<reviewers.size()+1;i++){
			bool taken= false;
			for (int j=0;j<depth+1;j++){
				//cout<<"i: "<<i<<", s[j]: "<<s[j]<<endl;
				if (abs(s[j])==i){
					taken = true;
				}
			}
			double branch_prob;
		
			double num1 = 1;
			double num2 =1;
			if (depth>0){
				for (int j=0;j<depth;j++){
					if (s[j]>0){
						num1*=reviewers[abs(s[j])-1].RT;
						num2*=reviewers[abs(s[j])-1].RF;
					}
					if (s[j]<0){
						num1*=(1-reviewers[abs(s[j])-1].RT);
						num2*=(1-reviewers[abs(s[j])-1].RF);
					}
				}
			}
		//cout<<"depth: "<<depth<<endl;
		double top = reviewers[i-1].RT*num1*p+reviewers[i-1].RF*num2*(1-p);
		double bottom = (p*num1+(1-p)*num2);

		branch_prob =top/bottom;
			if (taken){
				//cout<<"already taken"<<endl;
				continue;
			}
			s[depth]=i;
			//cout<<"i: "<<i<<", depth: "<<depth<<", state[0]: "<<s[0]<<", s[1]: "<<s[1]<<endl;

			double utility1=(run_dfs(s,depth+1));
			s[depth]=-i;
			double utility2 = (run_dfs(s,depth+1));
			//cout<<"utility1: "<<utility1<<endl;
			//cout<<"utility2: "<<utility2<<endl;
			//cout<<"branch prob: "<<branch_prob<<endl;

			utility=utility1*branch_prob+utility2*(1-branch_prob);
			//cout<<"branch prob: "<<branch_prob<<endl;
			//cout<<"utility: "<<utility<<endl;
			if (utility>maxUtil){
				maxUtil = utility;
				bestMove = i;
			}
		}
		s[depth]=0;
		utility = evaluateUtility(s);
		//cout<<"utility of publishing now: "<<utility<<endl;
		if (utility>maxUtil){
			maxUtil = utility;
			bestMove = 0;
		}
		 double rejectCost=0;
		 for (int i=0;i<s.size();i++){
		 	if (abs(s[i])>0){
		 		rejectCost-=reviewers[i].cost;
		 	}
		 }
		 if (rejectCost>maxUtil){
		 		maxUtil =rejectCost;
		 		bestMove = -1;
		 	}
		} else{

			maxUtil = evaluateUtility(s);
			//cout<<"s[0]: "<<s[0]<<", s[1]: "<<s[1]<<", utility at max depth: "<<maxUtil<<endl;
			double rejectCost=0;
			for (int i=0;i<s.size();i++){
				if (abs(s[i])>0){
					rejectCost-=reviewers[i].cost;
				}
			}
			if (rejectCost>maxUtil){
				maxUtil =rejectCost;
				bestMove=-1;
			}
			//cout<<"evaluating utility: "<<maxUtil<<endl;
		}
		//cout<<"maxUtil: "<<maxUtil<<", bestMove: "<<bestMove<<endl;
		//cout<<"depth: "<<depth<<", s[0]: "<<s[0]<<", s[1]: "<<s[1]<<", max utility: "<<maxUtil<<endl;
		//cout<<"states: "<<s[0]<<" "<<s[1]<<", utility1: "<<utility1<<", utility2: "<<utility2<<", depth: "<<depth<<", branch: "<<branch<<endl;

		return(maxUtil);
	};

	double evaluateUtility(vector<int> s){
		double succ=1;
		double fail=1;
		double fees=0;
		//cout<<"succ: "<<succ<<endl;
		for (int i=0;i<s.size();i++){
			if (s[i]!=0)
				fees+=reviewers[abs(s[i])-1].cost;

			if (s[i]>0){
				succ*=reviewers[abs(s[i])-1].RT;
				fail*=reviewers[abs(s[i])-1].RF;
			}
			if (s[i]<0){
				succ*=(1-reviewers[abs(s[i])-1].RT);
				fail*=(1-reviewers[abs(s[i])-1].RF);
			}
		//cout<<"succ: "<<succ<<", fail: "<<fail<<endl;
		}
		succ = succ*p/(succ*p+fail*(1-p));
		//cout<<"succ: "<<succ<<endl;

		double u = succ*S + (1-succ)*F-fees;
		
		return(u);
	}
};

double evaluateRejection(vector<int> s){

}

#endif