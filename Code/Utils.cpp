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

void Utils::WrightTextToFile(const char * path, std::string data)
{
	std::fstream file;
	file.open(path, std::ios::out);

	if (file.is_open())
	{
		file.clear();
		file << data;

		file.close();
	}
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


cv::Mat Utils::RotateMat(cv::Mat source, float angle)
{
	cv::Point2f src_center(source.size() / 2);
	cv::Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);

	cv::Rect rotatedRect = (cv::RotatedRect(src_center, source.size(), angle)).boundingRect();

	rot_mat.at<double>(0, 2) += rotatedRect.width / 2.0 - src_center.x;
	rot_mat.at<double>(1, 2) += rotatedRect.height / 2.0 - src_center.y;

	cv::Mat dst;

	cv::warpAffine(source, dst, rot_mat, rotatedRect.size());
	return dst;
}