#include "ImageCorrection.h"


ImageCorrection::ImageCorrection()
{
}

ImageCorrection::~ImageCorrection()
{
	delete m_correctionA;
	delete m_correctionB;
	delete m_stitcher;
}

void ImageCorrection::Init()
{
	m_capA = OpenCapture("AFirst.avi", 0);
	m_capB = OpenCapture("BFirst.avi", 1);

	m_correctionA = new CorrectPerspective();
	m_correctionB = new CorrectPerspective();

	m_stitcher = new RealTimeStitcher();
	
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

	cv::Mat images[]{ currentA, currentB };
	cv::Size sizesimage[]{ cv::Size(currentA.cols, currentA.rows),
	cv::Size(currentB.cols, currentB.rows) };
	if (calculateStich)
	{
		m_stitcher->CalculateStich( images, sizesimage, 0.75, 4.0, false);
		calculateStich = false;
	}

	if (m_stitcher->CanStich())
	{
		cv::Mat compleateOutput = m_stitcher->StichImages(images, sizesimage);
		imshow("compleateOutput", compleateOutput);
	}
}

cv::Mat ImageCorrection::UpdateCorrectionPerspective(cv::String sourceName, cv::Mat source, CorrectPerspective* corrector)
{
    if( true )
    {
        corrector->UpdateEditor(sourceName + " Editor", source);
    }
    return corrector->UpdatePerspective(source);
}

void ImageCorrection::CalculateImageStitch()
{
	calculateStich = true;
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
