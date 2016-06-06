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
	

    bool paused = false;
	bool updateTouches = false;
	int64 start = 0;
	int64 autoStartTime = cv::getTickCount();
	int fps = 0;
	Utils::StartTimer();
	while (true)
	{
		int temp = waitKey(30);
		if( temp >= 0 )
			std::cout << temp << "\n";

        	imageCorrector.Update();

			if(updateTouches )
				touchTrackerA.UpdateTracking(imageCorrector.GetImageA());
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

		start += Utils::GetTime();
		++fps;
		if (start >= 1)
		{
			std::cout << "FPS: " << fps << "\n";
			fps = 0;
			start = 0;
			Utils::StartTimer();
		}
	}

	destroyAllWindows();
}
