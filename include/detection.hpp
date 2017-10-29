#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>


using namespace std;
using namespace cv;

class Detector {
 public:
  static std::shared_ptr<Detector> CreateDetector(const std::string& name){
      if (name == "cascade") {
          return std::make_shared<CascadeDetector>();
      }
  }
  virtual bool Init(const std::string& model_file_path) = 0;
  virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                      std::vector<double>& scores) = 0;
};

class CascadeDetector : public Detector {
public:
    virtual bool Init(const std::string& model_file_path){
            detector.load(model_file_path);
        if (detector.empty() ) {  //проверяет: загружен ли классификатор
            return false;
        }
        else
            return  true;
    }
    virtual void Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
                        std::vector<double>& scores){
        if (detector.empty() ) {  //проверяет: загружен ли классификатор
            detector.detectMultiScale(frame, objects);
         //   scores[0] = objects.size();
            ///????
        } else return;

    }

protected:
    cv::CascadeClassifier detector;
};

