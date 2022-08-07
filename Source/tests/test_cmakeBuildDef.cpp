#include <iostream>
#include "hdv_config.h"

int main()
{
  std::cout << "HDV-" << HDV::VERSION::MAJOR << '.' << HDV::VERSION::MINOR << '.' << HDV::VERSION::PATCH << '\n';
  return 0;
}