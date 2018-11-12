#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/dnn.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>

#include <windows.h>
 
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
	string protoFile = "pose_deploy_linevec_faster_4_stages.prototxt";
	string weightsFile = "pose_iter_160000.caffemodel";

		// Read the network into Memory
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "Error opening video stream" << endl;
		return -1;
	}
	int inWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int inHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

		Net net = readNetFromCaffe(protoFile, weightsFile);
		VideoWriter video("outcpp.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(inWidth, inHeight));


		for (int x = 0; x < 100;x++)
		{
			Mat frame;
			//Mat frame1;
			// Capture frame-by-frame 
			cap >> frame;
			// If the frame is empty, break immediately
			if (frame.empty())
				break;

		// 
		//Mat frame = imread("single.jpg");
		//Mat frame1 = imread("single.jpg", CV_LOAD_IMAGE_COLOR);
		
		// Show our image inside it.
		
		// Specify the input image dimensions
		//int inWidth = 700;
		//int inHeight = 467;
		// Prepare the frame to be fed to the network
			/*
		Mat inpBlob = blobFromImage(frame, 1.0 / 255, Size(inWidth, inHeight), Scalar(0, 0, 0), false, false);

		// Set the prepared object as the input blob of the network
		net.setInput(inpBlob);
		Mat output = net.forward();
		

		int H = output.size[2];
		int W = output.size[3];
		// find the position of the body parts
		vector<Point> points(15);
		for (int n = 0; n < 15; n++) { // Probability map of corresponding body's part. 
			Mat probMap(H, W, CV_32F, output.ptr(0,n)); 
			Point2f p(-1,-1); 
			Point maxLoc; 
			double prob; 
			minMaxLoc(probMap, 0, &prob, 0, &maxLoc);// if (prob > thresh)
			
				p = maxLoc;
				p.x *= float(inWidth) / W;
				p.y *= float(inHeight) / H;
				printf("%lf %lf\n", p.x, p.y);
				circle(frame1,p,5,Scalar(0, 0, 255),-1,8);

			points[n] = p;
		}
		*/
		// Write the frame into the file 'outcpp.avi'
		video.write(frame);

		// Display the resulting frame    
		imshow("Frame", frame);
		time1 = (int)time(NULL);
		seconds = time1 % 60;
		time1 /= 60;

		minutes = time1 % 60;
		time1 /= 60;

		hours = time1 % 24;
		time1 /= 24;

		hourshere = hours - 8;
		if (hourshere < 0) {
			hourshere = 24 + hourshere;
		}

		Sleep(100);
		printf("H%d M%d  S%d  \n", hourshere, minutes, seconds);
		// Press  ESC on keyboard to  exit
		char c = (char)waitKey(1);
		if (c == 27)
			break;
		}

		// When everything done, release the video capture and write object
		cap.release();
		video.release();


		VideoCapture cap1("outcpp.avi");

		// Check if camera opened successfully
		if (!cap1.isOpened()) {
			cout << "Error opening video stream or file" << endl;
			return -1;
		}
		VideoWriter video1("outcpp1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(inWidth, inHeight));


		while (1) {

			Mat frame;
			Mat frame1;
			// Capture frame-by-frame
			cap1 >> frame;
			frame1 = frame;
			// If the frame is empty, break immediately
			if (frame.empty())
				break;
			Mat inpBlob = blobFromImage(frame, 1.0 / 255, Size(inWidth, inHeight), Scalar(0, 0, 0), false, false);

			// Set the prepared object as the input blob of the network
			net.setInput(inpBlob);
			Mat output = net.forward();


			int H = output.size[2];
			int W = output.size[3];
			// find the position of the body parts
			vector<Point> points(15);
			for (int n = 0; n < 15; n++) { // Probability map of corresponding body's part. 
				Mat probMap(H, W, CV_32F, output.ptr(0, n));
				Point2f p(-1, -1);
				Point maxLoc;
				double prob;
				minMaxLoc(probMap, 0, &prob, 0, &maxLoc);// if (prob > thresh)

				p = maxLoc;
				p.x *= float(inWidth) / W;
				p.y *= float(inHeight) / H;
				printf("%lf %lf\n", p.x, p.y);
				circle(frame1, p, 5, Scalar(0, 0, 255), -1, 8);

				points[n] = p;
			}

			// Display the resulting frame
			imshow("Frame", frame1);
			video1.write(frame1);

			// Press  ESC on keyboard to exit
			char c = (char)waitKey(25);
			if (c == 27)
				break;
		}

		// When everything done, release the video capture object
		cap1.release();

		// Closes all the frames
		destroyAllWindows();


		return 0;

		
		
}


	/*
	vector<Rect> body;
	CascadeClassifier body_cascade;
	body_cascade.load("haarcascade_upperbody.xml");

	Mat image;
	image = imread("waitingroom.jpg", CV_LOAD_IMAGE_COLOR);

	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}



	body_cascade.detectMultiScale(image, body, 1.01, 2, 0 | CASCADE_SCALE_IMAGE, Size(60, 60), Size(200, 200));

	if (body.size() > 0) {
		for (int gg = 0; gg < body.size(); gg++) {
			rectangle(image, body[gg].tl(), body[gg].br(), Scalar(0, 0, 255), 2, 8, 0);
		}
	}

	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", image);                   // Show our image inside it.

	waitKey(0);                                          // Wait for a keystroke in the window




  // Create a VideoCapture object and use camera to capture the video
  VideoCapture cap(0); 
 
  // Check if camera opened successfully
  if(!cap.isOpened())
  {
    cout << "Error opening video stream" << endl; 
    return -1; 
  } 
 
  // Default resolution of the frame is obtained.The default resolution is system dependent. 
  int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
  int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 

   
  // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
  VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height)); 


  while(1)
  { 
    Mat frame; 
     
    // Capture frame-by-frame 
    cap >> frame;
  
    // If the frame is empty, break immediately
    if (frame.empty())
      break;

	body_cascade.detectMultiScale(frame, body, 1.01, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	
	if (body.size() > 0) {
		for (int gg = 0; gg < body.size(); gg++) {
			rectangle(frame, body[gg].tl(), body[gg].br(), Scalar(0, 0, 255), 2, 8, 0);
		}
	}
	*/
	/*for (size_t i = 0; i < body.size(); i++)
	{
		Point center(body[i].x + body[i].width / 2, body[i].y + body[i].height / 2);
		ellipse(frame, center, Size(body[i].width / 2, body[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}*/
	/*
    // Write the frame into the file 'outcpp.avi'
    video.write(frame);
    
    // Display the resulting frame    
    imshow( "Frame", frame );
  
    // Press  ESC on keyboard to  exit
    char c = (char)waitKey(1);
    if( c == 27 ) 
      break;
  }
 
  // When everything done, release the video capture and write object
  cap.release();
  video.release();
 
  // Closes all the windows
  destroyAllWindows();
  return 0;
  */
