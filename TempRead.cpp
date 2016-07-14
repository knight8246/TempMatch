#include <iostream>
#include <fstream>
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
using namespace std;
using namespace cv;

extern Template xlsProcess(string filename);
extern double* templateHistogram(Mat templateImg, double partNum, int binNum);

void getFiles(string path, vector<string>& files){
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1){
		do{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
			if ((fileinfo.attrib &  _A_SUBDIR)){
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

int main(){
	string filepath = "D:\\QQ\\Lab_EX\\test\\data\\Templates\\";
	vector<string> TemplatesInfoList;
	Template *templates; //template in matlab
	Mat templateImg;
	double partNum = (double)1 / 4;
	int binNum = 30;
	Mat labels;
	Mat centers;

	//��ȡexcel�ļ����б�
	getFiles("D:\\QQ\\Lab_EX\\test\\data\\Templates", TemplatesInfoList);

	int tempNum = TemplatesInfoList.size();
	templates = new Template[tempNum];

	//��ȡģ����Ϣ
	//xlsProcess
	for (int i = 0; i < tempNum; ++i){
		//cout << TemplatesInfoList[i].c_str() << endl;
		templates[i] = xlsProcess(TemplatesInfoList[i]);
	}

	//�������ͷ���ֵĺ���ͶӰ
	//templateHistogram
	for (int i = 0; i < tempNum; ++i){
		//cout << templates[i].FilePath << endl;
		templateImg = imread(templates[i].FilePath);
		templates[i].histCols = 2 * binNum;
		templates[i].histogram = templateHistogram(templateImg, partNum, binNum);
	}

	//������ͶӰ�������
	Mat histogram(tempNum, 2*binNum, CV_32FC1);
	for (int i = 0; i < tempNum; ++i){
		for (int j = 0; j < 2 * binNum; ++j){
			histogram.at<float>(i, j) = templates[i].histogram[j];
		}
	}
	
	//cout << "before kmeans" << endl;
	//ʹ��kmeans����
	kmeans(histogram, 5, labels, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0), 3, KMEANS_PP_CENTERS, centers);
	//histogram-��������
	//clusterCount-�������
	//labels-������
	//TermCriteria-������ֹ����
	//attempts-�������
	//flags-ѡȡ��ʼ�㷽��
	//centers-��������
	//cout << "after kmeans" << endl;
	//cout << labels << endl;
	//cout << centers << endl;

	//��ģ��Ĵ�������ṹ��
	for (int i = 0; i < tempNum; ++i){
		templates[i].cata = labels.at<int>(i);
	}

	//�������� templates
	ofstream file;
	file.open("clusterCenter.csv");
	file << centers.rows << '\n';
	file << centers.cols << '\n';
	for (int i = 0; i < centers.rows; ++i){
		for (int j = 0; j < centers.cols; ++j)
			file << centers.at<float>(i, j) << ',';
		file << '\n';
	}
	file.close();

	file.open("templateInfo.csv");
	file << tempNum << '\n';
	for (int i = 0; i < tempNum; ++i){
		file << templates[i].CompanyName << ',';
		file << templates[i].DocumentType << ',';
		file << templates[i].FilePath << ',';
		file << templates[i].TemplateType << ',';
		file << templates[i].PageFlag << ',';
		file << templates[i].TableFlag << ',';
		file << templates[i].histCols << ',';
		for (int j = 0; j < templates[i].histCols; ++j){
			file << templates[i].histogram[j] << ',';
		}
		file << templates[i].cata << ',';
		file << templates[i].rectNum << ',';
		for (int j = 0; j < templates[i].rectNum; ++j){
			file << templates[i].rect[j].pos.x << ',';
			file << templates[i].rect[j].pos.y << ',';
			file << templates[i].rect[j].pos.width << ',';
			file << templates[i].rect[j].pos.height << ',';
			file << templates[i].rect[j].FeatureFlag << ',';
			file << templates[i].rect[j].DatabaseTablename << ',';
			file << templates[i].rect[j].DatabaseColname << ',';
		}
		file << '\n';
	}
	file.close();

	system("pause");
	return 0;
}
