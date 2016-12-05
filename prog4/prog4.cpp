#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Digester.h"
#include "Bio.h"
#include "Model.h"

using namespace std;

int main(int argc, char* argv[]){

	Digester trainingSet(argv[1],argv[3]);
	Model model = trainingSet.train(.1);
	Digester testingSet(argv[2],argv[3]);
	model.predict(testingSet.bios);
	



}
