#include "TouchManager.h"

TouchManager::TouchManager() : m_calibrating(true), m_linuxInput( LinuxInput() )
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


	if ((*m_tracker->GetCurrentTouches())[0]->isActive())
	{
		cv::Point p = (*m_tracker->GetCurrentTouches())[0]->GetLocation();

		float stepX = 1.0f / (m_point3.x - m_point0.x);
		float posX = (p.x - m_point0.x ) * stepX;
		p.x = 100.0f * (1.0f - posX) + 500.0f * posX;

		float stepY = 1.0f / (m_point3.y - m_point0.y);
		float posY = (p.y - m_point0.y) * stepY;
		p.y = 100.0f * (1.0f - posY) + 620.0f * posY;

		if (m_calibrationStage > 0)
		{
			m_linuxInput.StartTouch(0, p.x, p.y);
		}
		else
		{
			m_linuxInput.MoveTouch(0, p.x, p.y);
		}

		m_calibrationStage = 0;
	}
	else
	{
		if (m_calibrationStage = 0)
		{
			m_linuxInput.EndTouch(0);
			++m_calibrationStage;
		}
	}

	/*
	auto touch = m_tracker->GetCurrentTouches()->begin();
	for ( ; touch != m_tracker->GetCurrentTouches()->end(); touch++)
	{
		if ((*touch)->isActive())
		{
			
		}
	}
	*/
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
		DrawCross(&image, cv::Point(100, 100)); // top left
		break;
	case 1:
		DrawCross(&image, cv::Point(500, 100)); // top right
		break;
	case 2:
		DrawCross(&image, cv::Point(100, 620)); // bottom left
		break;
	case 3:
		DrawCross(&image, cv::Point(500, 620)); // bottom right
		break;
	case 4:
		m_calibrating = false;
		m_calibrationTouchActive = false;
		m_calibrationStage = 0;
		cv::destroyWindow("CalibrationUpdate");
		return;
	}

	if (touchFound && !m_calibrationTouchActive)
	{
		m_calibrationTouchActive = true;

		switch (m_calibrationStage)
		{
		case 0:
			m_point0 = touchPoint; // top left
			break;
		case 1:
			m_point1 = touchPoint; // top right
			break;
		case 2:
			m_point2 = touchPoint; // bottom left
			break;
		case 3:
			m_point3 = touchPoint; // bottom right
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


