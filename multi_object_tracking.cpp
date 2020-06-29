#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/ocl.hpp>


using namespace std;
using namespace cv;
void showImage(Mat image, string name);
Mat resizeFrame(Mat frame);
string videoPath = "C:/Users/sherlock/Documents/multi-object-tracking/multi-object-tracking/videos/soccer_01.mp4";
int main() {
	VideoCapture cap;
	cap.open(videoPath);
	Mat frame;
	cap.read(frame);
	frame = resizeFrame(frame);
	if (!cap.isOpened()) {
		cout << "no video loaded" << endl;
		return -1;
	}
	showImage(frame, "frame");

	Ptr<TrackerMIL> tracker = TrackerMIL::create();
	Rect2d bbox = selectROI("frame", frame);

	tracker->init(frame, bbox);
	while(cap.read(frame)) {
		frame = resizeFrame(frame);
		tracker->update(frame, bbox);
		rectangle(frame, bbox, Scalar(0, 0, 255),1);
		showImage(frame, "frame");
		if (waitKey(20) == 'q') {
			break;
		}
	}
	cap.release();
	return 0;
}

void showImage(Mat image, string name) {
	imshow(name, image);
	waitKey(1);
}
Mat resizeFrame(Mat frame) {
	Mat new_frame;
	double width = frame.cols;
	double height = frame.rows;
	double r = width / height;
	double new_height = 600;
	double new_width = new_height * r;
	resize(frame, new_frame, Size(new_width, new_height));
	return new_frame;
}