#include "RealTimeStitcher.h"



RealTimeStitcher::RealTimeStitcher()
{
	matchedPoints = nullptr;
}


RealTimeStitcher::~RealTimeStitcher()
{
}

void RealTimeStitcher::CalculateStich(Mat images[], Size sizes[], float ratio, float reprojThresh, bool showMatches)
{
	RealTimeStitcher::DetectResults* resultsA = DetectAndDescribe(images[0]);
	RealTimeStitcher::DetectResults* resultsB = DetectAndDescribe(images[1]);

	matchedPoints = MatchKeypoints(resultsA, resultsB, ratio, reprojThresh);
}

bool RealTimeStitcher::CanStich()
{
	return matchedPoints != nullptr;
}

Mat RealTimeStitcher::StichImages(Mat images[], Size sizes[])
{
	Mat outputImage;

	if (matchedPoints == nullptr)
	{
		return outputImage;
	}

	// start the warp here.
	Size outPutSize = sizes[0];
	outPutSize.width += sizes[1].width;
	outPutSize.height += sizes[1].height;

	warpPerspective(images[0], outputImage, matchedPoints->homoMatrix, outPutSize);

	images[1].copyTo(outputImage(Rect(0, 0, sizes[0].width, sizes[0].height)));
	return outputImage;
}

RealTimeStitcher::DetectResults* RealTimeStitcher::DetectAndDescribe(Mat image)
{
	RealTimeStitcher::DetectResults* returnData = new RealTimeStitcher::DetectResults();
	cv::cvtColor(image, image, COLOR_BGR2GRAY);

	auto detector = xfeatures2d::SIFT::create();
	returnData->keyPoints = std::vector<KeyPoint>();
	detector->detect(image, returnData->keyPoints);
	
	returnData->descriptors;
	detector->compute(image, returnData->keyPoints, returnData->descriptors);

	return returnData;
}

RealTimeStitcher::MatchedKeypoints* RealTimeStitcher::MatchKeypoints(DetectResults * resultsA, DetectResults * resultsB, float ratio, float reprojThresh)
{
	auto matcher = DescriptorMatcher::create("BruteForce");	
	std::vector<std::vector<DMatch>> matches = std::vector<std::vector<DMatch>>();
	matcher->knnMatch(resultsA->descriptors, resultsB->descriptors, matches, 2);

	std::vector< int > matchIndexes = std::vector<int>();

	for (int i = 0; i < matches.size(); ++i)
	{
		if (matches[i].size() == 2)
		{
			if (matches[i][0].distance < ( matches[i][1].distance * ratio ))
			{
				matchIndexes.push_back(matches[i][0].trainIdx);
				matchIndexes.push_back(matches[i][0].queryIdx);
			}
		}
	}

	if (matches.size() >= 4)
	{
		std::vector<Point2f> ptsA = std::vector<Point2f>();
		std::vector<Point2f> ptsB = std::vector<Point2f>();

		for (int i = 0; i < matchIndexes.size(); i+=2)
		{
			ptsA.push_back(resultsA->keyPoints[ matchIndexes[ i + 1 ] ].pt);
			ptsB.push_back(resultsB->keyPoints[ matchIndexes[  i  ] ].pt);
		}

		MatchedKeypoints* matchData = new MatchedKeypoints();
		matchData->homoMatrix = findHomography(ptsA, ptsB, RANSAC, reprojThresh, matchData->status);

		return matchData;
	}

	return nullptr;
}
