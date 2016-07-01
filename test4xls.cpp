#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <io.h>
#include <cv.h>
#include <highgui.h>
#include <direct.h>
#include <mat.h>
#include "Template.h"

extern Template xlsProcess(string filename);

using namespace cv;
using namespace std;

int main(){
	string file = "GoldenSeasoninvoice20160330161341.csv";
	Template temp;
	temp = xlsProcess(file);
	cout << temp.FilePath << endl;

	system("pause");

	return 0;
}