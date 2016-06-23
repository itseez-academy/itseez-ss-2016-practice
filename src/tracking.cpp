#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
  std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  return nullptr;
}
