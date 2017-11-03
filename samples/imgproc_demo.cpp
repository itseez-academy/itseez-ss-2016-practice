#include <iostream>
#include <string>
#include <opencv/cv.hpp>
#include <opencv2/core.hpp>
#include <image_processing.hpp>

using namespace std;
using namespace cv;

const char* kOptions =
                "{ @image         | <none> | image to process            }"
                "{ gray           |        | convert ROI to gray scale   }"
                "{ median         |        | apply median filter for ROI }"
                "{ edges          |        | detect edges in ROI         }"
                "{ pix            |        | pixelize ROI                }"
                "{ h ? help usage |        | print help message          }";
struct MouseCallbackState {
    bool is_selection_started;
    bool is_selection_finished;
    Point point_first;
    Point point_second;
} mouseCallbackState;
void onMouse(int event, int x, int y, int flag, void* param)
{
  if(event == cv::EVENT_LBUTTONDOWN)
  {
    mouseCallbackState.is_selection_started = true;
    mouseCallbackState.is_selection_finished = false;
    mouseCallbackState.point_first = cv::Point(x, y);
  }
  if(event == cv::EVENT_LBUTTONUP)
  {
    mouseCallbackState.is_selection_started = false;
    mouseCallbackState.is_selection_finished = true;
    mouseCallbackState.point_second = cv::Point(x, y);
  }
  if(event == cv::EVENT_MOUSEMOVE && !mouseCallbackState.is_selection_finished)
  {
    mouseCallbackState.point_second = cv::Point(x, y);
  }
}

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }


  // Do something cool.
  cv::namedWindow("input");
  cv::setMouseCallback("input", onMouse);
  std::string filePath;
  if (parser.has("@image")){
      filePath = parser.get<std::string>("@image");
  }
  cv::Mat input = cv::imread(filePath);
  cv::Mat input_copy;
  cv::Rect rect;
  while(true)
  {
      input.copyTo(input_copy);

      if(mouseCallbackState.is_selection_started && !mouseCallbackState.is_selection_finished) {
           rect = cv::Rect(mouseCallbackState.point_first.x, mouseCallbackState.point_first.y,
                           mouseCallbackState.point_second.x - mouseCallbackState.point_first.x,
                           mouseCallbackState.point_second.y - mouseCallbackState.point_first.y);
      }
      cv::rectangle(input_copy, rect, cv::Scalar(255, 0, 100));
      cv::imshow("input", input_copy);
      char c = cv::waitKey(33);
      if (c == 27)
        break;
  }
    ImageProcessorImpl processor;

    if(parser.has("gray")){
        input = processor.CvtColor(input, rect);
    }
    else if(parser.has("pix")) {
        input = processor.Pixelize(input, rect, 5);
    }
       else if(parser.has("edges")) {
        input = processor.DetectEdges(input, rect, 5, 2, 3, 5);
    }
    else if(parser.has("median")) {
        input = processor.Filter(input, rect, 5);
    }
    else{
        cerr << "no flag" << endl;
    }
    cv::imshow("input", input);
    cv::waitKey(0);

    return 0;
}
