#include <iostream>
#include <vector>
#include <numeric>
#include <cv.h>
using namespace std;
using namespace cv;

double m_hd(Mat img1, Mat img2){
	double d; //the modified haussdorff distance
	Mat img1_not, img2_not;
	Mat distImg1, distImg2;
	vector<double> Dist1, Dist2;
	double sum1, sum2, d12, d21;

	//输入检查
	if (img1.rows-img2.rows!=0 || img1.cols-img2.cols!=0){
		cout << "img1 and img2 must have the same size" << endl;
		exit(1);
	}
	bitwise_not(img1, img1_not); //二值图像取反
	distanceTransform(img1_not, distImg1, CV_DIST_L2, CV_DIST_MASK_PRECISE); //bwdist in MATLAB
	bitwise_not(img2, img2_not);
	distanceTransform(img2_not, distImg2, CV_DIST_L2, CV_DIST_MASK_PRECISE);
	//计算距离
	for (int i = 0; i < img1.rows; ++i){
		for (int j = 0; j < img1.cols; ++j){
			if (int(img2.at<uchar>(i, j)))
				Dist1.push_back(distImg1.at<float>(i, j));
			if (int(img1.at<uchar>(i, j)))
				Dist2.push_back(distImg2.at<float>(i, j));
		}
	}
	//计算均值
	sum1 = accumulate(Dist1.begin(), Dist1.end(), 0.0);
	sum2 = accumulate(Dist2.begin(), Dist2.end(), 0.0);
	d12 = sum1 / Dist1.size();
	d21 = sum2 / Dist2.size();
	d = max(d12, d21);

	return d;
}
