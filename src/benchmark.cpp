#include "benchmark.hpp"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <numeric>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

float DetectionQualityEvaluator::IntersectionOverUnion(const Rect& r,
                                                      const Rect& p) {
  float intersection_area = (r & p).area();
  float union_area = r.area() + p.area() - intersection_area;
  float iou = union_area > 0 ? intersection_area / union_area : 0.0f;
  return iou;
}

DetectionQualityEvaluator::DetectionQualityEvaluator() {
  num_objects_ = 0;
  num_objects_found_ = 0;
  num_responses_ = 0;
  num_false_alarms_ = 0;
  num_frames_ = 0;
  threshold_ = 0.5f;
}

void DetectionQualityEvaluator::UpdateMetrics(const vector<Rect>& guess,
                                             const vector<Rect>& ground_truth) {
  num_frames_++;
  num_objects_ += ground_truth.size();
  num_responses_ += guess.size();

  vector<bool> objects_detected(ground_truth.size(), false);
  vector<bool> correct_detections(guess.size(), false);

  for (size_t j = 0; j < guess.size(); ++j) {
    // Do not count any already matched detector alarm twice.
    if (correct_detections.at(j)) {
      continue;
    }
    const Rect& alarm = guess.at(j);
    for (size_t i = 0; i < ground_truth.size(); ++i) {
      // Do not allow several matches for one detector.
      if (objects_detected.at(i)) {
        continue;
      }
      const Rect& gt = ground_truth.at(i);
      if (IntersectionOverUnion(alarm, gt) >= threshold_) {
        objects_detected[i] = true;
        correct_detections[j] = true;
        break;
      }
    }
  }

  num_objects_found_ +=
      std::count(objects_detected.begin(), objects_detected.end(), true);
  num_false_alarms_ +=
      std::count(correct_detections.begin(), correct_detections.end(), false);
}

void DetectionQualityEvaluator::UpdateMetrics(const vector<Rect>& guess,
                                             const vector<double>& scores,
                                             const vector<Rect>& ground_truth) {
  if (guess.size() != scores.size()) {
    cerr << "Check failed 'guess.size() == scores.size()' (" << guess.size()
         << " vs " << scores.size() << ")." << endl;
    return;
  }
  // Sort detector alarm by scores.
  vector<size_t> idx(guess.size());
  iota(idx.begin(), idx.end(), 0);
  std::sort(idx.begin(), idx.end(),
            [&](size_t i, size_t j) { return scores.at(i) > scores.at(j); });
  vector<Rect> guess_sorted(guess.size());
  std::transform(idx.begin(), idx.end(), guess_sorted.begin(),
                 [&](size_t i) { return guess.at(i); });
  // Evaluate metrics.
  UpdateMetrics(guess_sorted, ground_truth);
}

float DetectionQualityEvaluator::GetDetectionRate() const {
  return num_objects_ == 0
             ? 0.0f
             : num_objects_found_ / static_cast<float>(num_objects_);
}

float DetectionQualityEvaluator::GetFalseAlarmRate() const {
  return num_responses_ == 0
             ? 0.0f
             : num_false_alarms_ / static_cast<float>(num_responses_);
}

GroundTruthReader::GroundTruthReader() {
  is_opened_ = false;
}

void GroundTruthReader::Open(const string& filename) {
  try {
    is_opened_ = file_storage_.open(filename, FileStorage::READ);
  } catch (exception&) {
    cerr << "Failed to read ground truth file." << endl;
    is_opened_ = false;
  }
  if (is_opened_) {
    file_node_ = file_storage_["objects"];
    CV_Assert(file_node_.isSeq());
    is_opened_ = !(file_node_.empty() || file_node_.isNone());
    if (is_opened_) {
      file_node_iter_ = file_node_.begin();
    }
  }
}

bool GroundTruthReader::Next(vector<Rect>& rects) {
  if (file_node_iter_ != file_node_.end()) {
    ++file_node_iter_;
  }
  return Get(rects);
}

bool GroundTruthReader::Get(vector<Rect>& rects) {
  rects.clear();
  if (!is_opened_ || file_node_iter_ == file_node_.end()) {
    return false;
  } else {
    CV_Assert((*file_node_iter_).isSeq());
    for (auto i : *file_node_iter_) {
      Rect rect;
      i >> rect;
      rects.emplace_back(rect);
    }
    return true;
  }
}

bool GroundTruthReader::IsOpen() const { return is_opened_; }
