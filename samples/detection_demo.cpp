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
  if (parser.get<bool>("h")) {
    parser.printMessage();
    return 0;
  }

    CascadeDetector detector;

    std::vector<cv::Rect> objects;
    std::vector<double>  scores;

    std::string filePath;

   if(parser.has("m")){
        std::string filePathDetector;
        filePathDetector = parser.get<std::string>("m");
        detector.Init(filePathDetector);
    }
    
   if(parser.has("i")){
        std::string filePath;
        filePath = parser.get<std::string>("i");
        Mat input = imread(filePath);
        detector.Detect(input, objects, scores);
    }
    else if(parser.has("v")){
        Mat frame;
        std::string filePath;
        filePath = parser.get<std::string>("v");
        cv::VideoCapture video(filePath);

        while(true) {
            video >> frame;
            detector.Detect(frame, objects, scores);
            for(const auto& rect : objects){
                rectangle(frame, rect, Scalar(250, 150, 10));
            }
            imshow("capture", frame);
            char c = waitKey(33);
            if (c == 27) {
                break;
            }
            objects.clear();
        }
        video.release();
    }
     else if(parser.has("c")){

       VideoCapture cap(0);
       if(!cap.isOpened())
           return -1;
       Mat frame;
       if (detector.Init(parser.get<std::string>("m"))) {
           while (true) {
               cap >> frame;
               detector.Detect(frame, objects, scores);
               for (const auto &rect : objects) {
                   rectangle(frame, rect, Scalar(250, 150, 10));
               }
               imshow("capture", frame);
               char c = waitKey(33);
               if (c == 27) {
                   break;
               }
               objects.clear();
           }
           cap.release();
       }
       else{
           cerr << "Error load model";
           return -1;
       }
   }
    else{
        cerr << "no flag" << endl;
   }

  return 0;
}


