#include "td-4-a.h"
#include "../TD1/timespec.h"
#include <time.h>

// POSIX THREAD

PosixThread::PosixThread() 
{
  posixId = INVALID_PTHREAD();
  pthread_attr_init(&posixAttr);
}

PosixThread::PosixThread(pthread_t posixId) : posixId(posixId) 
{
  pthread_attr_init(&posixAttr);
}

PosixThread::~PosixThread() 
{
  pthread_attr_destroy(&posixAttr);
  pthread_cancel(posixId);
}

void PosixThread::start(void *(*threadFunc)(void *), void *threadArg) 
{
  pthread_create(&posixId, &posixAttr, threadFunc, threadArg);
}

void PosixThread::join() 
{ 
  pthread_join(posixId, NULL); 
}

bool PosixThread::join(double timeout_ms) 
{
  timespec timeout = timespec_from_ms(timeout_ms);
  int retval = pthread_timedjoin_np(posixId, NULL, &timeout);

  if (retval != 0)
    return false;
  else
    return true;
}

bool PosixThread::setScheduling(int schedPolicy, int priority) 
{
  struct sched_param schedParam;
  schedParam.sched_priority = priority;
  pthread_setschedparam(posixId, schedPolicy, &schedParam);
}

bool PosixThread::getScheduling(int *p_schedPolicy = nullptr, int *p_priority = nullptr) 
{
  struct sched_param schedParam;
  int *l_schedPolicy;
  bool retVal = pthread_getschedparam(posixId, l_schedPolicy, &schedParam);
  if (p_priority)
    p_priority = &schedParam.sched_priority;
  if (p_schedPolicy)
    p_schedPolicy = l_schedPolicy;
  return retVal;
}

void *PosixThread::dummyFunction(void *) 
{ 
  PosixThread::hasFinished = true; 
}

pthread_t PosixThread::INVALID_PTHREAD() 
{
  if (INVALID_PTHREAD_ == 0) 
  {
    PosixThread::hasFinished = false;
    pthread_create(&INVALID_PTHREAD_, nullptr, dummyFunction, nullptr);
    while (!hasFinished) 
    {
      pthread_yield();
    }
  }
  return INVALID_PTHREAD_;
}

// THREAD

Thread::Thread(): startTime(0), stopTime(0) 
{

}

Thread::~Thread() 
{

}

double Thread::startTime_ms() 
{
  return startTime;
}

double Thread::stopTime_ms() 
{
  return stopTime;
}

double Thread::execTime_ms() 
{
  if (stopTime != 0 && startTime != 0)
    return stopTime - startTime;
  return 0;
}

void Thread::run() 
{
  // user code here
  std::cout << "I'm a thread running" << std::endl;
}

void *Thread::call_run(void *v_thread) 
{
  Thread t = * (Thread*) v_thread;
  t.run();
}


void Thread::start() 
{
  startTime = timespec_to_ms(timespec_now());
  call_run(this);
}