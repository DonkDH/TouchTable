#include "CorrectPerspective.h"



CorrectPerspective::CorrectPerspective()
{
	currentIndex = -1;
	points.push_back(cv::Point2f( 0.0f, 0.0f));
	points.push_back(cv::Point2f( 0.0f, 200.0f));
	points.push_back(cv::Point2f(200.0f, 200.0f));
	points.push_back(cv::Point2f(200.0f, 0.0f));
}


CorrectPerspective::~CorrectPerspective()
{
}

void CorrectPerspective::UpdateEditor(cv::String windowName, cv::Mat source)
{
    cv::Mat displayImage(source);

	cv::line(displayImage, points[0], points[1], cv::Scalar(255, 255, 255), 1, CV_AA, 0);
	cv::line(displayImage, points[1], points[2], cv::Scalar(0, 255, 255), 1, CV_AA, 0);
	cv::line(displayImage, points[2], points[3], cv::Scalar(0, 0, 255), 1, CV_AA, 0);
	cv::line(displayImage, points[3], points[0], cv::Scalar(255, 0, 255), 1, CV_AA, 0);

    	cv::setMouseCallback(windowName, CorrectPerspective::CallBackFunc, this);
	imshow(windowName, displayImage);
}

cv::Mat CorrectPerspective::UpdatePerspective(cv::Mat source)
{
	cv::Rect boundRect = boundingRect(points);

	std::vector<cv::Point2f> outputPoint;
	outputPoint.push_back(cv::Point2f(boundRect.x, boundRect.y));
	outputPoint.push_back(cv::Point2f(boundRect.x, boundRect.y + boundRect.height));
	outputPoint.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y));
	outputPoint.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y + boundRect.height));

	cv::Mat transformMatrix = cv::getPerspectiveTransform(points, outputPoint);
	cv::Mat transformed = cv::Mat::zeros(source.rows, source.cols, CV_8UC3);
	cv::warpPerspective(source, transformed, transformMatrix, source.size());
	rectangle(transformed, boundRect, cv::Scalar(0, 255, 0), 1, 8, 0);

	return transformed;
}

void CorrectPerspective::CallBackFunc(int event, int x, int y, int flags, void* userData)
{
    ((CorrectPerspective*)userData)->UpdateInput(event, x, y, flags);
}

void CorrectPerspective::UpdateInput(int event, int x, int y, int flags)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		for (int i = 0; i < points.size(); ++i)
		{
			float tempX = (points[i].x - x);
			tempX = tempX * tempX;
			float tempY = (points[i].y - y);
			tempY = tempY * tempY;
			float dist = sqrt(tempX + tempY);

			if (dist < 10)
			{
				currentIndex = i;
				break;
			}
		}
	}
	else if (event == cv::EVENT_LBUTTONUP)
	{
		currentIndex = -1;
	}
	else if (event == cv::EVENT_MOUSEMOVE)
	{
		if (currentIndex > -1 && currentIndex < points.size())
		{
			points[currentIndex].x = x;
			points[currentIndex].y = y;
		}
	}
}
