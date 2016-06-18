#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "ImageCorrection.h"
#include "TouchTracker.h"
#include "Utils.h"

using namespace cv;
using namespace std;

void RecordVideo()
{
	cv::VideoCapture capA(0);
	cv::VideoCapture capB(1);
	cv::VideoWriter writerA;
	cv::VideoWriter writerB;

	if (!capA.isOpened() || !capB.isOpened() )
	{
		return;
	}

	
	bool isRecodring = false;
	cv::Mat frameA;
	cv::Mat frameB;

	while (true)
	{
		int temp = waitKey(1);

		capA >> frameA;
		capB >> frameB;

		cv::imshow("Video Recording A", frameA);
		cv::imshow("Video Recording B", frameB);

		if (isRecodring)
		{
			writerA << frameA;
			writerB << frameB;
		}

		if ((temp == 1048608 || temp == 32 ) && !isRecodring) // space bar
		{

			cv::Size S = cv::Size((int)capA.get(CV_CAP_PROP_FRAME_WIDTH), (int)capA.get(CV_CAP_PROP_FRAME_HEIGHT));

			int ex = static_cast<int>(capA.get(CV_CAP_PROP_FOURCC));
			writerA = cv::VideoWriter("AFirst.avi", ex-1, capA.get(CV_CAP_PROP_FPS), S, true);
			writerB = cv::VideoWriter("BFirst.avi", ex-1, capB.get(CV_CAP_PROP_FPS), S, true);

			isRecodring = true;
		}
		else if (temp == 115 || temp == 11048608) // S key
		{
			//save vids and close caps

			capA.release();
			capB.release();
			break;
		}
	}
}

int main(int argc, char** argv)
{
	if (true)
	{
		RecordVideo();
		return 0;
	}


    ImageCorrection imageCorrector = ImageCorrection();
    imageCorrector.Init();

	TouchTracker touchTrackerA = TouchTracker();
	TouchTracker touchTrackerB = TouchTracker();

    bool paused = false;
	bool updateTouches = false;
	int64 fpsTime = 0;
	int64 fpsTickCount = cv::getTickCount();
	int fps = 0;

	int64 autoStartTime = cv::getTickCount();

	while (true)
	{
		int temp = waitKey(1);

		if( temp >= 0 )
			std::cout << temp << "\n";

			Utils::StartTimer();
        	imageCorrector.Update();

			if (updateTouches)
			{
				touchTrackerA.UpdateTracking(imageCorrector.GetImageA(), "Tracked A");
				touchTrackerB.UpdateTracking(imageCorrector.GetImageB(), "Tracked B");
			}
			else
			{
				double current = ((double)cv::getTickCount() - autoStartTime) / cv::getTickFrequency();
				if (current > 2)
				{
					updateTouches = true;
				}
			}

		if (temp == 1048608 || temp == 32) // space bar
		{
			//imageCorrector.CalculateImageStitch();
			updateTouches = !updateTouches;
		}
		else if (temp == 115 || temp == 11048608) // S key
		{
			imageCorrector.SaveSettings();
		}
		else if (temp == 112 || 1048688) // P 
		{
			paused = !paused;
		}
		else if( temp == 113 || temp == 1048689) // Q
		{
			break;
		}

		fpsTime += ((double)cv::getTickCount() - fpsTickCount) / cv::getTickFrequency();
		++fps;
		if (fpsTime >= 1)
		{
			std::cout << "\nFPS: " << fps << "\n";
			fps = 0;
			fpsTime = 0;
			fpsTickCount = cv::getTickCount();
		}


	}

	destroyAllWindows();
}
