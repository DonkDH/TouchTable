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
	}
}

void BlobDetection::Update(cv::Mat inputImage)
{
	cv::Mat grayImage;
	cv::cvtColor(inputImage, grayImage, CV_BGR2GRAY);

	cv::imshow("Not Blured", grayImage);

	cv::GaussianBlur(grayImage, grayImage, m_blurSize, m_blurSigma);

	cv::imshow("Blured", grayImage);

	cv::Laplacian(grayImage, grayImage, CV_16S);

	cv::imshow("Laplacian", grayImage);
}
