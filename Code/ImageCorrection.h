#pragma once
#include "opencv2/opencv.hpp"
#include "CorrectPerspective.h"

//comment

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
    cv::String m_nameA = "Capture A";
    cv::String m_nameB = "Capture B";
	cv::VideoCapture m_capA;
	cv::VideoCapture m_capB;
	CorrectPerspective m_correctionA;
	CorrectPerspective m_correctionB;
	cv::Mat m_sourceA;
	cv::Mat m_sourceB;
};

