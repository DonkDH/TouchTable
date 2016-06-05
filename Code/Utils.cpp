#include "Utils.h"

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