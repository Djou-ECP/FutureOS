#include <iostream>
#include "td1.h"

using namespace std;

void myHandler(int sig, siginfo_t* si, void*) {
    if(si->si_value < 15) {
        std::cout << "Compteur" << si->si_value << std::endl;
        si->si_value++;
    } else {
        timer_delete(sig);
    }
}

int main(int argc, char const *argv[]) {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = myHandler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, NULL);

    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    int compteur = 0;

    sev.sigev_value.sival_ptr = &compteur;

    timer_t tid;
    timer_create(CLOCK_REALTIME, &sev, &tid);
    itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 500000000;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 500000000;

    timer_settime(tid, 0, &its, NULL);

    return 0;
}
