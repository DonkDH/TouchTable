#pragma once
#include "opencv2/opencv.hpp"
#include <vector>

class CorrectPerspective
{
public:
	CorrectPerspective();
	~CorrectPerspective();

	cv::Mat UpdatePerspective(cv::Mat source);
	
	void CallBackFunc(int event, int x, int y, int flags);

private:
	std::vector<cv::Point2f> points;
	int currentIndex = -1;
};

