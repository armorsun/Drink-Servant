#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <exception>
#include <unistd.h>
#include "catch_cup.h"
#include "send_to_arduino.h"

#define camera_Y_shift 50

using namespace std;
using namespace cv;

Point get_cup_center(Mat image);

bool catch_cup(VideoCapture &camera)
{
	try{
		Mat frame;
		camera.read(frame);
		Point target(frame.cols / 2, frame.rows / 2 + camera_Y_shift);
		cout << "target: " << target.x << " " << target.y;
		Point cup_coor(0, 0);
		int range = 5;
		int shift = 1;
		double ratio = 0.008;
		bool x_aimed = false, y_aimed = false;
		Point tmp_cup(0, 0);
		string cmd;

		while(!(x_aimed & y_aimed))
		{
			camera.read(frame);

			cup_coor = get_cup_center(frame);
			if(cup_coor.x == 0 && cup_coor.y == 0)
			{
				cup_coor.x = tmp_cup.x;
				cup_coor.y = tmp_cup.y;
			}
			tmp_cup = cup_coor;

			sleep(0.01);
	
			cout << "cup coor: " << cup_coor.x << " " << cup_coor.y << endl;
	
			if(!x_aimed)
			{
				if(cup_coor.x > target.x + range)
				{
					cmd = "armX" + to_string((cup_coor.x - target.x - range) * ratio + shift);
					send_to_arduino(cmd.c_str());
				}
				else if(cup_coor.x < target.x - range)
				{
					cmd = "armX" + to_string((cup_coor.x - target.x + range) * ratio - shift);
					send_to_arduino(cmd.c_str());
				}else
				{
					x_aimed = true;
					cout << "x aimed!" << endl;
				}
			}

			if(!y_aimed) //y coordinate in image coordinate is inverse!
			{
				if(cup_coor.y > target.y + range)
				{
					cmd = "armY" + to_string((target.y - range - cup_coor.y) * ratio - shift);
					send_to_arduino(cmd.c_str());
				}
				else if(cup_coor.y < target.y - range)
				{
					cmd = "armY" + to_string((target.y + range - cup_coor.y) * ratio + shift);
					send_to_arduino(cmd.c_str());
				}
				else
				{
					y_aimed = true;
					cout << "y aimed!" << endl;
				}
			}
		}

		if(x_aimed && y_aimed)
		{
			sleep(1);
			cmd = "armZ" + to_string(-15);
			send_to_arduino(cmd.c_str());

			sleep(1);
			cmd = "clawClose";
			send_to_arduino(cmd.c_str());

			sleep(10);

			cmd = "armZ" + to_string(15);
			send_to_arduino(cmd.c_str());

//			sleep(15);
//			cmd = "clawOpen";
//			send_to_arduino(cmd.c_str());
//			sleep(7);
			return true;
		}
	}
	catch(exception &e)
	{
		cout << "Standard exception: " << e.what() << endl;
		return false;
	}
	
}

Point get_cup_center(Mat image)
{
	Mat hsv, blue;
	Point center(0, 0);
	cvtColor(image, hsv, CV_BGR2HSV);
	inRange(hsv, Scalar(100, 100, 0), Scalar(140, 255, 255), blue);

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
	morphologyEx(blue, blue, MORPH_OPEN, element, Point(-1, -1), 1);
	morphologyEx(blue, blue, MORPH_CLOSE, element, Point(-1, -1), 1);
	vector<Vec3f> circles;

	HoughCircles(blue, circles, HOUGH_GRADIENT, 1, blue.rows/4, 70, 20, blue.rows/7.5, blue.rows/3);

#if DEBUG==1

	namedWindow("origin", WINDOW_NORMAL);
	namedWindow("blue", WINDOW_NORMAL);

	cout << circles.size() << " found" << endl;
	for(size_t i = 0; i < circles.size(); i++)
	{
		center.x = cvRound(circles[i][0]);
		center.y = cvRound(circles[i][1]);
		int radius = cvRound(circles[i][2]);
		circle(image, center, radius, Scalar(0, 0, 255), 5, LINE_AA, 0);
	}

	imshow("origin", image);
	if(circles.size() != 0)
		imshow("blue", blue);
	waitKey(1);

#endif

	if(circles.size() == 1)
	{
		center.x = cvRound(circles[0][0]);
		center.y = cvRound(circles[0][1]);
	}

	return center;
}
