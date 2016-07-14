#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

double ModHausdorffDist(vector<vector<int>> vec1, vector<vector<int>> vec2){
	if (vec1[0].size() != vec2[0].size()){
		cout << "The dimensions of points in the two sets are not equal." << endl;
		system("pause");
		exit(1);
	}

	double fhd = 0, rhd = 0, mhd = 0;
	double minDist = DBL_MAX;
	for (int i = 0; i < vec1.size(); ++i){
		minDist = DBL_MAX;
		for (int j = 0; j < vec2.size(); ++j){
			double tmpDist = 0;
			for (int k = 0; k < vec1[0].size(); ++k)
				tmpDist += pow(vec1[i][k] - vec2[j][k], 2);
			tmpDist = pow(tmpDist, 0.5);
			if (tmpDist < minDist)
				minDist = tmpDist;
		}
		fhd += minDist;
	}
	fhd /= vec1.size();

	for (int j = 0; j < vec2.size(); ++j){
		minDist = DBL_MAX;
		for (int i = 0; i < vec1.size(); ++i){
			double tmpDist = 0;
			for (int k = 0; k < vec1[0].size(); ++k)
				tmpDist += pow(vec1[i][k] - vec2[j][k], 2);
			tmpDist = pow(tmpDist, 0.5);
			if (tmpDist < minDist)
				minDist = tmpDist;
		}
		rhd += minDist;
	}
	rhd /= vec2.size();

	mhd = max(fhd, rhd);
	return mhd;
}
