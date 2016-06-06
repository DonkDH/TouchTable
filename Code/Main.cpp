#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "ImageCorrection.h"
#include"TouchTracker.h"

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
	int64 end = 0;
	while (true)
	{
		start = cv::getTickCount();
		int temp = waitKey(30);
		if( temp >= 0 )
			std::cout << temp << "\n";

        	imageCorrector.Update();

			if(updateTouches )
				touchTrackerA.UpdateTracking(imageCorrector.GetImageA());

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

		double time = ((double)cv::getTickCount() - start) / cv::getTickFrequency();
		std::cout << "\n\n\nFull Frame Time: " << time << "\n\n\n";
	}

	destroyAllWindows();
}
