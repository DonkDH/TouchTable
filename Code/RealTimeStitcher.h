#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;

class RealTimeStitcher
{
public:
	struct DetectResults
	{
		Mat descriptors;
		std::vector<KeyPoint> keyPoints;
	};

	struct MatchedKeypoints
	{
		Mat status;
		Mat homoMatrix;
	};

public:
	RealTimeStitcher();
	~RealTimeStitcher();

	void CalculateStich(Mat images[], Size sizes[], float ratio, float reprojThresh, bool showMatches);

	bool CanStich();
	Mat StichImages(Mat images[], Size sizes[]);

private:
	DetectResults* DetectAndDescribe(Mat image);
	MatchedKeypoints* MatchKeypoints(DetectResults* resultsA, DetectResults* resultsB, float ratio, float reprojThresh);

	MatchedKeypoints* matchedPoints;
};

