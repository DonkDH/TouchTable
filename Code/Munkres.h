#pragma once
#include <vector>
#include <iostream>
#include <opencv2/core.hpp>

class Munkres
{
public:
	Munkres(int size);
	~Munkres();

	std::vector<std::pair<int, int> > CalculatePairs( std::vector<cv::Point> lastPoints,
						 std::vector<cv::Point> currentPoints );

private:
	void Step1(int &step);
	void Step2(int &step);
	void Step3(int &step);
	void Step4(int &step);
	void Step5(int &step);
	void Step6(int &step);

	bool CheckForRowAndCollumStars(const int x, const int y, const cv::Mat2f matrix);
	bool GetNonCoveredZero(int &outX, int &outY);
	int GetStarInRow(const int &row);
	int GetStarInCol(const int &col);
	int GetPrimeInRow(const int &row);

private:
	int m_maxSize;
	int m_currentSize;
	cv::Mat2f m_matrix;
	int* m_coveredCols;
	int* m_coveredRows;
	int** m_path;
	int m_tempPathX = -1;
	int m_tempPathY = -1;
};

