#ifndef MODEL_H
#define MODEL_H
#include "Bio.h"
#include <iomanip>
using namespace std;

class Model{
public:
	vector<vector<double> > logProbs;
	vector<double> logProbVec;
	vector<string> categories;
	vector<string> words;
	Model(vector<vector<double> > lp, vector<double> lpv,vector<string> c,vector<string> w): words(w),categories(c),logProbVec(lpv),logProbs(lp){};
	
	predict(vector<Bio> testSet){
		vector<vector<double> > testL;
		for (int i=0;i<testSet.size();i++){
			vector<double> catL;
			Bio thisBio =testSet[i];
			for (int j=0;j<categories.size();j++){
				double L_C = logProbVec[j];
				double sum=0;
				
				for (int k=0;k<thisBio.words.size();k++){

					//cout<<"word: "<<thisBio.words[k]<<endl;
					int wordIndex = -1;
					string thisWord = thisBio.words[k];
					for (int l=0;l<words.size();l++){
						//cout<<"compare word: "<<words[l]<<endl;
						if (thisWord==words[l]){
							
							wordIndex=l;
							break;
						}
					}
					double L_WC = logProbs[j][wordIndex];
					if (wordIndex==-1)
						L_WC=0;
					
					sum+=L_WC;
				}
				double totalLikelihood = L_C+sum;
				
				catL.push_back(totalLikelihood);
			}
			testL.push_back(catL);
		}
		//find minimimum likelihood category for each bio;
		vector<double> minLIndices;
		vector<double> minLs;
		for (int i=0;i<testSet.size();i++){
			//cout<<"bio: "<<testSet[i].name<<endl;
			double minL=testL[i][0];
			int minLIndex = -1;
			for (int j=0;j<testL[i].size();j++){
				//cout<<"minL: "
				if (testL[i][j]<=minL){

					//cout<<"i: "<<i<<", j: "<<j<<endl;
					minL =testL[i][j];
					minLIndex = j;
				}
			}
			minLs.push_back(minL);
			minLIndices.push_back(minLIndex);
		}
		vector<vector<double> > probs;
		for (int i=0;i<testSet.size();i++){
			double sum=0;
			vector<double> thisProb;
			for (int j=0;j<categories.size();j++){

				double cMinusM = minLs[i]-testL[i][j];
				
				double x=0;
				if (cMinusM<7){
					x=pow(2,cMinusM);
				}
				
				sum+=x;
			}
			
			for (int j=0;j<categories.size();j++){
				double cMinusM = minLs[i]-testL[i][j];
				double x=0;
				if (cMinusM<7){
					x=pow(2,cMinusM);
				}
				thisProb.push_back(floor(1000*x/sum)/1000);
			}
			probs.push_back(thisProb);
		}

		cout<<"                      ";
		for (int i=0;i<categories.size();i++){
			cout<<setw(15)<<categories[i];
		}
		cout<<"        maxprob      predicted         actual        correct"<<endl;
		double accuracy=0;
		for (int i=0;i<testSet.size();i++){
			accuracy+=(categories[minLIndices[i]]==testSet[i].category);
			cout<<setw(20)<<testSet[i].name<<": ";
			for (int j=0;j<categories.size();j++){
				cout<<setw(15)<<probs[i][j];
			}
			cout<<setw(15)<<probs[i][minLIndices[i]]<<setw(15)<<categories[minLIndices[i]]<<setw(15)<<testSet[i].category<<setw(15)<<(categories[minLIndices[i]]==testSet[i].category)<<endl;
			
		}
		accuracy/=testSet.size();

		cout<<"test accuracy: "<<accuracy<<endl;
	};

};
#endif