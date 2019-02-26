#include <iostream>
#include <pthread.h>
#include <string>
#include <time.h>

#include "../TD1/timespec.h"

struct counter {
  unsigned int nLoops;
  double *pCounter;
};

void incr(unsigned int nLoops, double *pCounter) {
  for (int i = 0; i < nLoops; i++) {
    *pCounter += 1;
  }
}

void *call_incr(void *v_counter) {
  volatile counter *p_counter = (volatile counter *)v_counter;
  incr(p_counter->nLoops, p_counter->pCounter);
}

int main(int argc, char const *argv[]) {

  /*
   Parsing arguments
  */
  unsigned int nLoops = 0;
  unsigned int nTasks = 0;
  if (argc > 2) {
    nLoops = std::stoi(argv[1]);
    nTasks = std::stoi(argv[2]);
  } else { // No argument, go for default value
    nLoops = 15;
    nTasks = 1;
  }

  double counter = 0;
  struct counter count = {nLoops, &counter};

  /*
    Instantiating threads
  */
  pthread_t threads[nTasks];

  timespec before = timespec_now();
  for (int i = 0; i < nTasks; i++) {
    pthread_create(&threads[i], NULL, call_incr, &count);
  }

  for (int i = 0; i < nTasks; i++) {
    pthread_join(threads[i], NULL);
  }
  timespec after = timespec_now();
  timespec duration = after - before;

  std::cout << "Counter: " << *count.pCounter << std::endl;
  std::cout << "Execution lasted "
            << (double)duration.tv_sec + (double)duration.tv_nsec * 1e-9
            << " seconds." << std::endl;

  return 0;
}
