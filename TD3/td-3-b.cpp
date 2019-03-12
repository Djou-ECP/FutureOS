#include "td-3-b.h"
#include <iostream>

Timer::Timer() {
  // Timer initialisation
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = Timer::call_callback;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGRTMIN, &sa, NULL);

  struct sigevent sev;
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIGRTMIN;
  sev.sigev_value.sival_ptr = this;

  timer_create(CLOCK_REALTIME, &sev, &this->tid);
  // End timer initialisation
}

Timer::~Timer() { timer_delete(this->tid); }

void Timer::start(double duration_ms) {
  itimerspec its;
  its.it_value = timespec_from_ms(duration_ms);
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;
  timer_settime(this->tid, 0, &its, NULL);
}

void Timer::stop() {
  itimerspec its;
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 0;
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;
  timer_settime(this->tid, 0, &its, NULL);
}

void Timer::call_callback(int sig, siginfo_t *si, void *user) {
  Timer *timer_ptr = (Timer *)(si->si_value).sival_ptr;
  timer_ptr->callback();
}

void PeriodicTimer::start(double duration_ms) {
  itimerspec its;
  its.it_value = timespec_from_ms(duration_ms);
  its.it_interval = timespec_from_ms(duration_ms);
  timer_settime(this->tid, 0, &its, NULL);
}

CountDown::CountDown(int n) : base_counter(n) {}

void CountDown::start(double duration_ms) {
  this->counter = this->base_counter;
  std::cout << "Counter: " << this->counter << std::endl;
  PeriodicTimer::start(duration_ms);
}

void CountDown::callback() {
  this->counter--;
  if (this->counter < 0)
    this->stop();
  else
    std::cout << "Counter: " << this->counter << std::endl;
}
