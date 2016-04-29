#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
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
			imageCorrector.CalculateImageStitch();
		}
		else if (temp == 115)
		{
			imageCorrector.SaveSettings();
		}
		else if (temp == 112)
		{
			paused = !paused;
		}
		else if( temp == 113)
		{
			break;
		}
	}

	destroyAllWindows();
}
