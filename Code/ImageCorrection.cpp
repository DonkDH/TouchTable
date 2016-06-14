#include "ImageCorrection.h"


ImageCorrection::ImageCorrection()
{
}

ImageCorrection::~ImageCorrection()
{
	delete m_correctionA;
	delete m_correctionB;
}

void ImageCorrection::Init()
{
	m_capA = OpenCapture("AFirst.avi", 0);
	m_capB = OpenCapture("BFirst.avi", 1);

	if (!m_capA.isOpened() || !m_capB.isOpened())
	{
		m_sourceA = cv::imread("AFirst.png");
		m_sourceB = cv::imread("BFirst.png");
	}

	cv::String loadedData = Utils::ReadAllTextFromFile(settingsFileName);
	if (loadedData.length() > 0)
	{
		nlohmann::json settings = nlohmann::json::parse(loadedData);

		if (settings["pointsA"].is_array())
		{
			std::vector<float> raw = settings["pointsA"];
			std::vector<cv::Point2f> perspectiveData;
			for (int i = 0; i < raw.size(); i+=2)
			{
				perspectiveData.push_back(cv::Point2f(raw[i], raw[i + 1]));
			}
			m_correctionA = new CorrectPerspective(perspectiveData);
		}

		if (settings["pointsB"].is_array())
		{
			std::vector<float> raw = settings["pointsB"];
			std::vector<cv::Point2f> perspectiveData;
			for (int i = 0; i < raw.size(); i += 2)
			{
				perspectiveData.push_back(cv::Point2f(raw[i], raw[i + 1]));
			}
			m_correctionB = new CorrectPerspective(perspectiveData);
		}

		if (settings["showEditor"].is_boolean())
		{
			m_showEditor = settings["showEditor"];
		}

		if (settings["showSourceImage"].is_boolean())
		{
			m_showSourceImage = settings["showSourceImage"];
		}

		if (settings["showCurrentImage"].is_boolean())
		{
			m_showCurrentImage = settings["showCurrentImage"];
		}
	}
	else
	{
		m_correctionA = new CorrectPerspective();
		m_correctionB = new CorrectPerspective();
	}
}

void ImageCorrection::Update()
{


    if( m_capA.isOpened() )
    {
        m_capA >> m_sourceA;
    }


	if( m_showSourceImage )
		imshow(m_nameA + " Source", m_sourceA);

    cv::Mat currentA(m_sourceA);

    currentA = UpdateCorrectionPerspective( m_nameA, currentA, m_correctionA );

	if (m_showCurrentImage)
		imshow(m_nameA + " current", currentA);


    if( m_capB.isOpened() )
    {
        m_capB >> m_sourceB;
    }

	if (m_showSourceImage)
		imshow(m_nameB + " Source", m_sourceB);

    cv::Mat currentB(m_sourceB);
    //currentB = UpdateCorrectionPerspective( m_nameB, currentB, m_correctionB );

	if(m_showCurrentImage)
		imshow(m_nameB + " current", currentB);

	cv::Mat images[]{ currentA, currentB };
	cv::Size sizesimage[]{ cv::Size(currentA.cols, currentA.rows),
						   cv::Size(currentB.cols, currentB.rows) };
	
}

cv::Mat ImageCorrection::UpdateCorrectionPerspective(cv::String sourceName, cv::Mat source, CorrectPerspective* corrector)
{
    if(m_showEditor)
    {
        corrector->UpdateEditor(sourceName + " Editor", source);
    }
    return corrector->UpdatePerspective(source, false);
}

void ImageCorrection::SaveSettings()
{
	nlohmann::json exportData;
	
	{
		std::vector<float> tempStore = std::vector<float>();
		const std::vector<cv::Point2f> localData = m_correctionA->GetCorrectionCordinates();
		std::vector<cv::Point2f>::const_iterator itor = localData.begin();
		for ( ; itor != localData.end(); ++itor)
		{
			tempStore.push_back(itor->x);
			tempStore.push_back(itor->y);
		}
		exportData["pointsA"] = tempStore;
	}
	{
		std::vector<float> tempStore = std::vector<float>();
		const std::vector<cv::Point2f> localData = m_correctionB->GetCorrectionCordinates();
		std::vector<cv::Point2f>::const_iterator itor = localData.begin();
		for (; itor != localData.end(); ++itor)
		{
			tempStore.push_back(itor->x);
			tempStore.push_back(itor->y);
		}
		exportData["pointsB"] = tempStore;
	}

	WrightTextToFile(settingsFileName, exportData.dump());
}

cv::Mat ImageCorrection::GetImageA()
{
	return m_correctionA->GetCurrentImage();
}

cv::Mat ImageCorrection::GetImageB()
{
	return m_correctionB->GetCurrentImage();
}

void ImageCorrection::WrightTextToFile(const char * path, std::string data)
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

cv::VideoCapture ImageCorrection::OpenCapture(cv::String source, int camera, bool forceCamera)
{
	cv::VideoCapture cap;
	if (forceCamera)
	{
		cap = cv::VideoCapture(camera);
	}
	else
	{
		cap = cv::VideoCapture(source);
		if (!cap.isOpened())
		{
			cap = cv::VideoCapture(camera);
		}
	}

	return cap;
}
