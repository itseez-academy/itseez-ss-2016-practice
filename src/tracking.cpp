#include "tracking.hpp"

#include <iostream>

#include "opencv2/features2d.hpp"
#include "opencv2/video.hpp"

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
	else
	{
		cerr << "Failed to create tracker with name '" << name << "'"<< endl;
		return nullptr;
	}
}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi)
{
	return false;
}

Rect MedianFlowTracker::Track(const cv::Mat &frame)
{
	if (this->frame_.empty())
	{
		std::cout<<"Can't track without initialization";
		return Rect(-1, -1, 0, 0);
	}
	else
	{
		std::vector<KeyPoint> features;
		goodFeaturesToTrack(frame, features, 50, 0.5, 1);

		std::vector<uchar> status, errors;

		calcOpticalFlowPyrLK(this->frame_, frame, this->features_, features, status, errors);

		for (int i = 0, erased = 0; i < status.size(); i++)
			if (status[i] != 1) //no flow found
			{
				this->features_.erase(this->features_.begin() + i - erased);
				features.erase(features.begin() + i - erased);
				erased++;
			}

		std::vector<float> diffx, diffy;
		for (int i = 0; i < features.size(); i++)
		{
			Point2f oldPoint = this->features_.at(i).pt;
			Point2f newPoint = features.at(i).pt;

			diffx.push_back(newPoint.x - oldPoint.x);
			diffy.push_back(newPoint.y - oldPoint.y);
		}

		std::sort(diffx.begin(), diffx.end());
		std::sort(diffy.begin(), diffy.end());

		this->position_.x += diffx.at(diffx.size() / 2);
		this->position_.y += diffy.at(diffy.size() / 2);

		this->frame_ = frame;
		this->features_.resize(features.size);
		std::copy(features.begin(), features.end(), this->features_.begin());

		return Rect(this->position_);
	}
}