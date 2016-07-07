#include "tracking.hpp"
#include "roi.hpp"

#include <iostream>

#include "opencv2/features2d.hpp"
#include "opencv2/video.hpp"
#include "opencv2/imgproc.hpp"

using std::string;
using std::shared_ptr;
using std::cerr;
using std::endl;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) 
{
	if (name == "median_flow") 
	{
		return std::make_shared<MedianFlowTracker>();
	}

	cerr << "Failed to create tracker with name '" << name << "'"<< endl;
	return nullptr;
}



std::vector<Point2f> generateFeatures(const cv::Mat &frame, const cv::Rect &roi)
{
	std::vector<Point2f> features;

	goodFeaturesToTrack(frame, features, 50, 0.15, 1.0);

	for (int i = 0; i < features.size(); i++)
	{
		features[i].x += roi.x;
		features[i].y += roi.y;
	}

	return features;
}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi)
{
	Mat toSaveGray, toSaveFormat;

	cvtColor(frame, toSaveGray, CV_BGR2GRAY);
	toSaveGray.convertTo(toSaveFormat, CV_8UC1);
	toSaveFormat.copyTo(this->frame_);

	Rect containedRoi = getContainedRoi(toSaveFormat, roi);
	std::vector<Point2f> features = generateFeatures(toSaveFormat(containedRoi), containedRoi);

	this->position_ = containedRoi;

	this->features_.resize(features.size());
	std::copy(features.begin(), features.end(), this->features_.begin());
	
	return !this->frame_.empty() && !features.empty();
}

Rect MedianFlowTracker::Track(const cv::Mat &frame)
{
	if (this->frame_.empty())
	{
		std::cout<<"Can't track without initialization";
		return Rect(-1, -1, 0, 0);
	}
	else if (this->features_.empty())
	{
		//object is lost
		return Rect(0, 0, 1, 1);
	}
	else
	{
		Mat grayFrame, grayEightBit;
		cv::cvtColor(frame, grayFrame, CV_BGR2GRAY);
		grayFrame.convertTo(grayEightBit, CV_8UC1);

		std::vector<Point2f> features;
		std::vector<uchar> status;
		Mat errors;

		calcOpticalFlowPyrLK(this->frame_, grayEightBit, this->features_, features, status, errors);

		for (int i = 0, erased = 0; i < status.size(); i++)
			if (status[i] != 1) //no optical flow found
			{
				this->features_.erase(this->features_.begin() + (i - erased));
				features.erase(features.begin() + (i - erased));
				erased++;
			}

		if (!this->features_.empty())
		{
			//we have something to track yet

			std::vector<float> diffx, diffy;
			for (int i = 0; i < features.size(); i++)
			{
				Point2f oldPoint = this->features_.at(i);
				Point2f newPoint = features.at(i);

				diffx.push_back(newPoint.x - oldPoint.x);
				diffy.push_back(newPoint.y - oldPoint.y);
			}

			std::sort(diffx.begin(), diffx.end());
			std::sort(diffy.begin(), diffy.end());

			this->position_.x += diffx.at(diffx.size() / 2);
			this->position_.y += diffy.at(diffy.size() / 2);

			grayEightBit.copyTo(this->frame_);

			this->features_.resize(features.size());
			std::copy(features.begin(), features.end(), this->features_.begin());
		}

		return Rect(this->position_);
	}
}