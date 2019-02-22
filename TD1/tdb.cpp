#include <iostream>
#include <string>
#include <time.h>

void incr(unsigned int nLoops, double *pCounter) {
  for (int i = 0; i < nLoops; i++) {
    *pCounter += 1;
  }
}

int main(int argc, char const *argv[]) {

  unsigned int nLoops = 0;
  if (argc > 1) {
      nLoops = std::stoi(argv[1]);
  } else { // No argument, go for default value
      nLoops = 15;
  }

  double counter = 0.0;

  incr(nLoops, &counter);
  std::cout << "Counter: " << counter << std::endl;
  return 0;
}
