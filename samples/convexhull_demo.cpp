#include <opencv2/opencv.hpp>

#include <iostream>
#include <memory>

using namespace std;
using namespace cv;

const char* kWindowName = "Video";
const int kEscapeKey = 27;
const int kSpeedLimit = 20;

const char* kOptions =
"{ @image         |    | video to process          }"
"{ d delay        | 25 | delay (in ms) between frames }"
"{ h ? help usage |    | print help message        }";

const char* kAbout = "This is OpenCV sample application.\n";

struct EventData {
  Point pressed_position;
  vector<Point> points_position;
  vector<Point> points_direction;
};

int GetZVector(Point a, Point b, Point c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void ConvexHull(vector<Point> &points, vector<size_t> &convex_hull) {
  const size_t size = points.size();
  if (size == 0)
    return;

  convex_hull.resize(size);

  for (int i = 0; i < convex_hull.size(); ++i)
    convex_hull[i] = i;

  auto min_max = minmax_element(points.begin(), points.begin() + (int)size,
    [](Point const& a, Point const& b) {
    return a.y < b.y;
  }
  );

  auto& min = *min_max.first;

  iter_swap(convex_hull.begin() + (min_max.first - points.begin()), convex_hull.begin());

  auto end_iter = remove_if(convex_hull.begin() + 1, convex_hull.end(),
    [&points, &min](const size_t id) {
    auto& point = points[id];
    if (point.x == min.x && point.y == min.y) {
      return true;
    }
    else {
      return false;
    }
  }
  );

  convex_hull.erase(end_iter, convex_hull.end());

  if (convex_hull.size() >= 3) {
    sort(convex_hull.begin() + 1, convex_hull.end(),
      [&points, &min](const size_t& id1, const size_t& id2) -> bool
    {
      Point a = Point(points[id1].x - min.x, points[id1].y - min.y);
      Point b = Point(points[id2].x - min.x, points[id2].y - min.y);

      if (a.x * b.x <= 0) {
        return a.x <= b.x;
      }
      else {
        if (a.x < 0) {
          return a.x * b.y < a.y * b.x;
        }
        else {
          return a.y * b.x > a.x * b.y;
        }
      }
    }
    );

    for (size_t i = 1; i < convex_hull.size() && convex_hull.size() > 2;) {
      size_t a = convex_hull[i - 1];
      size_t b = convex_hull[i];
      size_t c = convex_hull[(i + 1) % convex_hull.size()];

      if (GetZVector(points[a], points[b], points[c]) >= 0) {
        convex_hull.erase(convex_hull.begin() + (int)i);
        if (i != 1)
          --i;
      }
      else {
        i++;
      }
    }
  }
}

void Process(const Mat& in, Mat& out, const vector<Point> &points, 
    vector<size_t> &convex_hull) {
  out = in.clone();

  for (size_t i = 0; i < points.size(); ++i) {
    circle(out, points[i], 1.5, Scalar(255, 0, 0), 1);
    stringstream s;
    s << i;
    putText(out, s.str(), points[i], CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 0, 0));
  }

  for (size_t i = 0; i < convex_hull.size(); ++i) {
    auto point_start = convex_hull[i];
    auto point_end = convex_hull[(i + 1) % convex_hull.size()];
    circle(out, points[point_start], 2, Scalar(0, 0, 255), 2);
    line(out, points[point_start], points[point_end], Scalar(0, 0, 255));
    stringstream s;
    s << i;
    putText(out, s.str(), points[point_start], CV_FONT_HERSHEY_COMPLEX, 1., 
      Scalar(0, 255, 0));
  }
}

void ProcessWithFilter(const Mat& in, Mat& out, const vector<Point> &points, 
    vector<size_t> &convexHull) {
  const int lowerThreshold = 150;
  const int upperThreshold = 230;
  Mat gray, blur, equalize, edges, edgesColor,
    mask(in.rows, in.cols, CV_8U, Scalar(0));

  vector<Point> poly(convexHull.size());
  for (size_t i = 0; i < convexHull.size(); ++i)
    poly[i] = points[convexHull[i]];

  out = in.clone();

  cvtColor(in, gray, COLOR_BGR2GRAY);
  medianBlur(gray, blur, 3);
  equalizeHist(blur, equalize);
  Canny(equalize, edges, lowerThreshold, upperThreshold, 3, false);
  cvtColor(edges, edgesColor, COLOR_GRAY2BGR);
  fillConvexPoly(mask, poly.data(), convexHull.size(), Scalar(1));

  edgesColor.copyTo(out, mask);
}

bool UpdatePointPosition(Point &position, Point &direction, 
    const Size& frame_size) {
  bool has_collision = false;
  position.x += direction.x;
  position.y += direction.y;

  if (position.x < 0) {
    position.x = 0;
    direction.x = -direction.x;
    has_collision = true;
  }
  if (position.x >= frame_size.width) {
    position.x = frame_size.width - 1;
    direction.x = -direction.x;
    has_collision = true;
  }

  if (position.y < 0) {
    position.y = 0;
    direction.y = -direction.y;
    has_collision = true;
  }
  if (position.y >= frame_size.height) {
    position.y = frame_size.height - 1;
    direction.y = -direction.y;
    has_collision = true;
  }

  return has_collision;
}

void OnMouse(int event, int x, int y, int, void *data) {
  auto event_data = static_cast<EventData*>(data);

  if (event == EVENT_LBUTTONDOWN) {
    event_data->pressed_position.x = x;
    event_data->pressed_position.y = y;
  }
  else if (event == EVENT_LBUTTONUP) {
    int xSpeed = (x - event_data->pressed_position.x) / 4;
    if (xSpeed > kSpeedLimit) {
      xSpeed = kSpeedLimit;
    }
    else if (xSpeed < -kSpeedLimit) {
      xSpeed = -kSpeedLimit;
    }

    int ySpeed = (y - event_data->pressed_position.y) / 4;
    if (ySpeed > kSpeedLimit) {
      ySpeed = kSpeedLimit;
    }
    else if (ySpeed < -kSpeedLimit) {
      ySpeed = -kSpeedLimit;
    }

    event_data->points_position.emplace_back(x, y);
    event_data->points_direction.emplace_back(xSpeed, ySpeed);
  }
}

int main(int argc, const char** argv) {
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }
   
  size_t delay = parser.get<size_t>("delay");
  
  if (!parser.check()) {
    parser.printErrors();
    return 0;
  }

  namedWindow(kWindowName, WINDOW_AUTOSIZE);
  moveWindow(kWindowName, 0, 0);

  EventData data;
  setMouseCallback(kWindowName, OnMouse, &data);

  Mat frame = imread(parser.get<String>(0));
  int mode = 0;
  vector<size_t> convex_hull;

  while (true) {
    Mat out_image;
    for (size_t i = 0; i < data.points_position.size(); ++i)
      UpdatePointPosition(data.points_position[i], data.points_direction[i], frame.size());
    ConvexHull(data.points_position, convex_hull);
    switch (mode) {
    case 0:
      Process(frame, out_image, data.points_position, convex_hull);
      break;
    case 1:
      ProcessWithFilter(frame, out_image, data.points_position, convex_hull);
      break;
    };

    imshow(kWindowName, out_image);

    int key = waitKey(delay) & 0x00FF;
    if (key == kEscapeKey)
      break;
    if (key == ' ')
      mode = (mode + 1) % 2;
  }

  return 0;
}
