#pragma once

#include <memory>
#include <string>
#include <opencv/cv.hpp>
#include "opencv2/core/core.hpp"

class Detector {
 public:
  static std::shared_ptr<Detector> CreateDetector(const std::string& name);
  virtual bool Init(const std::string& model_file_path) = 0;
  virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                      std::vector<double>& scores) = 0;
};

class CascadeDetector : public  Detector{
public:
    bool Init  (const std::string &model_file_path) override;
    void Detect(const cv::Mat &frame, std::vector<cv::Rect> &objects,
                std::vector<double> &scores) override;

protected:
    cv::CascadeClassifier detector;
};
