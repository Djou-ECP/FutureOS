#ifndef DEF_TIMER
#define DEF_TIMER

#include "../TD1/timespec.h"

class Timer {
public:
  Timer();
  ~Timer();
  virtual void start(double duration_ms);
  void stop();

protected:
  timer_t tid;
  virtual void callback() = 0;

private:
  static void call_callback(int sig, siginfo_t *si, void *user);
};

class PeriodicTimer : Timer {
public:
  void start(double duration_ms);
};

#endif