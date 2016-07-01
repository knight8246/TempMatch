#include <iostream>
#include <cv.h>
#include <highgui.h>
using namespace std;
using namespace cv;

double* templateHistogram(Mat templateImg, double partNum, int binNum){
	Mat img;
	CvSize normalSize;
	normalSize.height = 60;
	normalSize.width = 60;
	if (templateImg.channels() > 1)
		cvtColor(templateImg, img, CV_RGB2GRAY);
	else
		img = templateImg;
	resize(img, img, normalSize);
	threshold(img, img, 0, 255, CV_THRESH_OTSU);

	int *count = new int[binNum];
	for (int i = 0; i < binNum; ++i)
		count[i] = 0;
	double *histogram = new double[2 * binNum];
	int minCount = INT_MAX, maxCount = 0;
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
	for (int k = 0; k < binNum; ++k)
		histogram[k] = double(count[k] - minCount) / maxCount;
	for (int i = 0; i < binNum; ++i)
		count[i] = 0;
	minCount = 255; maxCount = 0;
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