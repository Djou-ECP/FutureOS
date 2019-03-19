#ifndef DEF_THREAD
#define DEF_THREAD

#include <iostream>
#include <pthread.h>
#include "../TD1/timespec.h"

class PosixThread {
public:
  class Exception;
  PosixThread();
  PosixThread(pthread_t posixId);
  ~PosixThread();
  void start(void *(*threadFunc)(void *), void *threadArg);
  void join();
  bool join(double timeout_ms);
  bool setScheduling(int schedPolicy, int priority);
  bool getScheduling(int *p_schedPolicy = nullptr, int *p_priority = nullptr);
  static pthread_t INVALID_PTHREAD();

private:
  pthread_t posixId;
  pthread_attr_t posixAttr;
  static pthread_t INVALID_PTHREAD_;
  static void *dummyFunction(void *);
  static volatile bool hasFinished;
};

class Thread: public PosixThread {
public:
  Thread();
  ~Thread();
  void start();
  double startTime_ms();
  double stopTime_ms();
  double execTime_ms();
  static void sleep_ms(double delay_ms);

protected:
  void run();

private:
  static void *call_run(void *v_thread);
  double startTime;
  double stopTime;
};
#endif