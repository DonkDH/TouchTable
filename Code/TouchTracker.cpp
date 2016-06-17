#include "TouchTracker.h"



TouchTracker::TouchTracker()
{
	LoadSettings();
}


TouchTracker::~TouchTracker()
{
}

void TouchTracker::UpdateTracking( cv::Mat inputImage, cv::String name)
{
	cv::Mat adjustedImage = GenerateTrackingImage(inputImage);

	cv::imshow(name, adjustedImage);

	TrackObjects(adjustedImage, name);
	
	//CalculateCurrentBlobs(adjustedImage, false, true);
}

cv::Mat TouchTracker::GenerateTrackingImage(cv::Mat inputImage)
{
	cv::Mat grayImage;
	cvtColor(inputImage, grayImage, CV_BGR2GRAY);

	if (!grabBackground)
	{
		if (inputImage.cols != backgroundImage.cols ||
			inputImage.rows != backgroundImage.rows)
		{
			grabBackground = true;
		}
	}

	if (grabBackground)
	{
		grayImage.copyTo(backgroundImage);
		grabBackground = false;
	}

	cv::absdiff(backgroundImage, grayImage, grayImage);

	cv::threshold(grayImage, grayImage, m_cutOffThreshhold, 255, cv::ThresholdTypes::THRESH_BINARY);

	return grayImage;
}

void TouchTracker::TrackObjects( cv::Mat currentFrame, cv::String name )
{
	if (m_lastFrame.rows == 0 || m_lastFrame.cols == 0)
	{
		m_lastFrame = currentFrame.clone();
	}

	cv::Mat differenceImage;
	cv::absdiff(currentFrame, m_lastFrame, differenceImage);

	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	cv::findContours(differenceImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // retrieves all contours
	//cv::findContours(differenceImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); // retrieves external contours

	bool objectDetected = false;
	if (contours.size()>0)
		objectDetected = true;
	else
		objectDetected = false;

	if (objectDetected) {

		int count = 0;
		bool didSomething = false;
		for (int i = 0; i < contours.size(); ++i)
		{
			if (contours[i].size() > m_miniumBlobPoints)
			{
				didSomething = true;
				++count;

				cv::Rect rect = cv::boundingRect(contours[i]);
				int x = rect.x + rect.width / 2;
				int y = rect.y + rect.height / 2;

				cv::circle(differenceImage, cv::Point(x, y), 20, cv::Scalar(255, 255, 255), 2);
				cv::line(differenceImage, cv::Point(x, y), cv::Point(x, y - 25), cv::Scalar(255, 255, 255), 2);
				cv::line(differenceImage, cv::Point(x, y), cv::Point(x, y + 25), cv::Scalar(255, 255, 255), 2);
				cv::line(differenceImage, cv::Point(x, y), cv::Point(x - 25, y), cv::Scalar(255, 255, 255), 2);
				cv::line(differenceImage, cv::Point(x, y), cv::Point(x + 25, y), cv::Scalar(255, 255, 255), 2);
			}
		}
		if (didSomething)
		{
			cv::imshow(name + " Tracked", differenceImage);
			std::cout << "\n" << "Count: " << count << "\n";
		}
	}

	m_lastFrame = currentFrame.clone();
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
			//if (ittor->size() < m_miniumBlobPoints || ittor->size() > m_maximumBlobPoints)
			{
				cv::Size blobSize = GetSizeOfBlob( *ittor );

				std::cout << "Blob Size: " << blobSize.width << ", " << blobSize.height << " - ";

				// remove smaller blobs
				if (  ( blobSize.height > m_miniumBlobSize && blobSize.height < m_maximumBlobSize )
				   && ( blobSize.width > m_miniumBlobSize && blobSize.width < m_maximumBlobSize ) )
				{
					++ittor;
				}
				else
				{
					blobs.erase(ittor);
				}
			}
			//else
			{
			//	++ittor;
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

	for (; ittor != blob.end(); ++ittor)
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
			m_miniumBlobPoints = settings["miniumBlobSize"];
		}

		if (settings["maximumBlobSize"].is_number())
		{
			m_maximumBlobPoints = settings["maximumBlobSize"];
		}
		
		if (settings["miniumBlobWidth"].is_number())
		{
			m_miniumBlobSize = settings["miniumBlobWidth"];
		}

		if (settings["maximumBlobHeight"].is_number())
		{
			m_maximumBlobSize = settings["maximumBlobHeight"];
		}
	}
}

