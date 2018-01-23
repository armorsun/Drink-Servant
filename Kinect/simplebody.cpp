// basic-http-client.cpp
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include "client.cpp"
#include <Kinect.h>
#define PI 3.14159265

using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
											// Creates an HTTP request and prints the length of the response stream.


											// output operator for CameraSpacePoint


int main(int argc, char** argv)
{
	
	cout << "Try to get default sensor" << endl;
	IKinectSensor* pSensor = nullptr;
	if (GetDefaultKinectSensor(&pSensor) != S_OK)
	{
		cerr << "Get Sensor failed" << endl;
		return -1;
	}

	
	cout << "Try to open sensor" << endl;
	if (pSensor->Open() != S_OK)
	{
		cerr << "Can't open sensor" << endl;
		return -1;
	}

	// 2a. Get frame source
	cout << "Try to get body source" << endl;
	IBodyFrameSource* pFrameSource = nullptr;
	if (pSensor->get_BodyFrameSource(&pFrameSource) != S_OK)
	{
		cerr << "Can't get body frame source" << endl;
		return -1;
	}

	// 2b. Get the number of body
	INT32 iBodyCount = 0;
	if (pFrameSource->get_BodyCount(&iBodyCount) != S_OK)
	{
		cerr << "Can't get body count" << endl;
		return -1;
	}
	cout << " > Can trace " << iBodyCount << " bodies" << endl;
	IBody** aBody = new IBody*[iBodyCount];
	for (int i = 0; i < iBodyCount; ++i)
		aBody[i] = nullptr;

	// 3a. get frame reader
	cout << "Try to get body frame reader" << endl;
	IBodyFrameReader* pFrameReader = nullptr;
	if (pFrameSource->OpenReader(&pFrameReader) != S_OK)
	{
		cerr << "Can't get body frame reader" << endl;
		return -1;
	}

	// 2b. release Frame source
	cout << "Release frame source" << endl;
	pFrameSource->Release();
	pFrameSource = nullptr;

	// Enter main loop
	int iStep = 0;
	int send = 0;
	while (iStep==0)
	{
		// 4a. Get last frame
		IBodyFrame* pFrame = nullptr;
		if (pFrameReader->AcquireLatestFrame(&pFrame) == S_OK)
		{

			// 4b. get Body data
			if (pFrame->GetAndRefreshBodyData(iBodyCount, aBody) == S_OK)
			{
				int iTrackedBodyCount = 0;

				// 4c. for each body
				for (int i = 0; i < iBodyCount; ++i)
				{
					IBody* pBody = aBody[i];

					// check if is tracked
					BOOLEAN bTracked = false;
					if ((pBody->get_IsTracked(&bTracked) == S_OK) && bTracked)
					{
						++iTrackedBodyCount;
						cout << "User " << i << " is under tracking" << endl;

						// get joint position
						Joint aJoints[JointType::JointType_Count];
						if (pBody->GetJoints(JointType::JointType_Count, aJoints) != S_OK)
						{
							cerr << "Get joints fail" << endl;
						}

						// get joint orientation
						JointOrientation aOrientations[JointType::JointType_Count];
						if (pBody->GetJointOrientations(JointType::JointType_Count, aOrientations) != S_OK)
						{
							cerr << "Get joints fail" << endl;
						}

						// output information
						JointType rJointType = JointType::JointType_HandRight;
						JointType lJointType = JointType::JointType_HandRight;
						JointType rsJointType = JointType::JointType_ShoulderRight;
						JointType lsJointType = JointType::JointType_ShoulderLeft;
						JointType hJointType = JointType::JointType_Head;
						const Joint& rJointPos = aJoints[rJointType];
						const Joint& lJointPos = aJoints[lJointType];
						const Joint& rsJointPos = aJoints[rsJointType];
						const Joint& lsJointPos = aJoints[lsJointType];
						const Joint& hJointPos = aJoints[hJointType];
						float distance, x, y, z,angle;
						


						if (rJointPos.TrackingState == TrackingState_NotTracked || hJointPos.TrackingState == TrackingState_NotTracked)
						{
							cout << "not tracked" << endl;
						}
						else
						{
							x = hJointPos.Position.X;
							z = hJointPos.Position.Z;
							if (rJointPos.Position.Y > hJointPos.Position.Y)
							{
								if (send == 0) {
									cout << "righthand lift" << endl;
									distance = sqrt(pow(x, 2) + pow(z, 2));
									angle = atan2(x, z) * 180 / PI;
								
									cout << "d" << distance << "a" << angle << endl;
								

									HTTPRequestCustomQueryAsync(distance, angle).wait();
									send = 1;
								}
								


							}

						}

						if (lJointPos.TrackingState == TrackingState_NotTracked || hJointPos.TrackingState == TrackingState_NotTracked)
						{
							cout << "not tracked" << endl;
						}
						else
						{
							x = hJointPos.Position.X;
							z = hJointPos.Position.Z;
							if (lJointPos.Position.Y > hJointPos.Position.Y)
							{
								if (send == 0) {
									cout << "lefthand lift" << endl;
									distance = sqrt(pow(x, 2) + pow(z, 2));
									angle = atan2(x, z) * 180 / PI;
								
									cout << "d" << distance << "a" << angle << endl;
								
									HTTPRequestCustomQueryAsync(distance, angle).wait();
									send = 1;
								}
								
							}

						}

					}
				}

				if (iTrackedBodyCount > 0)
					cout << "Total " << iTrackedBodyCount << " bodies in this time\n" << endl;
			}
			else
			{
				cerr << "Can't read body data" << endl;
			}

			// 4e. release frame
			pFrame->Release();
		}
	}

	// delete body data array
	delete[] aBody;

	// 3b. release frame reader
	cout << "Release frame reader" << endl;
	pFrameReader->Release();
	pFrameReader = nullptr;

	// 1c. Close Sensor
	cout << "close sensor" << endl;
	pSensor->Close();

	// 1d. Release Sensor
	cout << "Release sensor" << endl;
	pSensor->Release();
	pSensor = nullptr;

	return 0;
}