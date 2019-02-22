#include <iostream>
#include <string>
#include <time.h>

void incr(unsigned int nLoops, double *pCounter) {
  for (int i = 0; i < nLoops; i++) {
    *pCounter += 1;
  }
}

int main(int argc, char const *argv[]) {
  unsigned int nLoops = std::stoi(argv[1]);
  double counter = 0.0;

  incr(nLoops, &counter);
  std::cout << "Counter: " << counter << std::endl;
  return 0;
}
