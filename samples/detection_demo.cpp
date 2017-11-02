#include <iostream>
#include "detection.hpp"


using namespace std;
using namespace cv;

const char* kOptions =
                "{ i image        | <none> | image to process                         }"
                "{ v video        | <none> | video to process                         }"
                "{ c camera       | <none> | camera to get video from                 }"
                "{ m model        | <none> | path to detector file                    }"
                "{ h ? help usage |        | print help message                       }";

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

    CascadeDetector detector;
            //("../test/test_data/detection/cascades/intel_logo_cascade.xml");
    detector.Init("/home/luba/github/itseez-ss-2016-practice/test/test_data/detection/cascades/unn_old_logo_cascade.xml");

    cv::Mat frame1;
    std::vector<cv::Rect> objects;
    std::vector<double>  scores;

    /*if(parser.has("v")){
      std::string filePath;
      filePath = parser.get("v");
      cv::VideoCapture video(filePath);

      while(true){
         video >> frame1;
         detector.Detect(frame1, objects, scores);
      }
    }
    else if(parser.has("i")){
        std::string filePath;
        filePath = parser.get("i");
        cv::Mat input = cv::imread(filePath);
        detector.Detect(input, objects, scores);
    }
    else if(parser.has("c")){

        CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY);
        assert( capture );
        IplImage* frame=0;
        cvNamedWindow("capture", CV_WINDOW_AUTOSIZE);
        printf("[i] press Esc for quit!\n\n");
        while(true) {// получаем кадр
            frame = cvQueryFrame(capture);
            detector.Detect(frame1, objects, scores);
            cvShowImage("capture", frame);
            char c = cvWaitKey(33);
            if (c == 27) { // нажата ESC
                break;
            }
        }
        cvReleaseCapture( &capture );
        cvDestroyWindow("capture");
    }
    else if(parser.has("m")){
        std::string filePathDetector;
        filePathDetector = parser.has("m");
        detector.Init( filePathDetector);
    }*/

    CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY);
    assert( capture );
    IplImage* frame=0;
    cvNamedWindow("capture", CV_WINDOW_AUTOSIZE);
    printf("[i] press Esc for quit!\n\n");
    while(true) {// получаем кадр
        frame = cvQueryFrame(capture);
        detector.Detect(frame1, objects, scores);
        cvShowImage("capture", frame);
        char c = cvWaitKey(33);
        if (c == 27) { // нажата ESC
            break;
        }
    }
        cvReleaseCapture( &capture );
        cvDestroyWindow("capture");



  return 0;
}


