#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "RealTimeStitcher.h"
#include "TouchTracker.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	VideoCapture capA(0);
	if (!capA.isOpened())  // check if we succeeded
		return -1;

	VideoCapture capB(1);
	if (!capB.isOpened())  // check if we succeeded
		return -1;

	namedWindow("Display Image A", WINDOW_AUTOSIZE);
	namedWindow("Display Image B", WINDOW_AUTOSIZE);

	namedWindow("Result", WINDOW_AUTOSIZE);

	int myCount = 0;
	RealTimeStitcher sticher = RealTimeStitcher();
	bool paused = false;

	TouchTracker tracker;

	while (true)
	{
		int temp = waitKey(30);
		if( temp >= 0 )
			std::cout << temp << "\n";

		if (!paused)
		{
			Mat imageA;
			capA >> imageA;

			imshow("Display Image A", imageA);

			Mat imageB;
			capB >> imageB;

			imshow("Display Image B", imageB);

			Mat images[]{ imageA, imageB };
			cv::Size sizes[]{ Size((int)capA.get(CV_CAP_PROP_FRAME_WIDTH), (int)capA.get(CV_CAP_PROP_FRAME_HEIGHT)),
				Size((int)capB.get(CV_CAP_PROP_FRAME_WIDTH), (int)capB.get(CV_CAP_PROP_FRAME_HEIGHT))
			};

			if (sticher.CanStich())
			{
				Mat result = sticher.StichImages(images, sizes);
				tracker.UpdateTracking(result);
				imshow("Result", result);
			}

			if (temp == 1048608 || temp == 32)
			{
				sticher.CalculateStich(images, sizes, 0.75, 4.0, false);

				myCount++;
			}	
		}
		if (temp == 1048608 || temp == 32)
					{
		}
		else if (temp == 112)
		{
			paused = !paused;
		}
		else if( temp >= 0)
		{
			break;
		}
	}

	destroyAllWindows();

	/*
	if (argc != 2)
	{
		printf("No save path");
		return -1;
	}

	VideoCapture capA(0);
	if (!capA.isOpened())  // check if we succeeded
		return -1;

	VideoCapture capB(1);
	if (!capB.isOpened())  // check if we succeeded
		return -1;

	std::string fileA = argv[1];
	fileA.append("A");
	std::string fileB = argv[1];
	fileB.append("B");

	int frame_width = capA.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = capA.get(CV_CAP_PROP_FRAME_HEIGHT);

	VideoWriter outputVideoA(fileA.c_str(), CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height), true);
	VideoWriter outputVideoB(fileB.c_str(), CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height), true);

	if (!outputVideoA.isOpened() || !outputVideoB.isOpened())
	{
		return -1;
	}

	namedWindow("Display Image A", WINDOW_AUTOSIZE);
	namedWindow("Display Image B", WINDOW_AUTOSIZE);

	while (true)
	{
		Mat imageA;
		capA >> imageA;
		outputVideoA << imageA;

		// video.write(image);
		imshow("Display Image A", imageA);

		Mat imageB;
		capB >> imageB;
		outputVideoB << imageB;

		// video.write(image);
		imshow("Display Image B", imageB);


		if ( waitKey(30) >= 0 )
			break;
	}

	destroyAllWindows();
	return 0;
	*/
}
