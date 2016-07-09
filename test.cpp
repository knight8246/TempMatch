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

extern double* templateHistogram(Mat templateImg, double partNum, int binNum);
extern Rect BorderRevised(Rect RectPosition, Mat framegray, int EdgeWidth);
extern double m_hd(Mat img1, Mat img2);

int str2int(string str){
	int num = 0, res = 0;
	for (unsigned int i = 0; i < str.length(); ++i){
		num = str[i] - '0';
		res = res * 10 + num;
	}
	return res;
}

string double2str(double num){
	stringstream ss;
	string str;
	ss << num;
	ss >> str;
	return str;
}

float str2float(string str){
	stringstream ss;
	float num;
	ss << str;
	ss >> num;
	return num;
}

Mat cRead(string filename){
	ifstream file;
	string filepath = "D:\\QQ\\Lab_EX\\test\\TempMatch\\TempMatch\\";
	string context, cell = ""; //context:each line in csv; cell:each cell in context;
	filename = filepath + filename;
	file.open(filename);
	if (!file){
		cout << "the file isn't exist!" << endl;
		system("pause");
		exit(1);
	}

	getline(file, context);
	int rowNum = str2int(context);
	getline(file, context);
	int colNum = str2int(context);
	Mat result(rowNum, colNum, CV_32FC1);

	int colCount = 0;
	for (int i = 0; i < rowNum; ++i){
		getline(file, context);
		colCount = 0;
		for (int j = 0; j < context.length(); ++j){
			if (context[j] == ','){
				result.at<float>(i, colCount) = str2float(cell);
				++colCount;
				cell = "";
			}
			else
				cell += context[j];
		}
		if (colCount != colNum){
			cout << "Data Error!" << endl;
			system("pause");
			exit(1);
		}
	}

	file.close();
	return result;
}

Template* tRead(string filename, int &tempNum){
	ifstream file;
	string filepath = "D:\\QQ\\Lab_EX\\test\\TempMatch\\TempMatch\\";
	int count = 0, cntrect = 0, pos;//count:count for cell; cntrect:count for rect; pos:position;
	string context, cell = ""; //context:each line in csv; cell:each cell in context;
	filename = filepath + filename;
	file.open(filename);
	if (!file){
		cout << "the file isn't exist!" << endl;
		system("pause");
		exit(1);
	}
	
	getline(file, context);
	tempNum = str2int(context);
	int histCount = 0;
	int rectCount = 0;
	Template *result = new Template[tempNum];
	for (int i = 0; i < tempNum; ++i){
		getline(file, context);
		count = 0;
		histCount = 0;
		rectCount = 0;
		cell = "";
		for (int j = 0; j < context.length(); ++j){
			if (context[j] == ','){
				switch (count){
				case 0: result[i].CompanyName = cell; ++count; break;
				case 1: result[i].DocumentType = cell; ++count; break;
				case 2: result[i].FilePath = cell; ++count; break;
				case 3: result[i].TemplateType = cell; ++count; break;
				case 4:
					if (cell == "FLASE")
						result[i].PageFlag = '0';
					else
						result[i].PageFlag = '1';
					++count; break;
				case 5:
					if (cell == "FLASE")
						result[i].TableFlag = '0';
					else
						result[i].TableFlag = '1';
					++count; break;
				case 6:
					result[i].histCols = str2int(cell);
					result[i].histogram = new double[str2int(cell)];
					++count; break;
				case 7:
					result[i].histogram[histCount] = str2float(cell);
					++histCount;
					if (histCount == result[i].histCols)
						++count;
					break;
				case 8: result[i].cata = str2int(cell); ++count; break;
				case 9:
					result[i].rectNum = str2int(cell);
					result[i].rect = new RECT[str2int(cell)];
					++count; break;
				case 10: result[i].rect[rectCount].pos.x = str2int(cell); ++count; break;
				case 11: result[i].rect[rectCount].pos.y = str2int(cell); ++count; break;
				case 12: result[i].rect[rectCount].pos.width = str2int(cell); ++count; break;
				case 13: result[i].rect[rectCount].pos.height = str2int(cell); ++count; break;
				case 14: result[i].rect[rectCount].FeatureFlag = cell; ++count; break;
				case 15: result[i].rect[rectCount].DatabaseTablename = cell; ++count; break;
				case 16:
					result[i].rect[rectCount].DatabaseColname = cell;
					++rectCount;
					if (rectCount == result[i].rectNum)
						++count;
					else
						count = 10;
				}
				cell = "";
			}
			else cell += context[j];
		}
	}

	file.close();
	return result;
}

