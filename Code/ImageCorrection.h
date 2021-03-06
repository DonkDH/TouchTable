#pragma once
#include "opencv2/opencv.hpp"
#include "CorrectPerspective.h"
#include "json.hpp"
#include <fstream>
#include "Utils.h"

class ImageCorrection
{
public:
	ImageCorrection();
	~ImageCorrection();
	void Init();
	void Update();
	void SaveSettings();

	cv::Mat GetImageA();
	cv::Mat GetImageB();

private:
	cv::VideoCapture OpenCapture(cv::String source, int camera, bool forceCamera = false);
	cv::Mat UpdateCorrectionPerspective(cv::String sourceName, cv::Mat source, CorrectPerspective* corrector);
	
private:
    cv::String m_nameA = "Capture A";
    cv::String m_nameB = "Capture B";
	cv::VideoCapture m_capA;
	cv::VideoCapture m_capB;
	CorrectPerspective* m_correctionA;
	CorrectPerspective* m_correctionB;
	cv::Mat m_sourceA;
	cv::Mat m_sourceB;
	cv::Mat m_currentA;
	cv::Mat m_currentB;

	float m_cameraAngleA = 0;
	float m_cameraAngleB = 0;

	bool m_showEditor = true;
	bool m_showSourceImage = true;
	bool m_showCurrentImage = true;

	const char* settingsFileName = "Image_Settings.json";
};

