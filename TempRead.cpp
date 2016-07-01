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
#include <fstream>
#include "Template.h"

extern Template xlsProcess(string filename);


using namespace cv;
using namespace std;

extern double* templateHistogram(Mat templateImg, double partNum, int binNum);

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


int main(){
	string filepath = "D:\\QQ\\Lab_EX\\test\\data\\Templates\\";
	vector<string> TemplatesInfoList;
	Template *templates;//template in matlab
	Mat templateImg;
	double partNum = 1 / 4;
	int binNum = 30;
	Mat histogram;
	Mat labels;
	Mat centers;
	ofstream file;

	//获取excel文件名列表
	getFiles("D:\\QQ\\Lab_EX\\test\\data\\Templates", TemplatesInfoList);

	templates = new Template[TemplatesInfoList.size()];

	//读取模版信息
	//xlsProcess
	for (int i = 0; i < TemplatesInfoList.size(); ++i){
		cout << TemplatesInfoList[i].c_str() << endl;
		templates[i] = xlsProcess(TemplatesInfoList[i].c_str());
	}
	
	//计算表格表头部分的横纵投影
	//templateHistogram
	for (int i = 0; i < sizeof(templates)/sizeof(Template); ++i){
		templateImg = imread(templates[i].FilePath);
		templates[i].histogram = templateHistogram(templateImg, partNum, binNum);
	}

	//将所有投影存入矩阵
	for (int i = 0; i < sizeof(templates) / sizeof(Template); ++i){
		for (int j = 0; j < sizeof(templates[i].histogram) / sizeof(double); ++j)
			histogram.at<uchar>(i, j) = templates[i].histogram[j];
	}

	//使用kmeans聚类
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

	//TODO:保存数据 templates
	//save clusterCenter
	file.open("clusterCenter.csv");
	for (int i = 0; i < centers.rows; ++i){
		for (int j = 0; j < centers.cols; ++j)
			file << centers.at<uchar>(i, j) << ",";
		file << "\n";
	}
	file.close();

	//save templates
	file.open("templateInfo.csv");
	for (int i = 0; i < sizeof(templates) / sizeof(Template); ++i){
		file << templates[i].FilePath << "," << templates[i].CompanyName << ",";
		file << templates[i].DocumentType << "," << templates[i].TemplateType << ",";
		file << templates[i].PageFlag << "," << templates[i].TableFlag << ",";
		file << templates[i].cata << "," << sizeof(templates[i].rect) / sizeof(RECT) << "\n";
		for (int j = 0; j < sizeof(templates[i].histogram) / sizeof(double); ++j){
			file << templates[i].histogram[j] << ",";
		}
		file << "\n";
		for (int j = 0; j < sizeof(templates[i].rect) / sizeof(RECT); ++j){
			file << templates[i].rect[i].pos[0] << "," << templates[i].rect[i].pos[1] << ",";
			file << templates[i].rect[i].pos[2] << "," << templates[i].rect[i].pos[3] << ",";
			file << templates[i].rect[i].FeatureFlag << "," << templates[i].rect[i].DatabaseTablename << ",";
			file << templates[i].rect[i].DatabaseColname << "\n";
		}
	}

	system("pause");

	return 0;
}