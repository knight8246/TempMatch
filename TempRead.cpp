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
#include "xlsProcess.cpp"

using namespace cv;
using namespace std;

void getFiles(string path, vector<string>& files){
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1){
		do{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if((fileinfo.attrib &  _A_SUBDIR)){
				if(strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		}
		while(_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

extern double* templateHistogram(Mat templateImg, double partNum, int binNum);

int maind(){
	vector<string> TemplatesInfoList;
	Template *templates;//template in matlab
	Mat templateImg;
	double partNum = 1 / 4;
	int binNum = 30;
	Mat histogram;
	Mat labels;
	Mat centers;

	//获取excel文件名列表
	getFiles("D:\\QQ\\Lab_EX\\test\\data\\Templates", TemplatesInfoList);

	templates = new Template[TemplatesInfoList.size()];

	//读取模版信息
	//TODO:xlsProcess
	for (int i = 0; i < TemplatesInfoList.size(); ++i)
		templates[i] = xlsProcess(TemplatesInfoList[i].c_str());
	
	//计算表格表头部分的横纵投影
	//TODO:templateHistogram
	for (int i = 0; i < sizeof(templates)/sizeof(Template); ++i){
		templateImg = imread(templates[i].FilePath);
		templates[i].histogram = templateHistogram(templateImg, partNum, binNum);
	}

	//将所有投影存入矩阵
	for (int i = 0; i < sizeof(templates) / sizeof(Template); ++i){
		for (int j = 0; j < sizeof(templates[i].histogram) / sizeof(double); ++j)
			histogram.at<uchar>(i, j) = templates[i].histogram[j];
	}

	//TODO:使用kmeans聚类
	kmeans(histogram, 2, labels, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0), 1, KMEANS_RANDOM_CENTERS, centers);
	//histogram-聚类数据
	//clusterCount-聚类个数
	//labels-聚类编号
	//TermCriteria-迭代终止条件
	//attempts-聚类次数
	//flags-选取初始点方法
	//centers-聚类中心

	//将模版的粗类别存入结构体
	for (int i = 0; i < sizeof(templates) / sizeof(Template); ++i){
		templates[i].cata = int(labels.at<uchar>(i));
	}

	//TODO:保存数据
	double *outC = new double[M*N];
	for (int i = 0; i<M; i++)
		for (int j = 0; j<N; j++)
			outC[M*j + i] = C[i][j];
	pmatFile = matOpen("culsterCenter.mat", "w");
	pMxArray = mxCreateDoubleMatrix(M, N, mxREAL);
	mxSetData(pMxArray, outC);
	matPutVariable(pmatFile, "A", pMxArray);
	matClose(pmatFile);

	system("pause");

	return 0;
}