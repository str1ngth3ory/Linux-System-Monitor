#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  Processor();
  float Utilization();

 private:
  std::vector<std::string> prev_cpu_util;
  std::vector<std::string> curr_cpu_util;
};

#endif