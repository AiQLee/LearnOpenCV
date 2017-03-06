#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

Mat rotate(Mat src, double angle) {
	Mat dst;
	Point2f pt(src.cols / 2., src.rows / 2.);
	Mat r = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(src, dst, r, Size(src.cols, src.rows));
	return dst;
}

int main(int argc, char** argv)
{
	//VideoCapture cap(0); //capture the video from webcam
	VideoCapture cap("http://172.16.0.254:9176");

	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1440);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1440);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1280);

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 38;
	int iHighH = 75;

	int iLowS = 51;
	int iHighS = 155;

	int iLowV = 51;
	int iHighV = 155;

	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (38 - 75)
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (51 - 155)
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255);//Value (51 - 155)
	createTrackbar("HighV", "Control", &iHighV, 255);

	int iLastX = -1;
	int iLastY = -1;

	//Capture a temporary image from the camera
	Mat imgTmp;
	cap.read(imgTmp);

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;

	double angle = 0.0;

	while (true)
	{
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video



		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

																									  //morphological opening (removes small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgThresholded);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		// if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
		if (dArea > 100000)
		{
			//calculate the position of the ball
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			{
				//Draw a red line from the previous point to the current point
				cout << "posX " << posX << ";  " << "posY " << posY << endl;
				cout << "iLastX " << iLastX << ";  " << "iLastY " << iLastY << endl;
				//line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2);
			}
			double posChanged;
			if ( posY < imgOriginal.rows / 2) {
				if (posX - iLastX > 0) {
					 posChanged = sqrt( pow((posX - iLastX),2) + pow((posY - iLastY),2));
				}
				else {
					 posChanged = -sqrt(pow((posX - iLastX),2) + pow((posY - iLastY),2));
				}
			}
			else {
				if (posX - iLastX > 0) {
					 posChanged = -sqrt(pow((posX - iLastX),2) + pow((posY - iLastY),2));
				}
				else {
					 posChanged = sqrt(pow((posX - iLastX),2) + pow((posY - iLastY),2));
				}
			}

			if (iLastX != -1 && iLastY != -1) {
				if(abs(posChanged) > 10)
					angle = angle + posChanged;
			}

			cout << "angle " << angle  << endl;

			iLastX = posX;
			iLastY = posY;
		}

		imshow("Thresholded Image", imgThresholded); //show the thresholded image

		imgOriginal = rotate(imgOriginal, angle/5);

		//imgOriginal = imgOriginal + imgLines;

		cv::Mat	rawImage = cv::Mat(600, 800, CV_8UC3);
		cv::resize(imgOriginal, rawImage, rawImage.size());

		cv::Mat	resultImage = cv::Mat(600, 800, CV_8UC3);
		for (int i = 0; i < 800; i++) {
		rawImage.col(i).copyTo(resultImage.col(799-i));
		}

		imshow("Original", resultImage); //show the original image

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;
}