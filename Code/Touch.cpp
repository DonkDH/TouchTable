#include "Touch.h"


Touch::Touch() : m_active(false), m_location(0, 0), m_onEnd(false), m_onStart(false)
{
	m_kalmanFilter = cv::KalmanFilter(2, 2);
	m_kalmanUpdateMat = cv::Mat_<float>(2,1);
}


Touch::~Touch()
{
}

void Touch::SetNewLocation(cv::Point loc)
{
	m_active = true;

	m_rawLocation = loc;

	m_kalmanUpdateMat(0) = loc.x;
	m_kalmanUpdateMat(1) = loc.y;

	m_kalmanFilter.correct(m_kalmanUpdateMat);

	m_inactiveFrameCount = 0;
	m_updatedThisFrame = true;
}

void Touch::Update()
{
	auto output = m_kalmanFilter.predict();

	m_location = cv::Point(output.at<float>(0), output.at<float>(1));

	history.push_back(m_location);
}

void Touch::FrameReset()
{
	if (m_active)
	{
		if (!m_updatedThisFrame)
		{
			++m_inactiveFrameCount;
		}

		if (m_inactiveFrameCount > 10)
		{
			m_active = false;
		}
	}

	m_updatedThisFrame = false;
}
