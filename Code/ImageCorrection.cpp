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

	m_correctionA = new CorrectPerspective();
	m_correctionB = new CorrectPerspective();
}

void ImageCorrection::Update()
{
    if( m_capA.isOpened() )
    {
        m_capA >> m_sourceA;
    }

    imshow(m_nameA + " Source", m_sourceA);

    cv::Mat currentA(m_sourceA);
    currentA = UpdateCorrectionPerspective( m_nameA, currentA, m_correctionA );

    imshow(m_nameA + " current", currentA);	

    if( m_capB.isOpened() )
    {
        m_capB >> m_sourceB;
    }

    imshow(m_nameB + " Source", m_sourceB);

    cv::Mat currentB(m_sourceB);
    currentB = UpdateCorrectionPerspective( m_nameB, currentB, m_correctionB );

    imshow(m_nameB + " current", currentB);
}

cv::Mat ImageCorrection::UpdateCorrectionPerspective(cv::String sourceName, cv::Mat source, CorrectPerspective* corrector)
{
    if( false )
    {
        corrector->UpdateEditor(sourceName + " Editor", source);
    }
    return corrector->UpdatePerspective(source);
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
