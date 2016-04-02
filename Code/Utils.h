#pragma once


#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

class Utils
{
public:
	Utils();
	~Utils();

	static float GetTimeInMiliseconds();
};

