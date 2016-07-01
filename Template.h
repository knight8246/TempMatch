#ifndef Template_
#define Template_
#include <iostream>
#include "Rect.h"
using namespace std;

class Template{
public:
	string CompanyName;
	string DocumentType;
	string FilePath;
	string TemplateType;
	char PageFlag;
	char TableFlag;
	RECT *rect;
	double *histogram;
	int cata;
};
#endif
