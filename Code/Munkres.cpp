#include "Munkres.h"



Munkres::Munkres(int size) : m_maxSize(size),
							 m_matrix(size, size), 
							 m_coveredCols(new int[size]),
							 m_coveredRows(new int[size])
{
	m_path = new int*[(size * 2) + 1];
	for (int i = 0; i < (size * 2) + 1; ++i)
	{
		m_path[i] = new int[2];
	}
}


Munkres::~Munkres()
{
}

std::vector< std::pair<int, int> > Munkres::CalculatePairs( std::vector<cv::Point> lastPoints,
							  std::vector<cv::Point> currentPoints)
{
	m_currentSize = lastPoints.size();

	if (lastPoints.size() != currentPoints.size())
	{
		m_currentSize = MAX(lastPoints.size(), currentPoints.size());
	}

	std::vector< std::pair<int, int> > returnData = std::vector< std::pair<int, int> >();
	for (int i = 0; i < m_currentSize; ++i)
	{
		std::pair<int, int> p = std::pair<int, int>();
		p.first = -1;
		p.second = -1;
		returnData.push_back(p);
	}

	if ((lastPoints.size() == 0 || currentPoints.size() == 0))
	{
		return returnData;
	}

	if (m_currentSize > m_maxSize)
	{
		std::cout << "Error current input data larger than max size, data lost\n";
			m_currentSize = m_maxSize;
	}


	m_matrix.setTo( cv::Scalar(0, -1) );
	for (int i = 0; i < m_maxSize; ++i)
	{
		m_coveredCols[i] = 0;
		m_coveredRows[i] = 0;
	}

	for (int i = 0; i < (m_maxSize * 2) + 1; ++i)
	{
		m_path[i][0] = 0;
		m_path[i][1] = 0;
	}

	m_tempPathX = -1;
	m_tempPathY = -1;

	for (int x = 0; x < lastPoints.size(); ++x)
	{
		for (int y = 0; y < currentPoints.size(); ++y)
		{
			m_matrix[x][y][0] = cv::norm(lastPoints[x] - currentPoints[y]);
		}
	}

	int breakoutCount = 0;
	bool brokeOut = false;
	bool done = false;
	int currentStep = 1;
	while (!done)
	{
		//std::cout << "\n\n" << m_matrix << "\n\n\n";

		if (breakoutCount > 2000)
		{
			currentStep = 7;
			std::cout << "\nBroke out\n";
			brokeOut = true;
		}

		switch (currentStep)
		{
		case 1:
			Step1(currentStep);
			break;
		case 2:
			Step2(currentStep);
			break;
		case 3:
			Step3(currentStep);
			break;
		case 4:
			Step4(currentStep);
			break;
		case 5:
			Step5(currentStep);
			break;
		case 6:
			Step6(currentStep);
			break;
		case 7:
			done = true;
			break;
		default:
			done = true;
			break;
		}

		++breakoutCount;
	}

	int starCount = 0;
	
	if (!brokeOut)
	{
		int count = 0;
		for (int x = 0; x < m_currentSize; ++x)
		{
			for (int y = 0; y < m_currentSize; ++y)
			{
				if (m_matrix[x][y][1] == 1)
				{
					if( count < lastPoints.size())
						returnData[count].first = x;
					if(count < currentPoints.size())
					returnData[count].second = y;
					++count;
				}
			}
		}
	}

	return returnData;
}

void Munkres::Step1(int & step)
{

	for (int y = 0; y < m_currentSize; ++y)
	{
		float min = m_matrix[0][y][0];
		for (int x = 0; x < m_currentSize; ++x)
		{
			if (m_matrix[x][y][0] < min)
			{
				min = m_matrix[x][y][0];
			}
		}
		for (int x = 0; x < m_currentSize; ++x)
		{
			m_matrix[x][y][0] -= min;
		}
	}
	step = 2;
}

void Munkres::Step2(int & step)
{
	for (int x = 0; x < m_currentSize; ++x)
	{
		for (int y = 0; y < m_currentSize; ++y)
		{
			if (m_matrix[x][y][0] == 0)
			{
				if (!CheckForRowAndCollumStars(x, y, m_matrix))
				{
					m_matrix[x][y][1] = 1;
				}
			}
		}
	}

	for (int i = 0; i < m_maxSize; ++i)
	{
		m_coveredCols[i] = 0;
		m_coveredRows[i] = 0;
	}

	step = 3;
}

void Munkres::Step3(int & step)
{
	int colCount = 0;
	for (int y = 0; y < m_currentSize; ++y)
	{
		for (int x = 0; x < m_currentSize; ++x)
		{
			if (m_matrix[x][y][1] == 1)
			{
				if (m_coveredCols[y] != 1)
					++colCount;
				m_coveredCols[y] = 1;
			}
		}
	}

	if (colCount >= m_currentSize)
	{
		step = 7;
		return;
	}

	step = 4;
}

