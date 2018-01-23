#ifndef catch_cup_h
#define catch_cup_h

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

bool catch_cup(cv::VideoCapture& camera);
cv::Point get_cup_center(cv::Mat image);

#endif
