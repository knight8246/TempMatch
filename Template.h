#ifndef Template_
#define Template_
#include <iostream>
#include "RECT.h"
using namespace std;

class Template{
public:
	string CompanyName;
	string DocumentType;
	string FilePath;
	string TemplateType;
	char PageFlag;
	char TableFlag;
	int rectNum;
	RECT *rect;
	int histCols;
	double *histogram;
	int cata;
};
#endif