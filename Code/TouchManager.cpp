#include "TouchManager.h"

TouchManager::TouchManager() : m_calibrating(true)
{
}


TouchManager::~TouchManager()
{
}

void TouchManager::SetTouchTracker(TouchTracker * tracker)
{
	m_tracker = tracker;
}

void TouchManager::Update()
{
	if (m_tracker == nullptr || m_tracker->GetCurrentTouches() == nullptr)
	{
		return;
	}

	if (m_calibrating)
	{
		CalibrationUpdate();
		return;
	}

	auto touch = m_tracker->GetCurrentTouches()->begin();
	for ( ; touch != m_tracker->GetCurrentTouches()->end(); touch++)
	{
		if ((*touch)->isActive())
		{

		}
	}
}

void TouchManager::CalibrationUpdate()
{
	cv::Mat image(cv::Size(1280, 720), CV_8SC3, cv::Scalar(0, 0, 255));

	cv::Point touchPoint;
	bool touchFound = false;

	auto touch = m_tracker->GetCurrentTouches()->begin();
	for (; touch != m_tracker->GetCurrentTouches()->end(); touch++)
	{
		if ((*touch)->isActive())
		{
			touchPoint = (*touch)->GetLocation();
			touchFound = true;
			break;
		}
	}

	if (!touchFound && m_calibrationTouchActive)
	{
		m_calibrationTouchActive = false;
		m_calibrationStage++;
	}

	switch (m_calibrationStage)
	{
	case 0:
		DrawCross(&image, cv::Point(100, 100));
		break;
	case 1:
		DrawCross(&image, cv::Point(500, 100));
		break;
	case 2:
		DrawCross(&image, cv::Point(100, 620));
		break;
	case 3:
		DrawCross(&image, cv::Point(500, 620));
		break;
	case 4:
		m_calibrating = false;
		m_calibrationTouchActive = false;
		m_calibrationStage = 0;
		return;
	}

	if (touchFound && !m_calibrationTouchActive)
	{
		m_calibrationTouchActive = true;

		switch (m_calibrationStage)
		{
		case 0:
			m_point0 = touchPoint;
			break;
		case 1:
			m_point1 = touchPoint;
			break;
		case 2:
			m_point2 = touchPoint;
			break;
		case 3:
			m_point3 = touchPoint;
			break;
		}
	}

	cvNamedWindow("CalibrationUpdate", CV_WINDOW_NORMAL);
	cvSetWindowProperty("CalibrationUpdate", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	cv::imshow("CalibrationUpdate", image);
}

void TouchManager::DrawCross(cv::Mat * mat, cv::Point p)
{
	float crossSize = 10;
	cv::line(*mat, p - cv::Point(0, crossSize), p + cv::Point(0, crossSize), cv:: Scalar(0, 0, 0));
	cv::line(*mat, p - cv::Point(crossSize, 0), p + cv::Point(crossSize, 0), cv::Scalar(0, 0, 0));
}


