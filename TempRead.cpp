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
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1){
		do{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
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

	//��ȡexcel�ļ����б�
	getFiles("D:\\QQ\\Lab_EX\\test\\data\\Templates", TemplatesInfoList);

	templates = new Template[TemplatesInfoList.size()];

	//��ȡģ����Ϣ
	//TODO:xlsProcess
	for (int i = 0; i < TemplatesInfoList.size(); ++i)
		templates[i] = xlsProcess(TemplatesInfoList[i].c_str());
	
	//�������ͷ���ֵĺ���ͶӰ
	//TODO:templateHistogram
	for (int i = 0; i < sizeof(templates)/sizeof(Template); ++i){
		templateImg = imread(templates[i].FilePath);
		templates[i].histogram = templateHistogram(templateImg, partNum, binNum);
	}

	//������ͶӰ�������
	for (int i = 0; i < sizeof(templates) / sizeof(Template); ++i){
		for (int j = 0; j < sizeof(templates[i].histogram) / sizeof(double); ++j)
			histogram.at<uchar>(i, j) = templates[i].histogram[j];
	}

	//TODO:ʹ��kmeans����
	kmeans(histogram, 2, labels, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0), 1, KMEANS_RANDOM_CENTERS, centers);
	//histogram-��������
	//clusterCount-�������
	//labels-������
	//TermCriteria-������ֹ����
	//attempts-�������
	//flags-ѡȡ��ʼ�㷽��
	//centers-��������

	//��ģ��Ĵ�������ṹ��
	for (int i = 0; i < sizeof(templates) / sizeof(Template); ++i){
		templates[i].cata = int(labels.at<uchar>(i));
	}

	//TODO:��������
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