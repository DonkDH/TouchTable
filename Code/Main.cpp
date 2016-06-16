#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "ImageCorrection.h"
#include "TouchTracker.h"
#include "Utils.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
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
