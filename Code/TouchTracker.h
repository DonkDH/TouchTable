#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "json.hpp"
#include "Utils.h"
#include "Touch.h"
#include "Munkres.h"

class TouchTracker
{
public:
	TouchTracker();
	~TouchTracker();

	void UpdateTracking( cv::Mat inputImage, cv::String name);

	std::vector<Touch*>* GetCurrentTouches();

private:
	cv::Mat GenerateTrackingImage(cv::Mat inputImage);
	void TrackObjects(cv::Mat currentFrame, cv::String name);
	void CalculateCurrentBlobs(cv::Mat inputImage, bool findHoles, bool useApproximation);
	void UpdateTouches();
	void UpdateHungarian();

	cv::Rect GetRectOfBlob( std::vector<cv::Point> blob );


	void LoadSettings();

	void InitTouches();
	void ClearTouches();
	void TouchModifiedReset();
	Touch* GetNewTouch();

	bool GetTouchForBlob(std::vector<cv::Point> blob, Touch* &touch);

private:
	Munkres m_munkres = Munkres(50);

	std::vector<cv::Rect> m_touchRects;

	cv::Mat backgroundImage;

	cv::Mat m_lastFrame;

	bool grabBackground = true;

	double m_cutOffThreshhold = 40;
	int m_miniumBlobPoints = 20;
	int m_maximumBlobPoints = 40;

	int m_miniumBlobSize = 20;
	int m_maximumBlobSize = 40;

	std::vector<Touch*> m_touches;

	const char* settingsFileName = "Tracking_Settings.json";
};

