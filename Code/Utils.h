#pragma once
#include "opencv2/opencv.hpp"
#include <fstream>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

class Utils
{
public:
	static cv::String ReadAllTextFromFile(const char * path);
	static void WrightTextToFile(const char * path, std::string data);

	static void StartTimer();
	static double GetTime(const bool print = false);
	static cv::Mat RotateMat(cv::Mat source, float angle);

	static double SqrDist(const cv::Point a, const cv::Point b);
	static double Dist(const cv::Point a, const cv::Point b);

private:
	static int64 m_timerSart;
};

