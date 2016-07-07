#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;
using namespace std;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) 
{
	if (name == "cascade") {
		return make_shared<CascadeDetector>();
	}
  cerr << "Failed to create detector with name '" << name << "'" << endl;
  return nullptr;
}

bool CascadeDetector::Init(const string & model_file_path)
{
	if (!detector.load(model_file_path))
	{ 
		printf("--(!)Error loading\n"); 
		return false; 
	}
	return true;
}

void CascadeDetector::Detect(const Mat & frame, vector<cv::Rect>& objects)//, vector<double>& scores)
{
	if (!detector.empty())
	{
		detector.detectMultiScale(frame, objects);
	}
}
