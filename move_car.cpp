#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
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

	const double dis_time_ratio = 1.8;
	const double turn_ratio = 0.07;
	double angle = atof(argv[1]) * turn_ratio;
	double forward_t = atof(argv[2]) * dis_time_ratio;

	cout << "turn time: " << abs(angle) << " forward time: " << forward_t << endl;

	//turn
	string cmd = "";
	if(atof(argv[1]) > 5) cmd = "move-50";
	else if(atof(argv[1]) < -5) cmd ="move50";
	
	send_to_arduino(cmd.c_str());	
	usleep(abs(angle) * 1000000);

//	send_to_arduino("stop");
	
	//forward
	send_to_arduino("move0");
	usleep(forward_t * 1000000);

	send_to_arduino("stop");

	return 0;
}
