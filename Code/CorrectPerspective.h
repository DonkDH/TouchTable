#pragma once
#include "opencv2/opencv.hpp"
#include <vector>

class CorrectPerspective
{
public:
	CorrectPerspective();
	~CorrectPerspective();

	cv::Mat UpdatePerspective(cv::Mat source);
	void UpdateEditor(cv::String windowName, cv::Mat source);

    static void CallBackFunc(int event, int x, int y, int flags, void* userData);
	void UpdateInput(int event, int x, int y, int flags);

private:
	std::vector<cv::Point2f> points;
	int currentIndex = -1;
};

