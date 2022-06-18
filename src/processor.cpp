#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
Processor::Processor() : prev_cpu_util(10, 0), curr_cpu_util(10, 0) {  
}

float Processor::Utilization() {
  prev_cpu_util = curr_cpu_util;
  curr_cpu_util = LinuxParser::CpuUtilization();
  return 0.0;
}