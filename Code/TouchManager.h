#pragma once
#include "TouchTracker.h"
#include "LinuxInput.h"
#include "opencv/cv.h"


class TouchManager
{
public:
	TouchManager();
	~TouchManager();

	void SetTouchTracker(TouchTracker* tracker);

	void Update();

private:
	void CalibrationUpdate();

	void DrawCross(cv::Mat* mat, cv::Point p);

private:
	TouchTracker* m_tracker = nullptr;

	bool m_calibrating = true;
	int m_calibrationStage = 0;
	bool m_calibrationTouchActive = false;

	cv::Point m_point0;
	cv::Point m_point1;
	cv::Point m_point2;
	cv::Point m_point3;
};

