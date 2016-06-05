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
			if (ittor->size() < m_miniumBlobSize || ittor->size() > m_maximumBlobSize)
			{
				cv::Size blobSize = GetSizeOfBlob( *ittor );

				std::cout << "Blob Size: " << blobSize.width << ", " << blobSize.height << " - ";

				// remove smaller blobs
				if (  ( blobSize.height < m_miniumBlobWidth && blobSize.height > m_maximumBlobHeight )
				   && ( blobSize.width < m_miniumBlobWidth && blobSize.width > m_maximumBlobHeight ) )
				{
					blobs.erase(ittor);
				}
				else
				{
					++ittor;
				}
			}
			else
			{
				++ittor;
			}
		}

		std::cout << "Blobs: " << blobs.size() << "\n \n \n";
	}
}

cv::Size TouchTracker::GetSizeOfBlob(std::vector<cv::Point> blob)
{
	if (blob.size() <= 0)
	{
		return cv::Size();
	}

	auto ittor = blob.begin();
	cv::Point2f lowLeft = cv::Point2f(ittor->x, ittor->y);
	cv::Point2f highRight = lowLeft;
	++ittor;

	for (; ittor == blob.end(); ++ittor)
	{
		lowLeft.x = MIN(lowLeft.x, ittor->x);
		lowLeft.y = MIN(lowLeft.y, ittor->y);

		highRight.x = MAX(highRight.x, ittor->x);
		highRight.y = MAX(highRight.y, ittor->y);
	}

	return cv::Size( (highRight.x - lowLeft.x), (highRight.y - lowLeft.y));
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

		if (settings["maximumBlobSize"].is_number())
		{
			m_maximumBlobSize = settings["maximumBlobSize"];
		}
		
		if (settings["miniumBlobWidth"].is_number())
		{
			m_miniumBlobWidth = settings["miniumBlobWidth"];
		}

		if (settings["maximumBlobHeight"].is_number())
		{
			m_maximumBlobHeight = settings["maximumBlobHeight"];
		}
	}
}

