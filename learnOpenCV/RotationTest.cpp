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
//
//Mat rotate(Mat src, double angle) {
//	Mat dst;
//	Point2f pt(src.cols / 2., src.rows / 2.);
//	Mat r = getRotationMatrix2D(pt, angle, 1.0);
//	warpAffine(src, dst, r, Size(src.cols, src.rows));
//	return dst;
//}
//
//int
//main(int argc, char **argv)
//{
//
//	// (1)画像の読み込み，出力用画像領域の確保を行なう
//
//	Mat src = imread("org.jpg");
//
//	if (!src.data)
//	{
//		cout << "can't find the specific image." << endl;
//		return -1;
//	}
//
//	Mat dst;
//	dst = rotate(src, 20);
//
//	imshow("src", src);
//	imshow("dst", dst);
//
//	while (1) {
//		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//		{
//			cout << "esc key is pressed by user" << endl;
//			break;
//		}
//	}
//
//	return 0;
//
//}