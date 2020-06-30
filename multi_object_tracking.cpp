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
    cap >> frame;
    vector<Rect> rois;
    selectROIs("rois", frame, rois,false);
    if (rois.size() < 1)
        return 0;
    MultiTracker trackers;
    vector<Rect2d> obj;
    vector<Ptr<Tracker>> algorithms;
    for (auto i = 0; i < rois.size(); i++) {
        obj.push_back(rois[i]);
        algorithms.push_back(TrackerKCF::create());
    }
    trackers.add(algorithms, frame, obj);
    while (cap.read(frame)) {
        bool ok = trackers.update(frame);
        if (ok) {
            for (auto j = 0; j < trackers.getObjects().size(); j++) {
                rectangle(frame, trackers.getObjects()[j], Scalar(255, 0, 0), 2, 1);
            }
            imshow("tracker", frame);
        }
        if (waitKey(1) == 27)break;
    }
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
	double new_height = 200;
	double new_width = new_height * r;
	resize(frame, new_frame, Size(new_width, new_height));
	return new_frame;
}