#include <iostream>
#include <string>
#include <time.h>

#include "timespec.h"

void switchHandler(int sig, siginfo_t *si, void *) {
  bool *stop_ptr = (bool *)(si->si_value).sival_ptr;
  *stop_ptr = true;
}

void startTimer(int sec, int nsec, volatile bool *pStop, timer_t *tid) {
  itimerspec its;
  its.it_value.tv_sec = sec;
  its.it_value.tv_nsec = nsec;
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;
  timer_settime(*tid, 0, &its, NULL);
}

void incr(unsigned int nLoops, double *pCounter, volatile bool *pStop) {
  for (int i = 0; i < nLoops && !*pStop; i++) {
    *pCounter += 1;
  }
}

int main(int argc, char const *argv[]) {
  volatile bool stop = false;

  // Timer initialisation
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = switchHandler;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGRTMIN, &sa, NULL);

  struct sigevent sev;
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIGRTMIN;
  sev.sigev_value.sival_ptr = const_cast<bool *>(&stop);

  timer_t tid;
  timer_create(CLOCK_REALTIME, &sev, &tid);
  // End timer initialisation

  unsigned int nLoops = UINT_MAX;

  double counter = 0.0;
  std::cout << "Starting 1s timer..." << std::endl;
  startTimer(1, 0, &stop, &tid);
  incr(nLoops, &counter, &stop);
  std::cout << "Counter: " << counter << std::endl;

  stop = false;
  counter = 0;

  std::cout << "Starting 5s timer..." << std::endl;
  startTimer(5, 0, &stop, &tid);
  incr(nLoops, &counter, &stop);
  std::cout << "Counter: " << counter << std::endl;

  return 0;
}
