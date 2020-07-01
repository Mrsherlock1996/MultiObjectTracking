#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/ocl.hpp>


using namespace std;
using namespace cv;
void showImage(Mat image, string name);
void addRois(vector<Rect> rois, vector<Rect2d> bboxes, vector<Ptr<Tracker>> trackers);
Mat resizeFrame(Mat frame);
string videoPath = "C:/Users/sherlock/Documents/multi-object-tracking/multi-object-tracking/videos/soccer_01.mp4";
int main() {
	VideoCapture cap;
	cap.open(videoPath);
	if (!cap.isOpened()) {
		cout << "no video open" << endl;
		return -1;
	}
	Mat frame;
	cap >> frame;
	frame = resizeFrame(frame);
	showImage(frame, "frame");
	MultiTracker multi_trackers;
	vector<Ptr<Tracker>> trackers;
	vector<Rect> rois;
	vector<Rect2d> boundingboxes;
	selectROIs("frame", frame, rois, false);
	for (auto i = 0; i < rois.size(); i++) {
		boundingboxes.push_back(rois[i]);
		trackers.push_back(TrackerMIL::create());
	}
	multi_trackers.add(trackers, frame, boundingboxes);
	char k = 'i';
	while (cap.read(frame)) {
		frame = resizeFrame(frame);
		multi_trackers.update(frame);
		for (int i = 0; i < multi_trackers.getObjects().size(); i++) {
			rectangle(frame, multi_trackers.getObjects()[i], Scalar(0, 0, 255), 1, 8);
		}
		showImage(frame, "frame");
		k = waitKey(1);
		if (k == 'q') {
			cout << "bye" << endl;
			break;
		}
		if (k == 's') {
			vector<Rect> addRois;
			vector<Rect2d> addBboxes;
			vector<Ptr<Tracker>> addTrackers;
			selectROIs("frame", frame, addRois, false);
			for (auto i = 0; i < addRois.size(); i++) {
				//如果这里依然采用上边的trackers会导致在multi_trackers.add时重复增加原来有的tracker,boundingboxes也是同理
				//trackers.push_back(TrackerMIL::create());  
				addBboxes.push_back(addRois[i]);
				addTrackers.push_back(TrackerMIL::create());

			}
			multi_trackers.add(addTrackers, frame, addBboxes);
		}
	}
	return 0;
}

void showImage(Mat image, string name) {
	imshow(name, image);
}
Mat resizeFrame(Mat frame) {
	Mat new_frame;
	double width = frame.cols;
	double height = frame.rows;
	double r = width / height;
	double new_height = 800;
	double new_width = new_height * r;
	resize(frame, new_frame, Size(new_width, new_height));
	return new_frame;
}

void addRois(vector<Rect> rois, vector<Rect2d> bboxes, vector<Ptr<Tracker>> trackers) {
	for (int i = 0; i < (rois).size(); i++) {
		bboxes.push_back(rois[i]);
		trackers.push_back(TrackerKCF::create());
	}
}