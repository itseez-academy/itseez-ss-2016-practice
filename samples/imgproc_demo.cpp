#include <iostream>
#include <string>
#include <opencv/cv.hpp>
#include <image_processing.hpp>

#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

struct MouseCallbackState {
    bool is_selection_started;
    bool is_selection_finished;
    Point point_first;
    Point point_second;
}mouseCallbackState;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
    "{ @image         |        | image to process                }"
    "{ gray           |        | convert ROI to gray scale       }"
    "{ median         |        | apply median filter for ROI     }"
    "{ edges          |        | detect edges in ROI             }"
    "{ pix            |        | pixelize ROI                    }"
    "{ h ? help usage |        | print help message              }";



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

  ImageProcessorImpl imageProcessor;
  // Do something cool.
  std::string filepath = parser.get<std::string>("@image");
  cv::Mat input_image  = cv::imread(filepath);
  cv::Mat copy_input_image;
  cv::Rect rect;
  cv::namedWindow("input_image");
  cv::setMouseCallback("input_image", onMouse);

  while(true){
      input_image.copyTo(copy_input_image);
      if(mouseCallbackState.is_selection_started)
          rect = cv::Rect(mouseCallbackState.point_first.x,
                  mouseCallbackState.point_first.y,
                  mouseCallbackState.point_second.x -
                  mouseCallbackState.point_first.x,
                  mouseCallbackState.point_second.y -
                  mouseCallbackState.point_first.y);

      cv::rectangle(copy_input_image, rect, cv::Scalar(0, 0, 255));
      cv::imshow("input_image", copy_input_image);
      int c = cv::waitKey(33);
      if(c == 27)
          break;
  }
  cv::Mat output_image;

  if(parser.has("gray")){
      output_image = imageProcessor.CvtColor(input_image, rect);
  }
  else if(parser.has("median")){
      output_image = imageProcessor.Filter(input_image, rect, 21 );
  }
  else if(parser.has("edges")){
      output_image = imageProcessor.DetectEdges(input_image, rect, 5, 5, 5, 5);
  }
  else if(parser.has("pix")){
      output_image = imageProcessor.Pixelize(input_image, rect, 5 );
  }
  else{
      cerr << "bad key" << endl;
      exit(-1);
  }

  cv::imshow("input_image", output_image);
  cv::waitKey(0);
    
  return 0;
}
