/*
Created by Li and Jonathon
*/

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unordered_map>

using namespace cv;
using namespace std;

Point computeRectPoint(Point point);
float computeAngle(Point2f a, Point2f b, Point2f c);

struct DoublePoint
{
	Point a;
	Point b;
};

/** Global variables */
int imageWidth;
int imageHeight;
int diameter;
int radius;

int main(int argc, char **argv)
{

	Mat src,frame;

	Mat showImage(768, 768, CV_8UC3);
	Mat finalImage(768, 768, CV_8UC3);

	//Mat src = imread("equirect.jpg");
	cv::VideoCapture capWebcam(1);		// declare a VideoCapture object and associate to webcam, 0 => use 1st webcam
	capWebcam.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	capWebcam.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

	if (!capWebcam.isOpened()) {
		// The camera is not found

		return -1;
	}

	capWebcam >> src;

	if (!src.data)
	{
		cout << "can't find the specific image." << endl;
		return -1;
	}

	//resize(frame, src, Size(1024, 512));
	imageWidth = src.cols;

	imageHeight = src.rows * 0.9;

	cout << imageWidth << " " << imageHeight << endl;

	diameter = imageHeight * 2;
	radius = imageHeight;


	Mat dst(diameter, diameter, CV_8UC3, Scalar(0, 0, 0));

	list<DoublePoint> pointList;

	for (int x = 0; x < diameter; x++)
	{
		for (int y = 0; y < diameter; y++)
		{
			Point difference = Point(x, y) - Point(radius, radius);
			double distance = sqrt(difference.ddot(difference));

			if (distance > radius)
			{ //Using euclidean distance might be faster
			  //Put black color
				//dst.at<Vec3b>(Point(x, y)) = Vec3b(0, 0, 0);
			}
			else
			{
				Point rectPoint = computeRectPoint(Point(x, y));
				DoublePoint dP;
				dP.a = Point(x, y);
				dP.b = rectPoint;
				pointList.push_back(dP);
				//Use point to get color from rect image to new image
				
			}
		}
	}

	while (1) {

		capWebcam >> src;
		//resize(frame, src, Size(1024, 512));

		for (DoublePoint doublePoint : pointList)
		{
			Vec3b color = src.at<Vec3b>(Point(doublePoint.b));
			//cout << doublePoint.a.x << " " << doublePoint.a.y << endl;
			dst.at<Vec3b>(doublePoint.a) = color;
		}
	
		resize(dst, showImage, Size(768, 768));
		cv::copyMakeBorder(showImage, finalImage, 0, 0, 128, 128, BORDER_CONSTANT, Scalar(0, 0, 0));




	//	cv::imshow("src", src);
		cv::imshow("showImage", finalImage);

	//	cv::imwrite("srchhh.jpg", src);

	//	cv::imwrite("outputhhh.jpg", finalImage);

			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			{
				cout << "esc key is pressed by user" << endl;
				break;
			}
		

	}

	return 0;
}


Point computeRectPoint(Point point)
{
	Point centerPoint(radius, radius);
	Point zeroPoint(radius, 0);
	float angle = computeAngle(zeroPoint, centerPoint, point);
	if (angle < 0)
		angle += 360;
	float dist = cv::norm(point - centerPoint);
	int x = (angle / 360) * imageWidth;
	if (x == diameter)
		x--;
	if (x < 0)
		x = 0;
	if (dist == radius)
		dist--;
	if (dist < 0)
		dist = 0;
	int y = dist;
	return Point(x, y);
}

float computeAngle(Point2f a, Point2f b, Point2f c)
{
	//if (zeroVec.x != 0 && zeroVec.y != 0) {

	//	double len1 = sqrt(newVec.x*newVec.x + newVec.y*newVec.y);
	//	double len2 = sqrt(zeroVec.x*zeroVec.x + zeroVec.y*zeroVec.y);
	//	double direction = (newVec.cross(zeroVec) >= 0 ? 1.0 : -1.0);
	//	double dot = newVec.x * zeroVec.x + newVec.y * zeroVec.y;
	//	double a = dot / (len1 * len2);
	//	a = (a > 1.0) ? 1.0 : a;
	//	a = (a < -1.0) ? -1.0 : a;

	//	return (direction * acos(a) * 180.0 / 3.14159265358979323846); //Fix PI include

	//}

	Point2f ab;
	Point2f cb;

	ab.x = b.x - a.x;
	ab.y = b.y - a.y;

	cb.x = b.x - c.x;
	cb.y = b.y - c.y;

	float angba = atan2(ab.y, ab.x);
	float angbc = atan2(cb.y, cb.x);
	float rslt = angba - angbc;
	return (rslt * 180) / 3.141592;
}