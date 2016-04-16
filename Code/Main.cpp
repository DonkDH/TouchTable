#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
//#include "RealTimeStitcher.h"
//#include "TouchTracker.h"
//#include "CorrectPerspective.h"
#include "ImageCorrection.h"


using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    ImageCorrection imageCorrector = ImageCorrection();
    imageCorrector.Init();

    bool paused = false;
	while (true)
	{
		int temp = waitKey(30);
		if( temp >= 0 )
			std::cout << temp << "\n";

        	imageCorrector.Update();

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
}
