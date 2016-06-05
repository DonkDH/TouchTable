#include "TouchTracker.h"



TouchTracker::TouchTracker()
{
	LoadSettings();
}


TouchTracker::~TouchTracker()
{
}

void TouchTracker::UpdateTracking( cv::Mat inputImage )
{
	cv::Mat adjustedImage = GenerateTrackingImage(inputImage);

	cv::imshow("Adjusted Image", adjustedImage);

	CalculateCurrentBlobs(adjustedImage, false, true);
}

cv::Mat TouchTracker::GenerateTrackingImage(cv::Mat inputImage)
{
	cv::Mat grayImage;
	cvtColor(inputImage, grayImage, CV_BGR2GRAY);

	if (grabBackground)
	{
		grayImage.copyTo(backgroundImage);
		grabBackground = false;
	}

	cv::absdiff(backgroundImage, grayImage, grayImage);

	cv::threshold(grayImage, grayImage, m_cutOffThreshhold, 255, cv::ThresholdTypes::THRESH_BINARY);

	return grayImage;
}

void TouchTracker::CalculateCurrentBlobs(cv::Mat inputImage, bool findHoles, bool useApproximation)
{
	std::vector< std::vector<cv::Point> > blobs;

	cv::RetrievalModes retrievalMode = findHoles ? cv::RetrievalModes::RETR_LIST : cv::RetrievalModes::RETR_EXTERNAL;
	cv::ContourApproximationModes contourApproximation = useApproximation ? cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE : cv::ContourApproximationModes::CHAIN_APPROX_NONE;
	cv::findContours(inputImage, blobs, retrievalMode, contourApproximation);

	if (blobs.size() > 0)
	{
		auto ittor = blobs.begin();
		while ( ittor != blobs.end())
		{
			if (ittor->size() < m_miniumBlobSize)
			{
				// remove smaller blobs
				blobs.erase(ittor);
			}
			else
			{
				++ittor;
			}
		}

		std::cout << "Blobs: " << blobs.size() << "\n \n \n";
	}
}

void TouchTracker::LoadSettings()
{
	cv::String loadedData = Utils::ReadAllTextFromFile(settingsFileName);
	if (loadedData.length() > 0)
	{
		nlohmann::json settings = nlohmann::json::parse(loadedData);
		if (settings["cutOffThreshhold"].is_number())
		{
			m_cutOffThreshhold = settings["cutOffThreshhold"];
		}

		if (settings["miniumBlobSize"].is_number())
		{
			m_miniumBlobSize = settings["miniumBlobSize"];
		}
	}
}

