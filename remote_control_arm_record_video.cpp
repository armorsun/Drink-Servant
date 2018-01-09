#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include "send_to_arduino.h"

#define NUM_THREADS 2

using namespace std;
using namespace cv;

struct thread_data {
   	string name;
};

static void help()
{
    cout << "./video-write inputvideoName " << endl;
}

void *control_car(void *)
{
    	struct termios old_tio, new_tio;
    	unsigned char c;
	const char moveL[] = "armL";
	const char moveR[] = "armR";
	const char moveF[] = "armF";
	const char moveB[] = "armB";
	const char ascend[] = "armAscend";
	const char descend[] = "armDescend";

    	/* get the terminal settings for stdin */
   	tcgetattr(STDIN_FILENO,&old_tio);

    	/* we want to keep the old setting to restore them a the end */
    	new_tio=old_tio;

    	/* disable canonical mode (buffered i/o) and local echo */
    	new_tio.c_lflag &=(~ICANON & ~ECHO);

    	/* set the new settings immediately */
    	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

    	do {
		c=getchar();
		switch(c)
		{
			case 'i':
				send_to_arduino(moveF);
				printf("forward\n");
				break;
			case 'j':
				send_to_arduino(moveL);
				printf("turn left\n");
				break;
			case 'l':
				send_to_arduino(moveR);
				printf("turn right\n");
				break;
			case 'k':
				send_to_arduino(moveB);
				printf("move backward\n");
				break;
			case 'u':
				send_to_arduino(ascend);
				printf("ascend\n");
				break;
			case 'o':
				send_to_arduino(descend);
				printf("descend\n");
				break;
		}
    	} while(c!='q');

    	/* restore the former settings */
    	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
	pthread_exit(NULL);
}

void *record_video(void *thread_arg)
{
	struct thread_data *data;
	data = (struct thread_data *) thread_arg;
	string video_name = data->name + ".avi";
	VideoCapture camera(1);              // Open input
	
	camera.set(CAP_PROP_EXPOSURE, false);
	camera.set(CV_CAP_PROP_BUFFERSIZE, 5);
	camera.set(CAP_PROP_FPS, 30);
	camera.set(CAP_PROP_FRAME_WIDTH, 640);
	camera.set(CAP_PROP_FRAME_HEIGHT, 480);

	if (!camera.isOpened())
    	{
        	cout  << "Could not open the camera " << endl;
		pthread_exit(NULL);
    	}

	
	int frame_width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);

	VideoWriter video( video_name, CV_FOURCC('M', 'J', 'P', 'G'),  30, Size(frame_width,frame_height), true);

	if (!video.isOpened())
    	{
        	cout  << "Could not open the output video for write: " << data->name << endl;
		pthread_exit(NULL);
    	}
	
	Mat frame;
	char checkForEscKey = 0;
	
	while(checkForEscKey != 27)
	{
		camera >> frame;
		video << frame;
		imshow( "Frame", frame );
		checkForEscKey  = waitKey(1);
	}	
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	help();

	if(argc != 2)
	{
		cout << "Not enough parameters" << endl;
	        return -1;
	}

	struct thread_data video_data;
	video_data.name = string(argv[1]);	

	pthread_t threads[NUM_THREADS];
	pthread_create(&threads[0], NULL, control_car, NULL);
	pthread_create(&threads[1], NULL, record_video, (void *)&video_data);

	pthread_exit(NULL);
}
