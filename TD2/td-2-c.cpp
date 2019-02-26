#include <iostream>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include <time.h>

#include "../TD1/timespec.h"

unsigned int nLoops;
unsigned int nTasks;
int priority;
bool useMutex = false;

struct counter {
  unsigned int nLoops;
  double *pCounter;
  pthread_mutex_t *mutex;
};

void incr(unsigned int nLoops, double *pCounter, pthread_mutex_t *mutex) {
  for (int i = 0; i < nLoops; i++) {
    pthread_mutex_lock(mutex);
    *pCounter += 1;
    pthread_mutex_unlock(mutex);
  }
}

void *call_incr(void *v_counter) {
  volatile counter *p_counter = (volatile counter *)v_counter;

  incr(p_counter->nLoops, p_counter->pCounter, p_counter->mutex);
}

int main(int argc, char const *argv[]) {

  /*
   Parsing arguments
  */
  if (argc > 3) {
    // Parsing amount of loops
    nLoops = std::stoi(argv[1]);

    // Parsing amount of threads
    nTasks = std::stoi(argv[2]);

    // Parsing priority schedule
    if (strcmp(argv[3], "SCHED_RR") && strcmp(argv[3], "SCHED_FIFO") &&
        strcmp(argv[3], "SCHED_OTHER")) {
      std::cout << "Invalid scheduling parameter, must be 'SCHED_RR', "
                   "'SCHED_FIFO' or 'SCHED_OTHER'"
                << std::endl;
      return 1;
    } else {
      if (!strcmp(argv[3], "SCHED_RR"))
        priority = SCHED_RR;
      if (!strcmp(argv[3], "SCHED_FIFO"))
        priority = SCHED_FIFO;
      if (!strcmp(argv[3], "SCHED_OTHER"))
        priority = SCHED_OTHER;
    }

    // Checking if protected mode is enabled
    if (!strcmp(argv[4], "protected"))
      useMutex = true;

  } else {
    // No argument, go for default value
    nLoops = 15;
    nTasks = 1;
    priority = SCHED_OTHER;
  }

  /*
    Setting main thread's priority
  */

  struct sched_param schedParam;
  schedParam.sched_priority = sched_get_priority_max(priority);
  pthread_setschedparam(pthread_self(), priority, &schedParam);

  /*
    Instantiating thread priority attributes
  */
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  sched_param schedParams;
  schedParams.sched_priority = fmax(sched_get_priority_max(priority) - 1, 0);
  pthread_attr_setschedparam(&attr, &schedParams);

  /*
    Instantiating threads
  */
  pthread_t threads[nTasks];
  double counter = 0;
  struct counter count = {nLoops, &counter};
  if (useMutex) // when protected mode is enabled
  {
    count.mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(count.mutex, nullptr);
  }

  timespec before = timespec_now();
  for (int i = 0; i < nTasks; i++) {
    pthread_create(&threads[i], &attr, call_incr, &count);
  }

  /*
    Waiting for the program to finish
  */
  for (int i = 0; i < nTasks; i++) {
    pthread_join(threads[i], NULL);
  }
  if (useMutex) // when protected mode is enabled
  {
    pthread_mutex_destroy(count.mutex);
    free(count.mutex);
  }
  timespec after = timespec_now();
  timespec duration = after - before;

  /*
    Printing data
  */
  std::cout << "Counter: " << *count.pCounter << std::endl;
  std::cout << "Execution lasted "
            << (double)duration.tv_sec + (double)duration.tv_nsec * 1e-9
            << " seconds." << std::endl;

  return 0;
}
