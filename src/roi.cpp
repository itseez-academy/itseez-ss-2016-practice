#include "roi.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

Rect getContainedRoi(const Mat & src, const Rect & roi)
{
	Rect newRoi = Rect();

	newRoi.x = max(0, roi.x);
	newRoi.y = max(0, roi.y);

	newRoi.width = roi.x < 0 ? roi.width + roi.x : roi.width;
	newRoi.height = roi.y < 0 ? roi.height + roi.y : roi.height;

	newRoi.width = min(newRoi.width, src.cols - newRoi.x);
	newRoi.height = min(newRoi.height, src.rows - newRoi.y);

	return newRoi;
}