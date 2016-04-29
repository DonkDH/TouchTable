#pragma once
#include "opencv2/opencv.hpp"
#include "CorrectPerspective.h"
#include "RealTimeStitcher.h"
#include "json.hpp"
#include <fstream>

class ImageCorrection
{
public:
	ImageCorrection();
	~ImageCorrection();
	void Init();
	void Update();
	void CalculateImageStitch();
	void SaveSettings();

private:
	cv::VideoCapture OpenCapture(cv::String source, int camera, bool forceCamera = false);
	cv::Mat UpdateCorrectionPerspective(cv::String sourceName, cv::Mat source, CorrectPerspective* corrector);


	cv::String ReadAllTextFromFile(const char* path);
	void WrightTextToFile(const char* path, std::string data);

private:
    cv::String m_nameA = "Capture A";
    cv::String m_nameB = "Capture B";
	cv::VideoCapture m_capA;
	cv::VideoCapture m_capB;
	CorrectPerspective* m_correctionA;
	CorrectPerspective* m_correctionB;
	cv::Mat m_sourceA;
	cv::Mat m_sourceB;

	RealTimeStitcher* m_stitcher;
	bool calculateStich = false;

	bool showEditor = true;
};

