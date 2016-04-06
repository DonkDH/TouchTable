#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "RealTimeStitcher.h"
#include "TouchTracker.h"
#include "CorrectPerspective.h"

using namespace cv;
using namespace std;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	((CorrectPerspective*)userdata)->CallBackFunc(event, x, y, flags);
}

int main(int argc, char** argv)
{
	VideoCapture capA("BFirst.avi");
	if (!capA.isOpened())  // check if we succeeded
		return -1;

	VideoCapture capB("AFirst.avi");
	if (!capB.isOpened())  // check if we succeeded
		return -1;

	namedWindow("Display Image A", WINDOW_AUTOSIZE);
	namedWindow("Display Image B", WINDOW_AUTOSIZE);

	namedWindow("Result", WINDOW_AUTOSIZE);

	int myCount = 0;
	RealTimeStitcher sticher = RealTimeStitcher();
	bool paused = false;

	TouchTracker tracker;
	CorrectPerspective* correctPers = new CorrectPerspective();

	while (true)
	{
		int temp = waitKey(30);
		if( temp >= 0 )
			std::cout << temp << "\n";

		if (!paused)
		{
			Mat imageA;
			capA >> imageA;

			Point2f center(imageA.cols / 2.0f, imageA.rows / 2.0f);
			Mat rotationMatrix = getRotationMatrix2D(center, 90.0f, 1.0f);
			Mat outA;
			//warpAffine(imageA, outA, rotationMatrix, imageA.size());
			imshow("Display Image A", imageA);

			Mat imageB;
			capB >> imageB;

			Mat imageCorrectedB = correctPers->UpdatePerspective(imageB);

			//warpAffine(imageB, imageB, rotationMatrix, imageB.size());

			imshow("Display Image B", imageB);

			cv::setMouseCallback("Display Image B", CallBackFunc, correctPers);
			imshow("Display Image B Corrected", imageCorrectedB);

			Mat images[]{ imageA, imageB };
				cv::Size sizes[]{ Size((int)imageA.cols, (int)imageA.rows),
				Size((int)imageB.cols, (int)imageB.rows)
			};

			if (sticher.CanStich())
			{
				Mat result = sticher.StichImages(images, sizes);

				Point2f center(result.cols / 2.0f, result.rows / 2.0f);
				Mat rotationMatrix = getRotationMatrix2D(center, -90.0f, 1.0f);
			//	warpAffine(result, result, rotationMatrix, result.size());
				
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
