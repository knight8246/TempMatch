#include <iostream>
#include <cv.h>
using namespace std;
using namespace cv;

Rect BorderRevised(Rect RectPosition, Mat framegray, int EdgeWidth){
	Rect revisedRectPosition;
	revisedRectPosition.x = RectPosition.x - EdgeWidth;
	revisedRectPosition.y = RectPosition.y - EdgeWidth;
	revisedRectPosition.width = RectPosition.width + 2 * EdgeWidth;
	revisedRectPosition.height = RectPosition.height + 2 * EdgeWidth;
	if (revisedRectPosition.x < 0)
		revisedRectPosition.x = 0;
	if (revisedRectPosition.y < 0)
		revisedRectPosition.y = 0;
	if (revisedRectPosition.x + revisedRectPosition.width >= framegray.cols)
		revisedRectPosition.width = framegray.cols - revisedRectPosition.x - 1;
	if (revisedRectPosition.y + revisedRectPosition.height >= framegray.rows)
		revisedRectPosition.height = framegray.rows - revisedRectPosition.y - 1;

	return revisedRectPosition;
}