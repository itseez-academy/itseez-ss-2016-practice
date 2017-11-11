#include <iostream>
#include <string>
#include <opencv/cv.hpp>
#include <tracking.hpp>

#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
    "{ v video        |        | video to process         }"
    "{ h ? help usage |        | print help message       }";


struct MouseCallbackState {
    bool is_selection_started;
    bool is_selection_finished;
    Point point_first;
    Point point_second;
}mouseCallbackState;

void onMouse(int event, int x, int y, int flags, void* userdata){
    if(event == cv::EVENT_LBUTTONDOWN){
        mouseCallbackState.is_selection_started  = true;
        mouseCallbackState.is_selection_finished = false;
        mouseCallbackState.point_first.x = x;
        mouseCallbackState.point_first.y = y;
    }
    if(event == cv::EVENT_LBUTTONUP){
        mouseCallbackState.is_selection_started  = false;
        mouseCallbackState.is_selection_finished = true;
        mouseCallbackState.point_second.x = x;
        mouseCallbackState.point_second.y = y;
    }
    if(event == cv::EVENT_MOUSEMOVE){
        if(!mouseCallbackState.is_selection_finished){
            mouseCallbackState.point_second.x = x;
            mouseCallbackState.point_second.y = y;
        }
    }
};


int main(int argc, const char** argv) {
    // Parse command line arguments.
    CommandLineParser parser(argc, argv, kOptions);
    parser.about(kAbout);

    // If help option is given, print help message and exit.
    if (parser.get<bool>("help")) {
        parser.printMessage();
        return 0;
    }

    cv::VideoCapture video("/home/tolik/myWorkSpace/itseez-ss-2016-practice/test/test_data/video/logo.mp4");
    cv::namedWindow("window");
    cv::setMouseCallback("window", onMouse);
    cv::Rect object;
    cv::Mat frame;
    video >> frame;
    cv::Mat copy_frame;
    while(true){
        frame.copyTo(copy_frame);
        if(mouseCallbackState.is_selection_started)
            object = cv::Rect(mouseCallbackState.point_first.x,
                              mouseCallbackState.point_first.y,
                              mouseCallbackState.point_second.x -
                              mouseCallbackState.point_first.x,
                              mouseCallbackState.point_second.y -
                              mouseCallbackState.point_first.y);

        cv::rectangle(copy_frame, object, cv::Scalar(0, 255, 0));
        cv::imshow("window", copy_frame);
        int c = cv::waitKey(33);
        if(c == 27)
            break;
    }
    auto tracker = Tracker::CreateTracker("median_flow");
    tracker->Init(frame, object);

    while(true){
        video >> frame;
        cv::Rect roi = tracker->Track(frame);
        std::cout << roi << endl;
        cv::rectangle(frame, roi, cv::Scalar(0, 255, 0));
        cv::imshow("window", frame);
        int c = cv::waitKey(33);
        if(c == 27)
            break;
    }
    return 0;
}
