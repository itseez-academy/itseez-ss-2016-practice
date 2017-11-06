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

void detectOnVideo( VideoCapture& capture, shared_ptr<Detector> detector){
    Mat frame;
    vector<Rect>   objects;
    vector<double> scores;
    while(true) {
        capture >> frame;
        try {
            detector->Detect(frame, objects, scores);
        }
        catch (char* msg){
            throw msg;
        }
        for(const auto& rect : objects){
            rectangle(frame, rect, Scalar(250, 150, 10));
        }
        imshow("capture", frame);
        int c = waitKey(33);
        if (c == 27) {
            break;
        }
        objects.clear();
        scores.clear();
    }

}

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("h")) {
    parser.printMessage();
    return 0;
  }
    std::shared_ptr<Detector> detector;
    try {
        detector = Detector::CreateDetector("cascade");
    }
    catch (const std::exception& e){
        cout << e.what() << endl;
    }

    std::vector<cv::Rect> objects;
    std::vector<double>  scores;

    std::string filePath;

   if(parser.has("m")){
        std::string filePathDetector;
        filePathDetector = parser.get<std::string>("m");
        detector->Init(filePathDetector);
    }
   else{
       cerr << "Error load model";
       return -1;
   }

   if(parser.has("i")){
       std::string filePath = parser.get<std::string>("i");;
       Mat input = imread(filePath);
       try {
           detector->Detect(input, objects, scores);
       }
       catch(char* msg){
           throw msg;
       }
    }
    else if(parser.has("v")){
        std::string filePath = parser.get<std::string>("v");;
        cv::VideoCapture video(filePath);
       try {
           detectOnVideo(video, detector);
       }
       catch (char* msg){
           cout << msg;
           video.release();
       }
        video.release();
    }
     else if(parser.has("c")){

       VideoCapture cap(0);
       if(!cap.isOpened())
           return -1;
       try {
           detectOnVideo(cap, detector);
       }
       catch (char* msg){
           cout << msg;
       }
       cap.release();
     }

    else{
        cerr << "no flag" << endl;
   }

  return 0;
}


