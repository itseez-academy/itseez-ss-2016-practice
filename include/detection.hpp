#pragma once

#include <memory>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/objdetect.hpp"

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
virtual bool Init(const std::string& model_file_path);
virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                    std::vector<double>& scores);

protected:
cv::CascadeClassifier detector;
};