#include <cv.h>
#include <highgui.h>
using namespace std;
int test()
{
	IplImage * test;
	test = cvLoadImage("D:\\QQ\\Lab_EX\\project\\data\\testdata\\0001.jpg");//ͼƬ·��
	cvNamedWindow("test_demo", 1);
	cvShowImage("test_demo", test);
	cvWaitKey(0);
	cvDestroyWindow("test_demo");
	cvReleaseImage(&test);
	return 0;
}