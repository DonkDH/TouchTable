#include "TouchManager.h"
#include <math.h>

TouchManager::TouchManager() : m_calibrating(true), m_linuxInput( LinuxInput() )
{
	////hl
	//cv::Point hl = cv::Point(30, 40);
	////lr
	//cv::Point lr = cv::Point(65, 90);
	////point
	//cv::Point2d p = cv::Point2d(0.5f, 0.6f);

	////cv::Point2f out = TouchSreenToPercent(p, hl, lr);
	//cv::Point2f out;

	//out.x = (( (lr.x - hl.x)) * p.x) + hl.x;
	//out.y = (((lr.y - hl.y)) * p.y) + hl.y;

	//printf("Percent: %f, %f \n", out.x, out.y);



	cv::String loadedData = Utils::ReadAllTextFromFile(settingsFileName);
	if (loadedData.length() > 0)
	{
		m_calibrating = false;
		m_calibrationTouchActive = false;
		points = std::vector<ConversionPoint>();

		nlohmann::json settings = nlohmann::json::parse(loadedData);

		if (settings["CalibrationData"].is_array())
		{
			std::vector<int> raw = settings["CalibrationData"];

			for (int i = 0; i < raw.size(); i += 4)
			{
				ConversionPoint newPoint = ConversionPoint();
				newPoint.screenPoint = cv::Point(raw[i    ], raw[i + 1]);
				newPoint.touchPoint  = cv::Point(raw[i + 2], raw[i + 3]);

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
	if (m_tracker == nullptr || 
		m_tracker->GetCurrentTouches() == nullptr ||
		m_tracker->GetCurrentTouches()->size() == 0 ||
		(*m_tracker->GetCurrentTouches())[0] == nullptr
		)
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

		ConversionPoint screenHL, screenLR;
		GetScreenAreaPoints(p, &screenHL, &screenLR);
		
		//DrawCross(&image, screenHL.screenPoint);
		//DrawCross(&image, screenLR.screenPoint);

		cv::Point2f percent = TouchSreenToPercent(p, screenHL.touchPoint, screenLR.touchPoint);
		//printf("Percent: %f, %f \n", percent.x, percent.y);

		p.x = (((screenLR.screenPoint.x - screenHL.screenPoint.x)) * percent.x) + screenHL.screenPoint.x;
		p.y = (((screenLR.screenPoint.y - screenHL.screenPoint.y)) * percent.y) + screenHL.screenPoint.y;

		//DrawCross(&image, p);

		if (!m_calibrationTouchActive )
		{
			m_linuxInput.StartTouch(1, p.x, p.y);
			//printf("StartTouch: %d, %d \n", p.x, p.y);
			m_calibrationTouchActive = true;
		}
		else
		{
			m_linuxInput.MoveTouch(1, p.x, p.y);
			//printf("MoveTouch: %d, %d \n", p.x, p.y);
		}
	}
	else
	{
		if (m_calibrationTouchActive)
		{
			//printf("m_linuxInput.EndTouch(1);");
			m_linuxInput.EndTouch(1);
			m_calibrationTouchActive = false;
		}
	}

	m_linuxInput.Update();

	/*cvNamedWindow("CalibrationUpdate", CV_WINDOW_NORMAL);
	cvSetWindowProperty("CalibrationUpdate", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	cv::imshow("CalibrationUpdate", image);*/
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
		printf("Point : ( %i, %i ) \n\n", newPoint.touchPoint.x, newPoint.touchPoint.y);

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
cv::Point2d TouchManager::TouchSreenToPercent(cv::Point touch, cv::Point screenHL, cv::Point screenLR)
{
	cv::Point2d returnPoint = cv::Point();

	float stepX = 1.0f / (screenLR.x - screenHL.x);
	float x = (touch.x - screenHL.x);
	returnPoint.x = x * stepX;
	//returnPoint.x = screenHL.x * (1.0f - posX) + screenLR.x * posX;

	float stepY = 1.0f / (screenLR.y - screenHL.y);
	returnPoint.y = (touch.y - screenHL.y) * stepY;
	//returnPoint.y = screenHL.y * (1.0f - posY) + screenLR.y * posY;

	return returnPoint;
}

void TouchManager::GetScreenAreaPoints(cv::Point touch, ConversionPoint * screenHL, ConversionPoint * screenLR)
{
	int indexA = 0;
	int indexB = xSize + 1;
	
	double closest = 100000000;

	int i = 0;

	for (int y = 0; y < ySize - 1; ++y)
	{
		for (int x = 0; x < xSize - 1; ++x)
		{
			double d = Utils::SqrDist(touch, points[i].touchPoint);
			d += Utils::SqrDist(touch, points[i + xSize + 1].touchPoint);
			if (d < closest)
			{
				closest = d;
				indexA = i;
				indexB = i + xSize + 1;
			}

			++i;
		}

		++i;
	}

	(*screenHL) = points[indexA];
	(*screenLR) = points[indexB];

	/*

	int pointIndex = 0;
	//if (touch.x < points[0].touchPoint.x)
	{
		for (; pointIndex < xSize - 1; ++pointIndex)
		{
			if (touch.x < points[pointIndex].touchPoint.x)
			{
				if (pointIndex > 0)
				{
					pointIndex -= 1;
				}
				break;
			}
		}
	}
if (touch.y > points[pointIndex].touchPoint.y)
	{
		for (; pointIndex < ((xSize - 1) * (ySize-1)); pointIndex += xSize)
		{
			if (touch.y > points[pointIndex].touchPoint.y)
			{
				break;
			}
		}
	}

	(*screenHL) = points[pointIndex];
	(*screenLR) = points[pointIndex + xSize + 1];*/
}
