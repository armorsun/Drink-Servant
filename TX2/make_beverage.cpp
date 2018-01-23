#include <iostream>
#include <stdlib.h>
#include <string>
#include "send_to_arduino.h"
#include "make_beverage.h"

using namespace std;

bool make_beverage(int b_type)
{
	switch(b_type){
		case 0: //water
			send_to_arduino("pump0");
			return true;
			break;
		case 1: //milk
			send_to_arduino("pump1");
			return true;
			break;
		case 2: //black tea
			send_to_arduino("pump2");
			return true;
			break;
		case 3: //milk tea
			send_to_arduino("pump3");
			return true;
			break;
		default:
			cout << "wrong option!" << endl;
			return false;
	}
}
