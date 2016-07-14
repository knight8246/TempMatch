#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <io.h>
#include <cv.h>
#include <highgui.h>
#include <direct.h>
#include <mat.h>
#include "Template.h"
using namespace cv;
using namespace std;

int str2num(string str){
	int num = 0, res = 0;
	for (unsigned int i = 0; i < str.size(); ++i){
		num = str[i] - '0';
		res = res * 10 + num;
	}
	return res;
}

Template xlsProcess(string filename){
	Template temp;
	string filepath = "D:\\QQ\\Lab_EX\\test\\data\\Templates\\";
	string context, cell = "";//context:each line in csv; cell:each cell in context;
	RECT *rect = new RECT[50];
	int count = 0, cntrect = 0, pos;//count:count for cell; cntrect:count for rect; pos:position;
	ifstream file;
	/*char *fname = new char[filename.size() + filepath.size()];

	for (int i = 0; i < filepath.size(); ++i)
	fname[i] = filepath[i];
	for (int i = 0; i < filename.size(); ++i)
	fname[i + filepath.size()] = filename[i];*/
	file.open(filename);
	if (!file){
		cout << "the file isn't exist!" << endl;
		system("pause");
		exit(1);
	}

	getline(file, context);
	//read second line in csv;
	getline(file, context);
	for (int i = 0; i < context.length(); ++i){
		if (context[i] == ','){
			switch (count){
			case 1:temp.FilePath = cell; break;
			case 2:temp.CompanyName = cell; break;
			case 3:temp.DocumentType = cell; break;
			case 4:temp.TemplateType = cell; break;
			case 5:if (cell == "FALSE")
				temp.PageFlag = '0';
				   else temp.PageFlag = '1'; break;
			case 6:if (cell == "FALSE")
				temp.TableFlag = '0';
				   else temp.TableFlag = '1'; break;
			case 8:pos = str2num(cell);
				rect[cntrect].pos.x = pos; break;
			case 9:pos = str2num(cell);
				rect[cntrect].pos.y = pos; break;
			case 10:pos = str2num(cell);
				rect[cntrect].pos.width = pos; break;
			case 11:pos = str2num(cell);
				rect[cntrect].pos.height = pos; break;
			case 12:rect[cntrect].FeatureFlag = cell; break;
			case 13:rect[cntrect].DatabaseTablename = cell; break;
			case 15:rect[cntrect].DatabaseColname = cell; break;
			}
			++count;
			cell = "";
		}
		else cell += context[i];
	}
	++cntrect;

	//read other line in csv;
	while (getline(file, context)){
		cell = "";
		count = 0;
		for (unsigned int i = 0; i < context.size(); ++i){
			if (context[i] == ','){
				switch (count){
				case 8:pos = str2num(cell);
					rect[cntrect].pos.x = pos; break;
				case 9:pos = str2num(cell);
					rect[cntrect].pos.y = pos; break;
				case 10:pos = str2num(cell);
					rect[cntrect].pos.width = pos; break;
				case 11:pos = str2num(cell);
					rect[cntrect].pos.height = pos; break;
				case 12:rect[cntrect].FeatureFlag = cell; break;
				case 13:rect[cntrect].DatabaseTablename = cell; break;
				case 15:rect[cntrect].DatabaseColname = cell; break;
				}
				++count;
				cell = "";
			}
			else cell += context[i];
		}
		++cntrect;
	}
	
	temp.rectNum = cntrect;
	temp.rect = new RECT[cntrect];
	for (int i = 0; i < cntrect; ++i)
		temp.rect[i] = rect[i];

	file.close();
	return temp;
}