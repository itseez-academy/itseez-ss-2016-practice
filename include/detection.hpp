#pragma once

#include <memory>
#include <string>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
using namespace std;

class Detector {
 public:
  static std::shared_ptr<Detector> CreateDetector(const std::string& name);
  virtual bool Init(const std::string& model_file_path) = 0;
  virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                      std::vector<double>& scores) = 0;
};

class CascadeDetector : public Detector {
public:
	virtual bool Init(const std::string& model_file_path)

	{
		detector.load(model_file_path);
		if( (detector.load(model_file_path)) == true)
			return true;
		return false;
	};

	virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
		std::vector<double>& scores)
	{
		if (detector.empty()) {
			std::cerr << "No detector" << std::endl;
			return;
		}
		detector.detectMultiScale(frame, objects);
	};
		
protected:
	cv::CascadeClassifier detector;
};
