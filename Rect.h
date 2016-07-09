#ifndef RECT_
#define RECT_
#include <iostream>
#include <cv.h>
using namespace cv;
using namespace std;

class RECT{
public:
	Rect pos; //x,y,w,h
	string FeatureFlag;
	string DatabaseTablename;
	string DatabaseColname;
};
#endif