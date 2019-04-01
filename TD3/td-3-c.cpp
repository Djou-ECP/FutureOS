#include "td-3-c.h"
#include <iostream>
#include <limits>
#include <pthread.h>

Calibrator::Calibrator(double samplingPeriod_ms, unsigned int nSamples): PeriodicTimer(), nSamples(nSamples), samplingPeriod_ms(samplingPeriod_ms) 
{
	samples.reserve(nSamples);
	start(samplingPeriod_ms);
	looper.runLoop(std::numeric_limits<double>::max());
}

void Calibrator::computeCalibration() 
{
	// calcul de a et b

	// Calcul des moyennes
	double xTempMean = 0;
	double yTempMean = 0;
	if (nSamples < 2)
		std::cout << "Error: not enough samples !" << std::endl;
	for (const auto &sample_pair : samples) 
	{
		yTempMean += sample_pair.first;
		xTempMean += sample_pair.second;
	}
	double xMean = xTempMean / nSamples;
	double yMean = yTempMean / nSamples;

	// Calcul des variances/covariances
	double cov = 0;
	double varx = 0;
	for (const auto &sample_pair : samples) 
	{
		cov += (sample_pair.first - yMean) * (sample_pair.second - xMean);
		varx += (sample_pair.second - xMean) * (sample_pair.second - xMean);
	}

	a = cov / varx;
	b = yMean - a * xMean;
	std::cout << "Calibration is over a:" << a << " b:" << b << std::endl;
}

void Calibrator::callback() 
{
	double sample;
	if (samples.size() < nSamples - 1) 
	{
		sample = looper.getSample();
	} 
	else 
	{
		sample = looper.stopLoop();
	}
	samples.push_back(
			std::make_pair(sample, (samples.size() + 1) * samplingPeriod_ms));
	if (samples.size() == nSamples)
		computeCalibration();
}

double Calibrator::nLoops(double duration_ms) 
{ 
	return a * duration_ms + b; 
}

double Looper::runLoop(double nLoops) 
{
	iLoop = 0;
	doStop = false;
	while (iLoop < nLoops && !doStop) 
	{
		iLoop++;
	}
}

double Looper::getSample() 
{ 
	return iLoop; 
}

double Looper::stopLoop() 
{
	doStop = true;
	return getSample();
}

CpuLoop::CpuLoop(Calibrator &calibrator) : calibrator(calibrator) 
{

}

void CpuLoop::runTime(double duration_ms) 
{
	Looper::runLoop(calibrator.nLoops(duration_ms));
}