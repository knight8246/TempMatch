#include <cv.h>
#include <highgui.h>
#include <io.h>
#include <direct.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <mat.h>

using namespace cv;
using namespace std;

//TODO:class Template
struct Template{};

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

int maind(){
	vector<string> TemplatesInfoList;
	Template *templates;//template in matlab
	Mat templateImg;
	double partNum = 1 / 4;
	int binNum = 30;
	double **histogram;
	histogram = new double *[TemplatesInfoList.size()];
	for (int i = 0; i < TemplatesInfoList.size(); ++i)
		histogram[i] = new double[binNum * 2];

	//��ȡexcel�ļ����б�
	getFiles("D:\\QQ\\Lab_EX\\project\\data\\Templates", TemplatesInfoList);

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
			histogram[i][j] = templates[i].histogram[j];
	}

	//TODO:ʹ��kmeans����


	//��ģ��Ĵ�������ṹ��
	for (int i = 0; i < sizeof(templates) / sizeof(Template); ++i){
		templates[i].cata = ids[i];
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

