#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
	if (name == "cascade") {
		return std::make_shared<CascadeDetector>();
	}
  return nullptr;
}
 