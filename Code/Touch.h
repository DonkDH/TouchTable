#pragma once
#include "opencv2/opencv.hpp"

class Touch
{
public:
	Touch();
	~Touch();

	void SetNewLocation(cv::Point loc);
	void Update();
	bool isActive() { return m_active; };
	cv::Point GetLocation() { return m_location; };
	cv::Point GetRawLocation() { return m_rawLocation; };

	void FrameReset();

	std::vector<cv::Point> history = std::vector<cv::Point>();

private:

	bool m_active;
	bool m_onStart;
	bool m_onEnd;
	bool m_updatedThisFrame;

	int m_inactiveFrameCount = 0;

	cv::Point m_location;
	cv::Point m_rawLocation;

	cv::KalmanFilter m_kalmanFilter;
	cv::Mat_<float> m_kalmanUpdateMat;
};

