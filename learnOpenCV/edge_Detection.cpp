//
//
//#include "opencv2/imgproc.hpp"
//#include "opencv2/highgui.hpp"
//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
///// Global variables
//
//Mat src, src_gray;
//Mat dst, detected_edges;
//
//int edgeThresh = 1;
//int lowThreshold;
//int const max_lowThreshold = 100;
//int ratio = 3;
//int kernel_size = 3;
//char* window_name = "Edge Map";
//
///**
//* @function CannyThreshold
//* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
//*/
//void CannyThreshold(int, void*)
//{
//	/// Reduce noise with a kernel 3x3
//	blur(src_gray, detected_edges, Size(3, 3));
//
//	/// Canny detector
//	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
//
//	/// Using Canny's output as a mask, we display our result
//	dst = Scalar::all(0);
//
//	src.copyTo(dst, detected_edges);
//	imshow(window_name, dst);
//}
//
//
///** @function main */
//int main(int argc, char** argv)
//{
//	/// Load an image
//	src = imread("org.jpg");
//
//	if (!src.data)
//	{
//		cout << "can't find the specific image." << endl;
//		return -1;
//	}
//
//	/// Create a matrix of the same type and size as src (for dst)
//	dst.create(src.size(), src.type());
//
//	/// Convert the image to grayscale
//	cvtColor(src, src_gray, COLOR_BGR2GRAY);
//
//	/// Create a window
//	namedWindow(window_name, WINDOW_AUTOSIZE);
//
//	/// Create a Trackbar for user to enter threshold
//	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
//
//	/// Show the image
//	CannyThreshold(0, 0);
//
//	/// Wait until user exit program by pressing a key
//	while (1) {
//		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//		{
//			cout << "esc key is pressed by user" << endl;
//			break;
//		}
//	}
//
//	return 0;
//}