/*
int main(){
	Mat tempBox, dataBox, display_left, display_right, display;
	tempBox = imread("S:\\Project_TempMatch\\TempMatch\\data\\Jinx.jpg");
	dataBox = imread("S:\\Project_TempMatch\\TempMatch\\data\\Jinx.jpg");
	imshow("t",tempBox);
	waitKey(0);
	copyMakeBorder(tempBox, display_left, 0, 0, 0, dataBox.cols, BORDER_ISOLATED);
	copyMakeBorder(dataBox, display_right, 0, 0, tempBox.cols, 0, BORDER_ISOLATED);
	add(display_left, display_right, display);
	double score = 1.234;
	cout << double2str(score) << endl;
	//imshow("HD"+double2str(score), display);
	//waitKey(0);
	system("pause");
}
*/

int main(){
	//读取样本与聚类类心
	int tempNum;
	cout << "before cRead" << endl;
	Mat C = cRead("clusterCenter.csv");
	cout << "before tRead" << endl;
	Template *templates = tRead("templateInfo.csv",tempNum);
	cout << "done" << endl;

	//读取测试图像，并转为灰度图
	Mat dataTest;
	dataTest= imread("D:\\QQ\\Lab_EX\\project\\data\\testdata\\20160524测试文件\\JH16050075\\发票.jpg");
	//dataTest = imread("D:\\QQ\\Lab_EX\\project\\data\\testdata\\0005.jpg");
	Mat dataTestGray;
	if (dataTest.channels() > 1)
		cvtColor(dataTest, dataTestGray, CV_RGB2GRAY);
	else
		dataTestGray = dataTest;

	cout << "read done" << endl;
	
	//统计表格前partNum部分的横纵投影
	double partNum = (double)1 / 4;
	int binNum = 30;
	double *testHistogram = new double[2 * binNum];
	testHistogram = templateHistogram(dataTestGray, partNum, binNum);

	cout << "div done" << endl;
	
	//根据表头粗分类
	double *distance = new double[C.rows];
	double num, den1, den2;
	double minDistance = DBL_MAX;
	int minC;
	for (int i = 0; i < C.rows; ++i){
		num = den1 = den2 = 0;
		for (int j = 0; j < C.cols; ++j){
			num += testHistogram[j] * double(C.at<float>(i, j));
			den1 += pow(testHistogram[j], 2);
			den2 += pow(double(C.at<float>(i, j)), 2);
		}
		distance[i] = num / sqrt(den1 * den2);
		if (distance[i] < minDistance){
			minDistance = distance[i];
			minC = i;
		}
	}

	cout << "calc done" << endl;

	//coarseIdx用来储存与表头投影距离最近的聚类中心所包含的模板编号
	bool *coarseIdx = new bool[tempNum];
	for (int i = 0; i < tempNum; ++i){
		if (templates[i].cata == minC)
			coarseIdx[i] = true;
		else
			coarseIdx[i] = false;
	}

	cout << "center done" << endl;

	//根据唯一性特征框精确查找
	int featureNum;
	int idScore;
	Mat img, framegray;
	Rect tempRectPosition, revisedTempRectPosition;
	Rect dataRectPosition, revisedDataRectPosition;
	Mat tempBox, dataBox;
	Mat display_left, display_right, display;
	double score, minTempScore=DBL_MAX;
	double *tempScore = new double[tempNum];
	for (int i = 0; i < tempNum; ++i)
		tempScore[i] = 0;

	for (int i = 0; i < tempNum; ++i){
		if (!coarseIdx[i])
			continue;
		img = imread(templates[i].FilePath);
		if (img.channels() == 3)
			cvtColor(img, framegray, CV_RGB2GRAY);
		else
			framegray = img;

		featureNum = 0;
		for (int j = 0; j < templates[i].rectNum; ++j){
			if (!strcmp(templates[i].rect[j].FeatureFlag.c_str(), "模板特征")){
				//需要进行矩形框识别
				tempRectPosition = templates[i].rect[j].pos;
				revisedTempRectPosition = BorderRevised(tempRectPosition, framegray, 10); //边界处理
				tempBox = framegray(revisedTempRectPosition); //截取特征框
				//归一化特征框位置
				dataRectPosition.x = int(floor(tempRectPosition.x / framegray.cols * dataTestGray.cols));
				dataRectPosition.y = int(floor(tempRectPosition.y / framegray.rows * dataTestGray.rows));
				dataRectPosition.width = tempRectPosition.width;
				dataRectPosition.height = tempRectPosition.height;
				revisedDataRectPosition = BorderRevised(dataRectPosition, dataTestGray, 10);
				dataBox = dataTestGray(revisedDataRectPosition);
				//resize(dataBox, dataBox, tempBox.size(), CV_INTER_LINEAR);//try to save the pro.change

				//计算特征框间相似度
				threshold(tempBox, tempBox, 0, 255, CV_THRESH_OTSU);
				threshold(dataBox, dataBox, 0, 255, CV_THRESH_OTSU);
				cout << tempBox.size() << endl;
				cout << dataBox.size() << endl;
				system("pause");
				score = m_hd(tempBox, dataBox);
				tempScore[i] += score;

				copyMakeBorder(tempBox, display_left, 0, 0, 0, dataBox.cols, BORDER_ISOLATED);
				copyMakeBorder(dataBox, display_right, 0, 0, tempBox.cols, 0, BORDER_ISOLATED);
				add(display_left, display_right, display);
				if (display.cols > 1000){
					CvSize size;
					size.width = 1000;
					size.height = int(floor((double)1000 / display.cols * display.rows));
					resize(display, display, size, CV_INTER_LINEAR);
				}
				if (display.rows > 1000){
					CvSize size;
					size.height = 1000;
					size.width = int(floor((double)1000 / display.rows * display.cols));
					resize(display, display, size, CV_INTER_LINEAR);
				}
				//imshow("hausdorf distance is "+double2str(score), display);
				//waitKey(0);

				++featureNum;
			}
		}
		tempScore[i] /= featureNum;
		if (tempScore[i] < minTempScore){
			minTempScore = tempScore[i];
			idScore = i;
		}
		//cout << i << endl;
		//cout << featureNum << endl;
	}

	cout << "search done" << endl;
	//cout << "featureNum:" << featureNum << endl;
	//cout << "idScore:" << idScore << endl;

	Mat tempMatched = imread(templates[idScore].FilePath);
	Mat dataDisplay(dataTest);
	if (tempMatched.rows - dataTest.rows != 0 || tempMatched.cols - dataTest.cols != 0)
		resize(dataTest, dataDisplay, tempMatched.size(), CV_INTER_LINEAR);
	copyMakeBorder(dataDisplay, display_left, 0, 0, 0, tempMatched.cols + 10, BORDER_ISOLATED);
	copyMakeBorder(tempMatched, display_right, 0, 0, dataDisplay.cols + 10, 0, BORDER_ISOLATED);
	add(display_left, display_right, display);
	if (display.cols > 1000){
		CvSize size;
		size.width = 1000;
		size.height = int(floor((double)1000 / display.cols * display.rows));
		resize(display, display, size, CV_INTER_LINEAR);
	}
	if (display.rows > 1000){
		CvSize size;
		size.height = 1000;
		size.width = int(floor((double)1000 / display.rows * display.cols));
		resize(display, display, size, CV_INTER_LINEAR);
	}
	imshow("Result", display);
	waitKey(0);

	//system("pause");

	return 0;
}
