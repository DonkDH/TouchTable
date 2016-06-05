#pragma once
#include "opencv2/opencv.hpp"
#include <fstream>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

class Utils
{
public:
	static cv::String ReadAllTextFromFile(const char * path);
};

