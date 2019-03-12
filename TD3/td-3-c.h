#ifndef DEF_CALIBRATOR
#define DEF_CALIBRATOR

#include "../TD1/timespec.h"
#include "td-3-b.h"
#include <vector>

class Looper {
public:
  double runLoop(double nLoops);
  double getSample();
  double stopLoop();

protected:
private:
  bool doStop;
  double iLoop;
};

class Calibrator : public PeriodicTimer {
public:
  Calibrator(double samplingPeriod_ms, unsigned int nSamples);
  double nLoops(double duration_ms);

protected:
  void callback();

private:
  void computeCalibration();
  double a;
  double b;
  Looper looper;
  const unsigned int nSamples;
  double samplingPeriod_ms;
  std::vector<std::pair<double, double>> samples;
};

class CpuLoop : public Looper {
public:
  CpuLoop(Calibrator &);
  void runTime(double duration_ms);

private:
  Calibrator &calibrator;
};

#endif