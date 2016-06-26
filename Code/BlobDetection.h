#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"

class BlobDetection
{
public:
	BlobDetection();
	~BlobDetection();

	void Init();
	void Update(cv::Mat inputImage);

private:
	cv::Size m_blurSize = cv::Size(15,15);
	double m_blurSigma = 6;
	int m_laplacianSize = 1;

	const char* settingsFileName = "Blob_Settings.json";
};

