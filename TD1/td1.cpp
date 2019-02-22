#include <iostream>
#include <time.h>
#include <signal.h>
#include <math.h>

using namespace std;

double timespec_to_ms(const timespec& time_ts){
    return time_ts.tv_sec*1000 + time_ts.tv_nsec/1000000.0;
}


timespec timespec_from_ms(double time_ms) {
    timespec time_ts;
    time_ts.tv_sec = floor(time_ms/1000);
    if(time_ts.tv_sec  >= 0)
        time_ts.tv_nsec = round((time_ms - time_ts.tv_sec*1000)*1000000);
    else
        time_ts.tv_nsec = 1000000000 - round((time_ms - time_ts.tv_sec*1000)*1000000);
    return time_ts;
}

timespec timespec_now() {
    timespec t;
    timespec_get(&t, TIME_UTC);
    return t;
}

timespec timespec_negate(const timespec& time_ts) {
    timespec t;
    if(time_ts.tv_nsec == 0) {
        t.tv_sec = -time_ts.tv_sec;
        t.tv_nsec =  0;
    } else {
        t.tv_sec = -time_ts.tv_sec - 1;
        t.tv_nsec = 1000000000 - time_ts.tv_nsec;
    }
    return t;
}

timespec timespec_add(const timespec& time1_ts, const timespec& time2_ts) {
    timespec t;
    int retenue = 0;

    int nsec = time1_ts.tv_nsec + time2_ts.tv_nsec;

    if(nsec >= 1000000000) {
        retenue++;
        nsec = nsec - 1000000000;
    }

    t.tv_sec = retenue + time1_ts.tv_sec + time2_ts.tv_sec;
    t.tv_nsec = nsec;
    return t;
}

timespec timespec_subtract(const timespec& time1_ts, const timespec& time2_ts) {
    return timespec_add(time1_ts, timespec_negate(time2_ts));
}

timespec timespec_wait(const timespec& delay_ts, bool force) {
    timespec rem = delay_ts;
    if(force)
        while(nanosleep(&rem, &rem) == -1);
    else
        nanosleep(&rem,&rem);
    return rem;
}

timespec operator- (const timespec& time_ts){
    return timespec_negate(time_ts);
}

timespec operator+ (const timespec& time1_ts, const timespec& time2_ts){
    return timespec_add(time1_ts, time2_ts);
}

timespec operator- (const timespec& time1_ts, const timespec& time2_ts){
    return timespec_subtract(time1_ts, time2_ts);
}

timespec& operator+= (timespec& time_ts, const timespec& delay_ts) {
    time_ts = time_ts + delay_ts;
    return time_ts;
}

timespec& operator-= (timespec& time_ts, const timespec& delay_ts) {
    time_ts = time_ts - delay_ts;
    return time_ts;
}

bool operator== (const timespec& time1_ts, const timespec& time2_ts) {
    return (time1_ts.tv_sec == time2_ts.tv_sec) && (time1_ts.tv_nsec == time2_ts.tv_nsec);
}

bool operator!= (const timespec& time1_ts, const timespec& time2_ts){
    return !(time1_ts == time2_ts);
}

bool operator< (const timespec& time1_ts, const timespec& time2_ts){
    return (time1_ts.tv_sec < time2_ts.tv_sec) || ((time1_ts.tv_sec == time2_ts.tv_sec) && (time1_ts.tv_nsec < time2_ts.tv_nsec));
}

bool operator> (const timespec& time1_ts, const timespec& time2_ts){
    return time2_ts < time1_ts;
}
