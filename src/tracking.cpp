#include "tracking.hpp"

#include <iostream>
#include <opencv2/video/tracking.hpp>


#include "opencv2/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;


using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
	if (name == "median_flow") {
		return std::make_shared<MedianFlowTracker>();
	}

  std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  return nullptr;
}


bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Mat &next_frame, const cv::Rect &roi)
{
	vector<Point2f> corners;
	vector<Point2f> next_corners;

	std::vector<uchar> status;
	std::vector<float> err;
	
	int maxCorners = 150;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int maxTrackbar = 100;

	RNG rng(12345);

	Mat frame_gray;
	frame.copyTo(frame_gray);

	//convert to gray
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

	//set 8-bit, single-channel
	frame_gray.convertTo(frame_gray, CV_8UC1);

	//cut roi
	Mat cropped_frame = frame_gray(roi);
	imshow("Croped_frame", cropped_frame);
		
	// get features from roi
	goodFeaturesToTrack(cropped_frame, corners, maxCorners, qualityLevel, minDistance);
	
	/// corners detected
	cout << "** Number of corners detected: " << corners.size() << endl;
	
	waitKey(0);

	//////////////////////////////////
	///for next_frame
	/////////////////////////////////
	
	Mat next_frame_gray;
	next_frame.copyTo(next_frame_gray);

	//convert to gray
	cvtColor(next_frame, next_frame_gray, COLOR_BGR2GRAY);

	//set 8-bit, single-channel
	next_frame_gray.convertTo(next_frame_gray, CV_8UC1);

	//cut roi
	Mat next_cropped_frame = next_frame_gray(roi);

	// get features from roi
	goodFeaturesToTrack(next_cropped_frame, next_corners, maxCorners, qualityLevel, minDistance);
	
	/// corners detected
	cout << "** Number of corners on next frame detected: " << next_corners.size() << endl;

	waitKey(0);

	//////
	
		
	//count optical flow	
	calcOpticalFlowPyrLK(cropped_frame, next_cropped_frame, corners, next_corners, status, err);

	for (int i = 0; i < status.size(); i++) {
		//if (!status[i]) 
		//cout << "** status: " << status[i] << endl;
	}
	
	return true;
}

cv::Rect MedianFlowTracker::Track(const cv::Mat &frame)
{
	cv::Rect objects;
	return objects;
}