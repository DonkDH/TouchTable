#include "Touch.h"


Touch::Touch() : m_active(false), m_location(0, 0), m_onEnd(false), m_onStart(false)
{
}


Touch::~Touch()
{
}

void Touch::SetNewLocation(cv::Point loc)
{
	m_location = loc;

	history.push_back(loc);
}
