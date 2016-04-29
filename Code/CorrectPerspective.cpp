#include "CorrectPerspective.h"



CorrectPerspective::CorrectPerspective()
{
	m_currentIndex = -1;
	m_points.push_back(cv::Point2f( 0.0f, 0.0f));
	m_points.push_back(cv::Point2f( 0.0f, 200.0f));
	m_points.push_back(cv::Point2f(200.0f, 200.0f));
	m_points.push_back(cv::Point2f(200.0f, 0.0f));
}

CorrectPerspective::CorrectPerspective(std::vector<cv::Point2f> points)
{
	m_points = std::vector<cv::Point2f>(points);
}


CorrectPerspective::~CorrectPerspective()
{
}

void CorrectPerspective::UpdateEditor(cv::String windowName, cv::Mat source)
{
	cv::Mat displayImage = source.clone();

	cv::line(displayImage, m_points[0], m_points[1], cv::Scalar(255, 255, 255), 1, CV_AA, 0);
	cv::line(displayImage, m_points[1], m_points[2], cv::Scalar(0, 255, 255), 1, CV_AA, 0);
	cv::line(displayImage, m_points[2], m_points[3], cv::Scalar(0, 0, 255), 1, CV_AA, 0);
	cv::line(displayImage, m_points[3], m_points[0], cv::Scalar(255, 0, 255), 1, CV_AA, 0);

    cv::setMouseCallback(windowName, CorrectPerspective::CallBackFunc, this);

	m_editorSourceImageSize.x = displayImage.cols;
	m_editorSourceImageSize.y = displayImage.rows;
	imshow(windowName, displayImage);
}

cv::Mat CorrectPerspective::UpdatePerspective(cv::Mat source, bool sharpenImage)
{
	cv::Rect boundRect = boundingRect(m_points);

	std::vector<cv::Point2f> outputPoint;
	outputPoint.push_back(cv::Point2f(boundRect.x, boundRect.y));
	outputPoint.push_back(cv::Point2f(boundRect.x, boundRect.y + boundRect.height));
	outputPoint.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y));
	outputPoint.push_back(cv::Point2f(boundRect.x + boundRect.width, boundRect.y + boundRect.height));

	cv::Mat transformMatrix = cv::getPerspectiveTransform(m_points, outputPoint);
	cv::Mat transformed = cv::Mat::zeros(source.rows, source.cols, CV_8UC3);
	cv::warpPerspective(source, transformed, transformMatrix, source.size());
	//rectangle(transformed, boundRect, cv::Scalar(0, 255, 0), 1, 8, 0);
	if (boundRect.x < 0)
	{
		boundRect.width += boundRect.x;
		boundRect.x = 0;
	}
	if (boundRect.width + boundRect.x > transformed.cols)
	{
		boundRect.width = transformed.cols - boundRect.x;
	}
	if (boundRect.y < 0)
	{
		boundRect.height += boundRect.y;
		boundRect.y = 0;
	}
	if (boundRect.height + boundRect.y > transformed.rows)
	{
		boundRect.height = transformed.rows - boundRect.y;
	}

	transformed = transformed(boundRect);

	if (sharpenImage)
	{
		transformed = SharpenImage(transformed);
	}

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
		for (int i = 0; i < m_points.size(); ++i)
		{
			float tempX = (m_points[i].x - x);
			tempX = tempX * tempX;
			float tempY = (m_points[i].y - y);
			tempY = tempY * tempY;
			float dist = sqrt(tempX + tempY);

			if (dist < 10)
			{
				m_currentIndex = i;
				break;
			}
		}
	}
	else if (event == cv::EVENT_LBUTTONUP)
	{
		m_currentIndex = -1;
	}
	else if (event == cv::EVENT_MOUSEMOVE)
	{
		if (m_currentIndex > -1 && m_currentIndex < m_points.size())
		{
			m_points[m_currentIndex].x = x;
			m_points[m_currentIndex].y = y;

			if (m_points[m_currentIndex].x < 0)
				m_points[m_currentIndex].x = 0;
			else if (m_points[m_currentIndex].x > m_editorSourceImageSize.x)
				m_points[m_currentIndex].x = m_editorSourceImageSize.x;

			if (m_points[m_currentIndex].y < 0)
				m_points[m_currentIndex].y = 0;
			else if (m_points[m_currentIndex].y > m_editorSourceImageSize.y)
				m_points[m_currentIndex].y = m_editorSourceImageSize.y;
		}
	}
}

cv::Mat CorrectPerspective::SharpenImage(cv::Mat source)
{
	cv::Mat image;
	cv::GaussianBlur(source, image, cv::Size(0, 0), 5);
	cv::addWeighted(source, 1.5, image, -0.5, 0, image);

	return image;
}
