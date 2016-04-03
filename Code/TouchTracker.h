#pragma once
#include <opencv2/opencv.hpp>

class TouchTracker
{
public:
	TouchTracker();
	~TouchTracker();

	void UpdateTracking( cv::Mat inputImage );

private:
	cv::Mat GenerateTrackingImage(cv::Mat inputImage);
	void CalculateCurrentBlobs(cv::Mat inputImage, bool findHoles, bool useApproximation);

private:
	cv::Mat backgroundImage;
	bool grabBackground = true;

	double m_cutOffThreshhold = 40;
};

