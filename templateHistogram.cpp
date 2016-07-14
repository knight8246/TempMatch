#include <iostream>
#include <cv.h>
#include <highgui.h>
using namespace std;
using namespace cv;

double* templateHistogram(Mat templateImg, double partNum, int binNum){
	Mat img;
	CvSize normalSize;
	//归一化尺寸
	normalSize.height = 600;
	normalSize.width = 600;
	//OTSU二值化
	if (templateImg.channels() > 1)
		cvtColor(templateImg, img, CV_RGB2GRAY);
	else
		templateImg.copyTo(img);
	resize(img, img, normalSize, CV_INTER_LINEAR);
	threshold(img, img, 0, 255, CV_THRESH_OTSU);
	

	int *count = new int[binNum];
	double *histogram = new double[2 * binNum];
	int minCount = INT_MAX, maxCount = 0;
	for (int i = 0; i < binNum; ++i)
		count[i] = 0;

	//对表头求投影
	//横投影
	for (int k = 0; k < binNum; ++k){
		for (int i = 0; i < floor(normalSize.height * partNum); ++i){
			for (int j = k * floor(normalSize.width / binNum); j < (k + 1) * floor(normalSize.width / binNum); ++j){
				count[k] += int(img.at<uchar>(i, j));
			}
		}
		if (count[k] < minCount)
			minCount = count[k];
		if (count[k] > maxCount){
			maxCount = count[k];
		}
		
	}
	for (int k = 0; k < binNum; ++k){
		histogram[k] = double(count[k] - minCount) / maxCount;
	}

	//初始化
	for (int i = 0; i < binNum; ++i)
		count[i] = 0;
	//纵投影
	minCount = INT_MAX; maxCount = 0;
	for (int k = 0; k < binNum; ++k){
		for (int i = k*floor(normalSize.height * partNum / binNum); i < (k + 1) * floor(normalSize.height * partNum / binNum); ++i){
			for (int j = 0; j < normalSize.width; ++j){
				count[k] += int(img.at<uchar>(i, j));
			}
		}
		if (count[k] < minCount)
			minCount = count[k];
		if (count[k] > maxCount){
			maxCount = count[k];
		}
	}
	for (int k = binNum; k < 2*binNum; ++k)
		histogram[k] = double(count[k-binNum] - minCount) / maxCount;

	return histogram;
}