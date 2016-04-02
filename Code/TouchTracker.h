#pragma once
#include <opencv2/opencv.hpp>

class TouchTracker
{
public:
	TouchTracker();
	~TouchTracker();

	void UpdateTracking( cv::Mat inputImage );

	cv::Mat GenerateTrackingImage(cv::Mat inputImage);
	
private:
	cv::Mat backgroundImage;
	bool grabBackground = true;

	double m_cutOffThreshhold = 20;
};

