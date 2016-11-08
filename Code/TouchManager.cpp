#include "TouchManager.h"

TouchManager::TouchManager() : m_calibrating(true), m_linuxInput( LinuxInput() )
{
	cv::String loadedData = Utils::ReadAllTextFromFile(settingsFileName);
	if (loadedData.length() > 0)
	{
		printf(loadedData.c_str());

		m_calibrating = false;
		m_calibrationTouchActive = false;
		points = std::vector<ConversionPoint>();

		nlohmann::json settings = nlohmann::json::parse(loadedData);

		if (settings["CalibrationData"].is_array())
		{
			std::vector<int> raw = settings["CalibrationData"];

			printf("raw.size() = %i \n", raw.size());

			for (int i = 0; i < raw.size(); i += 4)
			{
				ConversionPoint newPoint = ConversionPoint();
				newPoint.screenPoint = cv::Point(raw[i], raw[i + 1]);
				newPoint.touchPoint = cv::Point(raw[i + 2], raw[i + 3]);

				

				points.push_back(newPoint);
			}
		}

		printf("CalibrationData: load compleate.");
	}
	else
	{
		printf("No loaded data.");
	}
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


	cv::Mat image(cv::Size(1280, 720), CV_8SC3, cv::Scalar(0, 0, 255));

	if ((*m_tracker->GetCurrentTouches())[0]->isActive())
	{
		cv::Point p = (*m_tracker->GetCurrentTouches())[0]->GetLocation();

	/*	float stepX = 1.0f / (m_point3.x - m_point0.x);
		float posX = (p.x - m_point0.x ) * stepX;
		p.x = 100.0f * (1.0f - posX) + 500.0f * posX;

		float stepY = 1.0f / (m_point3.y - m_point0.y);
		float posY = (p.y - m_point0.y) * stepY;
		p.y = 100.0f * (1.0f - posY) + 620.0f * posY;*/

		ConversionPoint screenHL, screenLR;
		GetScreenAreaPoints(p, &screenHL, &screenLR);

		DrawCross(&image, screenHL.screenPoint);
		DrawCross(&image, screenHL.screenPoint);

		cv::Point percent = TouchSreenToPercent(p, screenHL.touchPoint, screenLR.touchPoint);
		p.x = ((1.0f / (screenLR.touchPoint.x - screenHL.touchPoint.x)) * percent.x) + screenHL.touchPoint.x;
		p.y = ((1.0f / (screenLR.touchPoint.y - screenHL.touchPoint.y)) * percent.y) + screenHL.touchPoint.y;


		DrawCross(&image, p);

		if (!m_calibrationTouchActive )
		{
			m_linuxInput.StartTouch(1, p.x, p.y);
			printf("StartTouch: %d, %d \n", p.x, p.y);
			m_calibrationTouchActive = true;
		}
		else
		{
			m_linuxInput.MoveTouch(1, p.x, p.y);
			printf("MoveTouch: %d, %d \n", p.x, p.y);
		}
	}
	else
	{
		if (m_calibrationTouchActive)
		{
			printf("m_linuxInput.EndTouch(1);");
			m_linuxInput.EndTouch(1);
			m_calibrationTouchActive = false;
		}
	}

	m_linuxInput.Update();

	cvNamedWindow("CalibrationUpdate", CV_WINDOW_NORMAL);
	cvSetWindowProperty("CalibrationUpdate", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	cv::imshow("CalibrationUpdate", image);

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
		xCurrentStage++;
		if (xCurrentStage >= xSize)
		{
			xCurrentStage = 0;
			yCurrentStage++;
		}
	}

	if (touchFound && !m_calibrationTouchActive)
	{
		m_calibrationTouchActive = true;

		ConversionPoint newPoint = ConversionPoint();
		newPoint.screenPoint = cv::Point();
		newPoint.screenPoint.x = xArray[xCurrentStage];
		newPoint.screenPoint.y = yArray[yCurrentStage];

		newPoint.touchPoint = cv::Point(touchPoint);

		printf("Screen: ( %i, %i ) \n", newPoint.screenPoint.x, newPoint.screenPoint.y);
		printf("Point : ( %i, %i ) \n", newPoint.touchPoint.x, newPoint.touchPoint.y);
		printf("sdfgg : ( %d, %f ) \n", 69.69f, 69.69f);
		printf("Touch : ( %i, %i ) \n\n\n", touchPoint.x, touchPoint.y);

		points.push_back(newPoint);
	}

	if (xSize * ySize <= m_calibrationStage)
	{
		m_calibrating = false;
		m_calibrationTouchActive = false;
		//m_calibrationStage = 0;
		cv::destroyWindow("CalibrationUpdate");

		{
			nlohmann::json exportData;

			{
				std::vector<int> tempStore = std::vector<int>();
				auto itor = points.begin();
				for (; itor != points.end(); ++itor)
				{
					tempStore.push_back(itor->screenPoint.x);
					tempStore.push_back(itor->screenPoint.y);
					tempStore.push_back(itor->touchPoint.x);
					tempStore.push_back(itor->touchPoint.y);
				}
				exportData["CalibrationData"] = tempStore;
			}

			Utils::WrightTextToFile(settingsFileName, exportData.dump());
		}

		return;
	}
	else
	{
		DrawCross(&image, cv::Point(xArray[xCurrentStage], yArray[yCurrentStage]));
	}

	/*	
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
		//m_calibrationStage = 0;
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
	*/

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

// This is poorly named.....
cv::Point TouchManager::TouchSreenToPercent(cv::Point touch, cv::Point screenHL, cv::Point screenLR)
{
	cv::Point returnPoint = cv::Point();

	float stepX = 1.0f / (screenHL.x - screenLR.x);
	float posX = (touch.x - screenLR.x) * stepX;
	returnPoint.x = screenHL.x * (1.0f - posX) + screenLR.x * posX;

	float stepY = 1.0f / (screenHL.y - screenLR.y);
	float posY = (touch.y - screenLR.y) * stepY;
	returnPoint.y = screenHL.y * (1.0f - posY) + screenLR.y * posY;

	return returnPoint;
}

void TouchManager::GetScreenAreaPoints(cv::Point touch, ConversionPoint * screenHL, ConversionPoint * screenLR)
{
	int pointIndex = 0;
	if (touch.x > points[0].touchPoint.x)
	{
		for (; pointIndex < xSize-1; ++pointIndex)
		{
			if (touch.x > points[pointIndex].touchPoint.x)
			{
				break;
			}
		}
	}

	if (touch.y > points[pointIndex].touchPoint.y)
	{
		for (; pointIndex < (xSize * (ySize-1)); pointIndex += xSize)
		{
			if (touch.y > points[pointIndex].touchPoint.y)
			{
				break;
			}
		}
	}

	(*screenHL) = points[pointIndex];
	(*screenLR) = points[pointIndex + xSize + 1];
}


