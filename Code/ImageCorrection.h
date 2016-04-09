#pragma once
#include "opencv2/opencv.hpp"
class ImageCorrection
{
public:
	ImageCorrection();
	~ImageCorrection();
	void Init();
	cv::VideoCapture OpenCapture(cv::String source, int camera, bool forceCamera = false);

	void Update();

private:
	cv::VideoCapture capA;
	cv::VideoCapture capB;
};

