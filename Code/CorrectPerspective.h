#pragma once
#include "opencv2/opencv.hpp"
#include <vector>
#include <ostream>
#include "Utils.h"

class CorrectPerspective
{
public:
	CorrectPerspective();
	CorrectPerspective(std::vector<cv::Point2f> points);
	~CorrectPerspective();

	cv::Mat UpdatePerspective(cv::Mat source, bool sharpenImage);
	void UpdateEditor(cv::String windowName, cv::Mat source);

    static void CallBackFunc(int event, int x, int y, int flags, void* userData);
	void UpdateInput(int event, int x, int y, int flags);

	std::vector<cv::Point2f> GetCorrectionCordinates() { return m_points; };

	cv::Mat GetCurrentImage();

private:
	cv::Mat SharpenImage(cv::Mat source);

private:
	cv::Mat m_currentImage;

	std::vector<cv::Point2f> m_points;
	int m_currentIndex = -1;
	cv::Point2f m_editorSourceImageSize;
};

