#pragma once
#include "TouchTracker.h"
#include "LinuxInput.h"
#include "opencv/cv.h"
#include <math.h>

class TouchManager
{
public:
	struct ConversionPoint
	{
		cv::Point screenPoint;
		cv::Point touchPoint;
	};

public:
	TouchManager();
	~TouchManager();

	void SetTouchTracker(TouchTracker* tracker);

	void Update();

private:
	void CalibrationUpdate();

	void DrawCross(cv::Mat* mat, cv::Point p);

	// This is poorly named.....
	cv::Point TouchSreenToPercent(cv::Point touch, cv::Point screenHL, cv::Point screenLR);
	void GetScreenAreaPoints(cv::Point touch, ConversionPoint* screenHL, ConversionPoint* screenLR);

private:
	TouchTracker* m_tracker = nullptr;
	LinuxInput m_linuxInput;

	bool m_calibrating = true;
	int m_calibrationStage = 0;
	bool m_calibrationTouchActive = false;

	cv::Point m_point0;
	cv::Point m_point1;
	cv::Point m_point2;
	cv::Point m_point3;

	int xCurrentStage = 0;
	int xSize = 6;
	float xArray[6] { 100.0f, 200.0f, 300.0f, 400.0f, 500.0f, 600.0f };
	int yCurrentStage = 0;
	int ySize = 6;
	float yArray[6]{ 100.0f, 200.0f, 300.0f, 400.0f, 500.0f, 600.0f };

	std::vector<ConversionPoint> points = std::vector<ConversionPoint>();
};

