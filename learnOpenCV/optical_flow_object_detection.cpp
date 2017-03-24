//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/bgsegm.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
//C++
#include <iostream>
#include <sstream>
#include <math.h>
#include <ctype.h>

using namespace std;
using namespace cv;


int main(int argc, const char** argv)
{

	// add your file name
	VideoCapture cap("video_input/stabilized_video_short_param800.avi");
	//VideoCapture cap(0);

	Mat flow, src;
	// some faster than mat image container
	UMat  flowUmat, prevgray;
	long skip = 0;
	while (cap.read(src))
	{
		skip++;
		if (skip % 2 == 0) {
			continue;
		}

		if (src.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		cv::Mat	frame = cv::Mat(src.rows / 2, src.cols / 2, CV_8UC3);
		cv::resize(src, frame, frame.size());

		Mat original;

		// save original for later
		frame.copyTo(original);

		

		// just make current frame gray
		cvtColor(frame, frame, COLOR_BGR2GRAY);


		// For all optical flow you need a sequence of images.. Or at least 2 of them. Previous                           //and current frame
		//if there is no current frame
		// go to this part and fill previous frame
		//else {
		// frame.copyTo(prevgray);
		//   }
		// if previous frame is not empty.. There is a picture of previous frame. Do some                                  //optical flow alg. 

		if (prevgray.empty() == false) {

			// calculate optical flow 
			//calcOpticalFlowFarneback(prevgray, frame, flowUmat, 0.4, 1, 12, 2, 8, 1.2, 0);

			// Parameter from OpenCV's website
			calcOpticalFlowFarneback(prevgray, frame, flowUmat, 0.5, 3, 15, 3, 5, 1.2, 0);

			// copy Umat container to standard Mat
			flowUmat.copyTo(flow);

			// make Mat for contourDetection
			//cv::Mat contourDetection;
			//Mat imgThresholded = cv::Mat(original.rows, original.cols, CV_8UC3, Scalar(0, 0, 0));;
			cv::Mat contourDetection = cv::Mat(original.rows, original.cols, CV_8UC1, Scalar(0));
			
			//Current rows is 540 and cols is 960
			// By y += 5, x += 5 you can specify the grid 
			double opticalflow_max = 0;
			for (int y = 0; y < original.rows; y += 5) {
				for (int x = 0; x < original.cols; x += 5)
				{
					if (y < 20 || y > 520 || x < 20 || x > 940)
						continue;
					// get the flow from y, x position * 10 for better visibility
					const Point2f flowatxy = flow.at<Point2f>(y, x) * 10;
					double res = sqrt(flowatxy.x*flowatxy.x + flowatxy.y*flowatxy.y);
					if (res > opticalflow_max)
						opticalflow_max = res;
					// draw line at flow direction
					if (res > 20) {


						line(original, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(255, 0, 0));
						// draw initial point
						circle(original, Point(x, y), 1, Scalar(0, 0, 0), -1);

						circle(contourDetection, Point(x, y), 15,  Scalar(255), -1);
					}

				}
			}
			//cout << " opticalflow_max : " << opticalflow_max << endl;


			Mat canny_output;
			vector<vector<Point> > contours;
			
			vector<Vec4i> hierarchy;
			int thresh = 100;
			RNG rng(12345);

			Mat extended(contourDetection.size() + Size(2, 2), contourDetection.type());
			Mat newContourDetection = extended(Rect(1, 1, contourDetection.cols, contourDetection.rows));

		//	imshow("newAreaDetected", newContourDetection);


			//inRange(imgThresholded , Scalar(254, 254, 254), Scalar(255, 255, 255), contourDetection); //Threshold the image

			/// Detect edges using canny
			Canny(contourDetection, canny_output, thresh, thresh * 3, 3);
			/// Find contours
			findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

			/*vector<vector<Point> > contours_poly(contours.size());
			for (int i = 0; i < contours.size(); i++)
			{
				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			}*/

			vector<vector<Point> >contours_poly(contours.size());
			for (int i = 0; i < contours.size(); i++)
			{
				convexHull(Mat(contours[i]), contours_poly[i], false);
			}


			/// Get the moments
			vector<Moments> mu(contours_poly.size());
			cout << "contours.size()" << contours_poly.size() << endl;
			for (size_t i = 0; i < contours_poly.size(); i++)
			{
				mu[i] = moments(contours_poly[i], false);
				cout << i << "  " << mu[i].m00 << endl;
			}

			///  Get the mass centers:
			vector<Point2f> mc(contours_poly.size());
			for (size_t i = 0; i < contours_poly.size(); i++)
			{
				mc[i] = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));
			}

			//vector<int> mcS(contours.size());
			//for (int i = 0; i < contours.size(); i++) {
			//	mcS[i] = contours[i].size();
			//	//cout << i << " mcS " << mcS[i] << endl;
			//}

			/// Draw contours
			Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
			for (size_t i = 0; i < contours_poly.size(); i++)
			{
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(drawing, contours_poly, (int)i, color, 2, 8, hierarchy, 0, Point());
				circle(drawing, mc[i], 4, color, -1, 8, 0);


				std::ostringstream areaSize;
				areaSize << mu[i].m00;
				//areaSize << mcS[i];
				putText(drawing, areaSize.str(), mc[i],
					FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(200, 200, 255), 1, CV_AA);

				if (mu[i].m00 > 8000) {
					std::ostringstream areaPos;
					areaPos << "pos: ( " << mc[i].x << "," << mc[i].y << " )" ;
					putText(drawing, areaPos.str(), Point2f(mc[i].x - 40 , mc[i].y + 10),
						FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(200, 200, 255), 0.5, CV_AA);

				}
			}



			namedWindow("prew", WINDOW_AUTOSIZE);
			imshow("prew", original);

			namedWindow("AreaDetected", WINDOW_AUTOSIZE);
			imshow("AreaDetected", contourDetection);

			namedWindow("Contour", WINDOW_AUTOSIZE);
			imshow("Contour", drawing);

			// fill previous image again
			frame.copyTo(prevgray);

			mu.clear();
			contours.clear();
			hierarchy.clear();

		}
		else {
			// fill previous image in case prevgray.empty() == true
			frame.copyTo(prevgray);

		}

		int key = waitKey(30);
		if (key == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
		else if (key == 32) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "space key is pressed by user" << endl;
			system("PAUSE");
		}
		
	}
	cap.release();

}




