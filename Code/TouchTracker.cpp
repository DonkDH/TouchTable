#include "TouchTracker.h"



TouchTracker::TouchTracker()
{
}


TouchTracker::~TouchTracker()
{
}

void TouchTracker::UpdateTracking( cv::Mat inputImage )
{

}

cv::Mat TouchTracker::GenerateTrackingImage(cv::Mat inputImage)
{
	cv::Mat grayImage;
	cvtColor(inputImage, grayImage, CV_BGR2GRAY);

	if (grabBackground)
	{
		grayImage.copyTo(backgroundImage);
		grabBackground = false;
	}

	cv::absdiff(backgroundImage, grayImage, grayImage);

	cv::threshold(grayImage, grayImage, m_cutOffThreshhold, 255, cv::ThresholdTypes::THRESH_BINARY);

	return grayImage;
}

