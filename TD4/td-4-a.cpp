#include "td-4-a.h"
#include "../TD1/timespec.h"
#include <time.h>

PosixThread::PosixThread() {
  posixId = INVALID_PTHREAD();
  pthread_attr_init(&posixAttr);
}

PosixThread::PosixThread(pthread_t posixId) : posixId(posixId) {
  pthread_attr_init(&posixAttr);
}

PosixThread::~PosixThread() {
  pthread_attr_destroy(&posixAttr);
  pthread_cancel(posixId);
}

void PosixThread::start(void *(*threadFunc)(void *), void *threadArg) {
  pthread_create(&posixId, &posixAttr, threadFunc, threadArg);
}

void PosixThread::join() { pthread_join(posixId, NULL); }

bool PosixThread::join(double timeout_ms) {
  timespec timeout = timespec_from_ms(timeout_ms);
  int retval = pthread_timedjoin_np(posixId, NULL, &timeout);

  if (retval != 0)
    return false;
  else
    return true;
}

bool PosixThread::setScheduling(int schedPolicy, int priority) {
  struct sched_param schedParam;
  schedParam.sched_priority = priority;
  pthread_setschedparam(posixId, schedPolicy, &schedParam);
}

bool PosixThread::getScheduling(int *p_schedPolicy = nullptr,
                                int *p_priority = nullptr) {
  struct sched_param schedParam;
  int *l_schedPolicy;
  pthread_getschedparam(posixId, l_schedPolicy, &schedParam);
  p_priority = &schedParam.sched_priority;
  p_schedPolicy = l_schedPolicy;
}

void *PosixThread::dummyFunction(void *) { PosixThread::hasFinished = true; }

pthread_t PosixThread::INVALID_PTHREAD() {
  if (INVALID_PTHREAD_ == 0) {
    PosixThread::hasFinished = false;
    pthread_create(&INVALID_PTHREAD_, nullptr, dummyFunction, nullptr);
    while (!hasFinished) {
      pthread_yield();
    }
  }
  return INVALID_PTHREAD_;
}