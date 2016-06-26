#pragma once
#include "opencv2/opencv.hpp"

class Touch
{
public:
	Touch();
	~Touch();

	bool m_active;
	bool m_onStart;
	bool m_onEnd;
	bool m_updatedThisFrame;

	cv::Point m_location;
};

