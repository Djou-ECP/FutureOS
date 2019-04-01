#ifndef DEF_CHRONO
#define DEF_CHRONO

#include "../TD1/timespec.h"

class Chrono 
{
public:
	Chrono();
	void stop();
	void restart();
	bool isActive();
	double startTime();
	double stopTime();
	double lap();

private:
	timespec startTime_;
	timespec stopTime_;
};

#endif