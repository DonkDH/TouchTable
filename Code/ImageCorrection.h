#pragma once
#include "opencv2/opencv.hpp"
#include "CorrectPerspective.h"

class ImageCorrection
{
public:
	ImageCorrection();
	~ImageCorrection();
	void Init();
	cv::VideoCapture OpenCapture(cv::String source, int camera, bool forceCamera = false);

	void Update();
	cv::Mat UpdateCorrectionPerspective(cv::String sourceName, cv::Mat source, CorrectPerspective corrector );

private:
    cv::String nameA = "Capture A";
    cv::String nameB = "Capture B";
	cv::VideoCapture capA;
	cv::VideoCapture capB;
	CorrectPerspective correctionA;
	CorrectPerspective correctionB;
};

