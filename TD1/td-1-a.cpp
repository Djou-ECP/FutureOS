#include "timespec.h"
#include <iostream>

using namespace std;

void incrHandler(int sig, siginfo_t *si, void *) 
{
  int *pcompteur = (int *)(si->si_value).sival_ptr;
  cout << "Compteur: " << *pcompteur << endl;
  *pcompteur += 1;
}

int main(int argc, char const *argv[]) 
{
  int limit = 15;

  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = incrHandler;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGRTMIN, &sa, NULL);

  struct sigevent sev;
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIGRTMIN;
  int pcompteur = 0;

  sev.sigev_value.sival_ptr = &pcompteur;

  timer_t tid;
  timer_create(CLOCK_REALTIME, &sev, &tid);
  itimerspec its;
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 500000000;
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 500000000;

  timer_settime(tid, 0, &its, NULL);
  while (pcompteur < limit) 
  {
    // Wait for compteur to reach limit
  }
  timer_delete(tid);
  cout << "Valeur finale: " << pcompteur << endl;

  return 0;
}
