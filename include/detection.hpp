#pragma once

#include <memory>
#include <string>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class Detector {
 public:
  static shared_ptr<Detector> CreateDetector(const string& name);
  virtual bool Init(const string& model_file_path) = 0;
  virtual void  Detect(const Mat& frame, vector<cv::Rect>& objects/*,vector<double>& scores*/) = 0;
};

class CascadeDetector : public Detector {
public:
	bool Init(const string& model_file_path);
	void  Detect(const Mat& frame, vector<cv::Rect>& objects);//,vector<double>& scores);

protected:
	CascadeClassifier detector;
};