////////////////////////////////////////////////////////////////////////////////////////////////
//NEXT PART is about the Lucas-Kanade Sparse Optical Flow in OpenCV
//
//static void help()
//{
//	// print a welcome message, and the OpenCV version
//	cout << "\nThis is a demo of Lukas-Kanade optical flow lkdemo(),\n"
//		"Using OpenCV version " << CV_VERSION << endl;
//	cout << "\nIt uses camera by default, but you can provide a path to video as an argument.\n";
//	cout << "\nHot keys: \n"
//		"\tESC - quit the program\n"
//		"\tr - auto-initialize tracking\n"
//		"\tc - delete all the points\n"
//		"\tn - switch the \"night\" mode on/off\n"
//		"To add/remove a feature point click it\n" << endl;
//}
//
//Point2f point;
//bool addRemovePt = false;
//
//static void onMouse(int event, int x, int y, int /*flags*/, void* /*param*/)
//{
//	if (event == EVENT_LBUTTONDOWN)
//	{
//		point = Point2f((float)x, (float)y);
//		addRemovePt = true;
//	}
//}
//
//int main(int argc, char** argv)
//{
//
//	TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
//	Size subPixWinSize(10, 10), winSize(31, 31);
//
//	const int MAX_COUNT = 500;
//	bool needToInit = false;
//	bool nightMode = false;
//
//	help();
//
//	VideoCapture cap("video_input/stabilized_video_short_param800.avi");
//	//VideoCapture cap(0);
//
//	if (!cap.isOpened())
//	{
//		cout << "Could not initialize capturing...\n";
//		return 0;
//	}
//
//	namedWindow("LK Demo", 1);
//	setMouseCallback("LK Demo", onMouse, 0);
//
//	Mat gray, prevGray, image, frame, src;
//	vector<Point2f> points[2];
//
//	for (;;)
//	{
//		cap >> src;
//		if (src.empty())
//			break;
//
//		cv::Mat	frame = cv::Mat(src.rows / 2, src.cols / 2, CV_8UC3);
//		cv::resize(src, frame, frame.size());
//
//		frame.copyTo(image);
//		cvtColor(image, gray, COLOR_BGR2GRAY);
//
//		if (nightMode)
//			image = Scalar::all(0);
//
//		if (needToInit)
//		{
//			// automatic initialization
//			goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
//			cornerSubPix(gray, points[1], subPixWinSize, Size(-1, -1), termcrit);
//			addRemovePt = false;
//		}
//		else if (!points[0].empty())
//		{
//			vector<uchar> status;
//			vector<float> err;
//			if (prevGray.empty())
//				gray.copyTo(prevGray);
//			calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
//				3, termcrit, 0, 0.001);
//			size_t i, k;
//			for (i = k = 0; i < points[1].size(); i++)
//			{
//				if (addRemovePt)
//				{
//					if (norm(point - points[1][i]) <= 5)
//					{
//						addRemovePt = false;
//						continue;
//					}
//				}
//
//				if (!status[i])
//					continue;
//
//				points[1][k++] = points[1][i];
//				circle(image, points[1][i], 3, Scalar(0, 255, 0), -1, 8);
//			}
//			points[1].resize(k);
//		}
//
//		if (addRemovePt && points[1].size() < (size_t)MAX_COUNT)
//		{
//			vector<Point2f> tmp;
//			tmp.push_back(point);
//			cornerSubPix(gray, tmp, winSize, Size(-1, -1), termcrit);
//			points[1].push_back(tmp[0]);
//			addRemovePt = false;
//		}
//
//		needToInit = false;
//		imshow("LK Demo", image);
//
//		char c = (char)waitKey(10);
//		if (c == 27)
//			break;
//		switch (c)
//		{
//		case 'r':
//			needToInit = true;
//			break;
//		case 'c':
//			points[0].clear();
//			points[1].clear();
//			break;
//		case 'n':
//			nightMode = !nightMode;
//			break;
//		}
//
//		std::swap(points[1], points[0]);
//		cv::swap(prevGray, gray);
//	}
//
//	return 0;
//}
