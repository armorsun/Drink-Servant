#include <string>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "send_to_arduino.h"

#define _USE_MATH_DEFINES

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		cout << "Not enough parameters!" << endl;
		return -1;
	}
	
	cout << "angle: " << argv[1] << " vertical dis: " << argv[2] << endl;

	const double dis_time_ratio = 5;
	const double turn_ratio = 4.5;
	double angle = atof(argv[1]) * turn_ratio;
	double forward_t = atof(argv[2]) * acos(atof(argv[1]) * M_PI / 180.0) * dis_time_ratio;

	cout << "turn: " << angle << " forward time: " << forward_t << endl;

	//turn
	string cmd = "move" + to_string(angle);
	send_to_arduino(cmd.c_str());
	sleep(3);
	
	//forward
	send_to_arduino("move0");
	sleep(forward_t);

	send_to_arduino("stop");

	return 0;
}
