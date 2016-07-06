#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

class Detector {
 public:
  static std::shared_ptr<Detector> CreateDetector(const std::string& name)
  {
	 // if (name == "cascade") {
	//	  return std::make_shared<CascadeDetector>();
	//  }
  }
  cv::CascadeClassifier CC;
  virtual bool Init(const std::string& model_file_path) = 0;
  virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                      std::vector<double>& scores) = 0;
};

class CascadeDetector : public Detector {
public:

	virtual bool Init(const std::string& model_file_path)
	{
		
		CC.load(model_file_path);
		return true;
	}
	virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
		std::vector<double>& scores)
	{
		
		//(const Mat& image, vector<Rect>& objects, double scaleFactor = 1.1, int minNeighbors = 3, int flags = 0, Size minSize = Size(), Size maxSize = Size())¶
		CC.detectMultiScale(frame, objects);// 1.1, 3, 0, cv::Size(3, 3), cv::Size(30, 30));
	}

protected:
	cv::CascadeClassifier detector;
};