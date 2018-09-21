#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"


class Detector {
 public:
  static std::shared_ptr<Detector> CreateDetector(const std::string& name);
  virtual bool Init(const std::string& model_file_path) = 0;
  virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                      std::vector<double>& scores) = 0;
};


class CascadeDetector : public Detector {
protected:
	cv::CascadeClassifier detector;

public:
	 bool Init(const std::string& model_file_path) override;
	 void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
		std::vector<double>& scores) override;


};