#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include "CSpreadSheet.h"

using namespace std;

int main(){
	CStringArray RowValues;
	CSpreadSheet SS("D:\\QQ\\Lab_EX\\project\\data\\Templates\\GoldenSeasoninvoice20160330161341.xls","Sheet");
	int rows = SS.GetTotalRows();
	cout<<rows<<endl;
	SS.ReadRow(RowValues);
}
