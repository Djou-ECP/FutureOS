#include "td-3-a.h"
#include "td-3-b.h"

#include <iostream>

int main(int argc, char const *argv[]) {
  Chrono chrono = Chrono();
  std::cout << chrono.lap() << std::endl;
  return 0;
}
