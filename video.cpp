#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/dnn.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <windows.h>
#include <iostream>
#include <fstream>
 
using namespace cv;
using namespace cv::dnn;
using namespace std;

int main(){

	int time1;
	int seconds;
	int minutes;
	int hours;
	int hourshere;

		// Specify the paths for the 2 files
	//string protoFile = "pose_deploy_linevec_faster_4_stages.prototxt";
	//string weightsFile = "pose_iter_160000.caffemodel";
	string protoFile = "pose_deploy_linevec_faster_4_stages.prototxt";
	string weightsFile = "pose_iter_160000.caffemodel";


	//start video capture 
	VideoCapture cap(0);
	
	if (!cap.isOpened())
	{
		cout << "Error opening video stream" << endl;
		return -1;
	}

	//get frame size
	int inWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int inHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
		//Net net = readNetFromCaffe(protoFile, weightsFile);

	//sample to see how frame per second
	
	
	//set up video file to save recording
	VideoWriter video("outcpp.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(inWidth, inHeight));
	VideoWriter videoout("outcpp1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(inWidth, inHeight));

	//take 400 frames and save to outcpp.avi
	auto start = chrono::steady_clock::now();

	for (int x = 0; x < 400;x++)
		{
		Mat frame1;
		//Mat frame1;
		// Capture frame-by-frame 
		cap >> frame1;
		// If the frame is empty, break immediately
		//if (frame1.empty())
		//	break;

		// Display the resulting frame    
		//imshow("Frame", frame1);

		video.write(frame1);
		}
	auto end = chrono::steady_clock::now();
	cout << "durations" << chrono::duration_cast<chrono::milliseconds> (end - start).count() << "\n";
	double fps = 400 / (double)chrono::duration_cast<chrono::milliseconds> (end - start).count() * 1000;
	cout << "fps" << fps << "\n";

		// When everything done, release the video capture and write object
		cap.release();
		video.release();


		//now read saved recording
		VideoCapture cap1("outcpp.avi");

		// Check if camera opened successfully
		if (!cap1.isOpened()) {
			cout << "Error opening video stream or file" << endl;
			return -1;
		}
		//VideoWriter video1("outcpp1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(inWidth, inHeight));


		Mat flow, frame;
		// some faster than mat image container
		UMat  flowUmat, prevgray;
		float xnet;
		float ynet;
		
		//output net motion to outnet.txt
		ofstream myfile1;
		myfile1.open("outnet.txt");
		float zero = 0.0;
		myfile1 << (float)fps << " " << zero << "\n";
		Net net = readNetFromCaffe(protoFile, weightsFile);

		int toprange = 300;
		int bottomrange = 301;
		int leftrange = 200;
		int rightrange = 400;
		//look at each frame and compare it to previous frame
		for (;;)
		{
			bool Is = cap1.grab();
			if (Is == false) {
				// if video capture failed
				//cout << "Video Capture Fail" << endl;
				break;
			}
			else {

				float positionx=0;
				float positiony=0;

				Mat img;
				Mat original;
				
				// capture frame from video file
				cap1.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);

				resize(img, img, Size(640, 480));

				// save original for later
				img.copyTo(original);

				// just make current frame gray
				cvtColor(img, img, COLOR_BGR2GRAY);


				if (prevgray.empty() == false) {

					// calculate optical flow 
					calcOpticalFlowFarneback(prevgray, img, flowUmat, 0.4, 1, 12, 2, 8, 1.2, 0);
					// copy Umat container to standard Mat
					flowUmat.copyTo(flow);
					xnet = 0;
					ynet = 0;

					// By y += 10, x += 10 you can specify the grid 
					//location of interest
					for (int y = toprange; y < bottomrange; y += 5) {
						for (int x = leftrange; x < rightrange; x += 5)
						{
							// get the flow from y, x position * 10 for better visibility
							const Point2f flowatxy = flow.at<Point2f>(y, x) * 10;
							// draw line at flow direction
							line(original, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(255, 0, 0));
							// draw initial point
							circle(original, Point(x, y), 1, Scalar(0, 0, 0), -1);
							xnet += flowatxy.x;//net posion of desired area
							ynet += flowatxy.y;
						}
					}
				
					positionx += xnet;
					positiony += ynet;
					myfile1 << xnet << " " << ynet << "\n";
					// draw the results
					namedWindow("prew", WINDOW_AUTOSIZE);
					imshow("prew", original);

					// fill previous image again
					img.copyTo(prevgray);
					videoout.write(original);
				}
				else {

					toprange = 300;
					bottomrange = original.row;
					leftrange = 200;
					rightrange = 500;
					img.copyTo(prevgray);

				}


				int key1 = waitKey(20);
				
			}
		}
		auto last = chrono::steady_clock::now();
		cout << chrono::duration_cast<chrono::milliseconds> (last - start).count() << "\n";

		cap1.release();
		videoout.release();

		myfile1.close();
}
