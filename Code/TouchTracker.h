#pragma once
#include <opencv2/opencv.hpp>
#include "json.hpp"
#include "Utils.h"

class TouchTracker
{
public:
	TouchTracker();
	~TouchTracker();

	void UpdateTracking( cv::Mat inputImage );

private:
	cv::Mat GenerateTrackingImage(cv::Mat inputImage);
	void CalculateCurrentBlobs(cv::Mat inputImage, bool findHoles, bool useApproximation);

	cv::Size GetSizeOfBlob( std::vector<cv::Point> blob );

	void LoadSettings();

private:
	cv::Mat backgroundImage;
	bool grabBackground = true;

	double m_cutOffThreshhold = 40;
	int m_miniumBlobPoints = 20;
	int m_maximumBlobPoints = 40;

	int m_miniumBlobSize = 20;
	int m_maximumBlobSize = 40;

	const char* settingsFileName = "Tracking_Settings.json";
};

