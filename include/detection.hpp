#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/objdetect.hpp"

class Detector {
 public:
  static std::shared_ptr<Detector> CreateDetector(const std::string& name);
  virtual bool Init(const std::string& model_file_path) = 0;
  virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                      std::vector<double>& scores) = 0;
};

class CascadeDetector : public Detector {
 public:
  virtual bool Init(const std::string& model_file_path);
  virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                      std::vector<double>& scores);

 protected:
  cv::CascadeClassifier detector;
};

class HOGDetector : public Detector {
 public:
  virtual bool Init(const std::string& model_file_path);
  virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                      std::vector<double>& scores);

 protected:
  void RemoveBorder(cv::Rect& box) const;
  cv::HOGDescriptor detector;
  bool do_use_meanshift_grouping_;
};