void Munkres::Step4(int & step)
{
	int x, y = -1;
	bool done = false;

	while (!done)
	{
		if (!GetNonCoveredZero(x, y))
		{
			step = 6;
			done = true;
		}
		else
		{
			m_matrix[x][y][1] = 2;
			int starInRow = GetStarInRow(x);
			if ( starInRow >= 0 )
			{
				m_coveredRows[x] = 1;
				m_coveredCols[starInRow] = 0;
			}
			else
			{
				done = true;
				step = 5;
				m_tempPathX = x;
				m_tempPathY = y;
			}
		}
	}
}

void Munkres::Step5(int & step)
{
	int pathCount = 1;
	m_path[0][0] = m_tempPathX;
	m_path[0][1] = m_tempPathY;

	bool done = false;
	while (!done)
	{
		int foundStar = GetStarInCol(m_path[pathCount - 1][1]);
		if (foundStar > -1)
		{
			++pathCount;
			m_path[pathCount - 1][0] = foundStar;
			m_path[pathCount - 1][1] = m_path[pathCount - 2][1];
		}
		else
		{
			done = true;
		}

		if (!done)
		{
			int foundPrime = GetPrimeInRow(m_path[pathCount - 1][0]);
			++pathCount;
			m_path[pathCount - 1][0] = m_path[pathCount - 2][0];
			m_path[pathCount - 1][1] = foundPrime;
		}
	}

	for (int i = 0; i < pathCount; ++i)
	{
		if (m_matrix[ m_path[i][0] ] [m_path[i][1] ][1] == 1)
		{
			m_matrix[ m_path[i][0] ][ m_path[i][1] ][1] = 0;
		}
		else
		{
			m_matrix [m_path[i][0] ][m_path[i][1] ][1] = 1;
		}
	}

	for (int i = 0; i < m_maxSize; ++i)
	{
		m_coveredCols[i] = 0;
		m_coveredRows[i] = 0;
	}

	for (int x = 0; x < m_currentSize; ++x)
	{
		for (int y = 0; y < m_currentSize; ++y)
		{
			if (m_matrix[x][y][1] == 2)
			{
				m_matrix[x][y][1] = 0;
			}
		}
	}

	step = 3;
}

void Munkres::Step6(int & step)
{
	double min = m_matrix[0][0][0];
	for (int x = 0; x < m_currentSize; ++x)
	{
		for (int y = 0; y < m_currentSize; ++y)
		{
			if (m_coveredRows[x] == 0 && m_coveredCols[y] == 0)
			{
				if (m_matrix[x][y][0] < min)
				{
					min = m_matrix[x][y][0];
				}
			}
		}
	}

	for (int x = 0; x < m_currentSize; ++x)
	{
		for (int y = 0; y < m_currentSize; ++y)
		{
			if (m_coveredRows[x] == 1)
				m_matrix[x][y][0] += min;
			if (m_coveredCols[y] == 0)
				m_matrix[x][y][0] -= min;
		}
	}

	step = 4;
}


bool Munkres::CheckForRowAndCollumStars(const int x, const int y, const cv::Mat2f matrix)
{
	for (int row = 0; row < m_currentSize; ++row)
	{
		if (matrix[row][y][1] > 0)
		{
			return true;
		}
	}

	for (int col = 0; col < m_currentSize; ++col)
	{
		if (matrix[x][col][1] > 0)
		{
			return true;
		}
	}

	return false;
}

bool Munkres::GetNonCoveredZero(int &outX, int &outY)
{
	outX = -1;
	outY = -1;

	for (int x = 0; x < m_currentSize; ++x)
	{
		for (int y = 0; y < m_currentSize; ++y)
		{
			if (m_matrix[x][y][0] == 0 && m_coveredCols[y] == 0 && m_coveredRows[x] == 0)
			{
				outX = x;
				outY = y;
				return true;
			}
		}
	}
	return false;
}

int Munkres::GetStarInRow(const int & row)
{
	for (int y = 0; y < m_currentSize; ++y)
	{
		if (m_matrix[row][y][1] == 1)
		{
			return y;
		}
	}

	return -1;
}

int Munkres::GetStarInCol(const int & col)
{
	for (int x = 0; x < m_currentSize; ++x)
	{
		if (m_matrix[x][col][1] == 1)
		{
			return x;
		}
	}

	return -1;
}

int Munkres::GetPrimeInRow(const int & row)
{
	for (int y = 0; y < m_currentSize; ++y)
	{
		if (m_matrix[row][y][1] == 2)
		{
			return y;
		}
	}

	return -1;
}
