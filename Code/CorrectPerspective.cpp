#include "CorrectPerspective.h"



CorrectPerspective::CorrectPerspective()
{
	points.push_back(cv::Point2f( 0.0f, 0.0f));
	points.push_back(cv::Point2f(200.0f, 0.0f));
	points.push_back(cv::Point2f(200.0f, 200.0f));
	points.push_back(cv::Point2f( 10.0f, 200.0f));
}


CorrectPerspective::~CorrectPerspective()
{
}

cv::Mat CorrectPerspective::UpdatePerspective(cv::Mat source)
{
	cv::Point2f P1 = points[0];
	cv::Point2f P2 = points[1];
	cv::Point2f P3 = points[2];
	cv::Point2f P4 = points[3];

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

	cv::line(source, P1, P2, cv::Scalar(0, 0, 255), 1, CV_AA, 0);
	cv::line(source, P2, P3, cv::Scalar(0, 0, 255), 1, CV_AA, 0);
	cv::line(source, P3, P4, cv::Scalar(0, 0, 255), 1, CV_AA, 0);
	cv::line(source, P4, P1, cv::Scalar(0, 0, 255), 1, CV_AA, 0);

	points[0] = P1;
	points[1] = P2;
	points[2] = P3;
	points[3] = P4;

	return transformed;
}

void CorrectPerspective::CallBackFunc(int event, int x, int y, int flags)
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
		if (currentIndex != -1)
		{
			points[currentIndex].x = x;
			points[currentIndex].y = y;
		}
	}
}