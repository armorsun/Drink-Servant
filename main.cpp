#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include "send_to_arduino.h"
#include "make_beverage.h"
#include "catch_cup.h"

#define camera_Y_shift 50

using namespace std;
using namespace cv;
	
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cout << "Not enough parameters" << endl;
	        return -1;
	}
	
	int front_camera_index = 1;
        VideoCapture front_camera(front_camera_index);
	Mat frame;
        while(!front_camera.isOpened())
        {
                cout << "cannot open camera";
                sleep(2);
        }

	front_camera.set(CAP_PROP_FRAME_WIDTH, 640);
	front_camera.set(CAP_PROP_FRAME_HEIGHT, 480);
	front_camera.set(CV_CAP_PROP_BUFFERSIZE, 2);

#if DEBUG == 1
	char check_esc = 0;
	Point center;
	while(check_esc != 27)
	{
		front_camera >> frame;
		center = get_cup_center(frame);
		check_esc = waitKey(1);
	}
#endif

	string cmd = "";
	bool catched = false;
	catched = catch_cup(front_camera);
	if(!catched) cout << "cup not catched!" << endl;

	sleep(2);
	cmd = "armHOME";
	send_to_arduino(cmd.c_str());

	sleep(3);
	cmd = "armY10";
	send_to_arduino(cmd.c_str());
	
	sleep(3);
	cmd = "armZ5";
	send_to_arduino(cmd.c_str());

	sleep(3);
	bool in_list = true; //0 water; 1 milk; 2 black tea; 3 milk tea
	in_list = make_beverage(atoi(argv[1]));
	if(!in_list) cout << "wrong drink option!" << endl;

	sleep(8);
	cmd = "clawOpen";
	send_to_arduino(cmd.c_str());

	sleep(10);
	cmd = "armHOME";
	send_to_arduino(cmd.c_str());
		
        return 0;
}
