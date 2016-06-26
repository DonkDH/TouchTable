#include "BlobDetection.h"
#include "Utils.h"
#include "json.hpp"

BlobDetection::BlobDetection()
{
}


BlobDetection::~BlobDetection()
{
}

void BlobDetection::Init()
{
	cv::String loadedData = Utils::ReadAllTextFromFile(settingsFileName);
	if (loadedData.length() > 0)
	{
		nlohmann::json settings = nlohmann::json::parse(loadedData);

		if (settings["blurSizeX"].is_number())
		{
			m_blurSize.width = settings["blurSizeX"];
			if (m_blurSize.width % 2 != 1)
				m_blurSize.width += 1;
		}

		if (settings["blurSizeY"].is_number())
		{
			m_blurSize.height = settings["blurSizeY"];
			if (m_blurSize.height % 2 != 1)
				m_blurSize.height += 1;
		}

		if (settings["blurSigma"].is_number())
		{
			m_blurSigma = settings["blurSigma"];
		}

		if (settings["laplacianSize"].is_number_integer())
		{
			m_laplacianSize = settings["laplacianSize"];
			if (m_laplacianSize % 2 != 1)
				m_laplacianSize += 1;
			if (m_laplacianSize > 31)
				m_laplacianSize = 31;
		}

		if (settings["cutOffThreshhold"].is_number())
		{
			m_cutOffThreshhold = settings["cutOffThreshhold"];
		}

		if (settings["shouldInvert"].is_boolean())
		{
			m_shouldInvert = settings["shouldInvert"];
		}
	}
}

void BlobDetection::Update(cv::Mat inputImage)
{
	cv::Mat grayImage;
	cv::cvtColor(inputImage, grayImage, CV_BGR2GRAY);

	if (inputImage.cols != backgroundImage.cols ||
		inputImage.rows != backgroundImage.rows)
	{
		backgroundImage = grayImage.clone();
		return;
	}

	cv::absdiff(backgroundImage, grayImage, grayImage);

	if(m_shouldInvert)
		cv::bitwise_not(grayImage, grayImage);

	cv::imshow("Not Blured", grayImage);

	cv::GaussianBlur(grayImage, grayImage, m_blurSize, m_blurSigma);

	cv::imshow("Blured", grayImage);

	cv::Laplacian(grayImage, grayImage, CV_8U, m_laplacianSize);

	cv::imshow("Laplacian", grayImage);

	cv::threshold(grayImage, grayImage, m_cutOffThreshhold, 255, cv::ThresholdTypes::THRESH_BINARY);

	cv::imshow("Threasholded", grayImage);
}
