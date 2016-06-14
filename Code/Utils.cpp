#include "Utils.h"

int64 Utils::m_timerSart = 0;

cv::String Utils::ReadAllTextFromFile(const char * path)
{
	std::fstream file(path);

	std::string line = "";
	std::string compleateData = "";

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			compleateData.append(line);
			compleateData.append("\n");
		}

		file.close();
	}

	return compleateData;
}

void Utils::StartTimer()
{
	m_timerSart = cv::getTickCount();
}

double Utils::GetTime(const bool print)
{
	double time = ((double)cv::getTickCount() - m_timerSart) / cv::getTickFrequency();
	if (print)
	{
		float temp = 1.0f / 30.0f;

		std::cout << "\n\n\nFull Frame Time: " << (temp - time) << "\n\n\n";
	}
	return time;
}
