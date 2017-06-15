

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

cv::Mat frameImage;         

int main(int argc, char** argv) {

	char		windowLiveStream[] = "LiveStream";

	cv::VideoCapture capWebcam(0);		// declare a VideoCapture object and associate to webcam, 0 => use 1st webcam
	capWebcam.set(CV_CAP_PROP_FRAME_WIDTH, 1440);
	capWebcam.set(CV_CAP_PROP_FRAME_HEIGHT, 1440);

	if (!capWebcam.isOpened()) {
		return -1;
	}

	while (1) { 
		
			capWebcam >> frameImage;
			cv::Mat compressedImage = cv::Mat(360, 360, CV_8UC3);
			cv::resize(frameImage, compressedImage, compressedImage.size());

			cv::Mat	rawImage = cv::Mat(480, 480, CV_8UC3);
			cv::Mat showImage = Mat(480, 480, CV_8UC3);

			cv::resize(frameImage, rawImage, rawImage.size());

			cv::copyMakeBorder(rawImage, showImage, 0, 0, 80, 80, BORDER_CONSTANT, Scalar(0, 0, 0));

			cv::imshow(windowLiveStream, showImage);
	
			int c = waitKey(30);
			if ((char)c == 27) { break; }
	}


	capWebcam.release();

	return 0;
}