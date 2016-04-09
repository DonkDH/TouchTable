#include "ImageCorrection.h"



ImageCorrection::ImageCorrection()
{
}


ImageCorrection::~ImageCorrection()
{
}

void ImageCorrection::Init()
{
	capA = OpenCapture("AFirst.avi", 0);
	capB = OpenCapture("BFirst.avi", 1);
}

void ImageCorrection::Update()
{

}

cv::VideoCapture ImageCorrection::OpenCapture(cv::String source, int camera, bool forceCamera = false)
{
	cv::VideoCapture cap;
	if (forceCamera)
	{
		cap = cv::VideoCapture(camera);
	}
	else
	{
		cap = cv::VideoCapture(source);
		if (!cap.isOpened())
		{
			cap = cv::VideoCapture(camera);
		}
	}

	return cap;
}