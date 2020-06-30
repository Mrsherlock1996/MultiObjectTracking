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
	if (!cap.isOpened()) {
		cout << "no video loaded" << endl;
		return - 1;
	}
	Mat frame;
	//cap.read(frame);
	//resizeFrame(frame);
	Ptr<MultiTracker> multi_trackers;
	vector<Ptr<Tracker>> trackers;
	vector<Rect2d> bboxes;
	//bboxes.push_back(selectROI("frame", frame));
	//trackers.push_back(TrackerKCF::create());
	//showImage(frame, "frame");
	char key = 'c';

	while (cap.read(frame)) {
		cap.read(frame);
		if (!cap.read(frame)) {
			cout << "no frame can be read" << endl;
			break;
		}
		frame = resizeFrame(frame);
		////multi_trackers->update(frame, bboxes);
		for (int i = 0; i < bboxes.size(); i++) {
			rectangle(frame, bboxes[i], Scalar(0, 0, 255), 1, 8);
		}
		showImage(frame, "frame");
		key = waitKey(100);
		if (key == 's') {
			bboxes.push_back(selectROI("frame", frame,1,0));
			trackers.push_back(TrackerKCF::create());
			multi_trackers->add(trackers.at(trackers.size()-1), frame, bboxes.at(bboxes.size()-1));

		}
		if (key == 'q') {
			break;
		}
	}
	cap.release();
	
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
	double new_height = 200;
	double new_width = new_height * r;
	resize(frame, new_frame, Size(new_width, new_height));
	return new_frame;
}