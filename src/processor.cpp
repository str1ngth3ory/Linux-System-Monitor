#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
Processor::Processor() : prev_cpu_util(10, "0"), curr_cpu_util(10, "0") {  
}

float Processor::Utilization() {
  prev_cpu_util = curr_cpu_util;
  curr_cpu_util = LinuxParser::CpuUtilization();
  long idle = std::stol(curr_cpu_util[LinuxParser::CPUStates::kIdle_])
            + std::stol(curr_cpu_util[LinuxParser::CPUStates::kIOwait_]);
  long prev_idle = std::stol(prev_cpu_util[LinuxParser::CPUStates::kIdle_])
                 + std::stol(prev_cpu_util[LinuxParser::CPUStates::kIOwait_]);
  long active = std::stol(curr_cpu_util[LinuxParser::CPUStates::kUser_])
              + std::stol(curr_cpu_util[LinuxParser::CPUStates::kNice_])
              + std::stol(curr_cpu_util[LinuxParser::CPUStates::kSystem_])
              + std::stol(curr_cpu_util[LinuxParser::CPUStates::kIRQ_])
              + std::stol(curr_cpu_util[LinuxParser::CPUStates::kSoftIRQ_])
              + std::stol(curr_cpu_util[LinuxParser::CPUStates::kSteal_]);
  long prev_active = std::stol(prev_cpu_util[LinuxParser::CPUStates::kUser_])
                   + std::stol(prev_cpu_util[LinuxParser::CPUStates::kNice_])
                   + std::stol(prev_cpu_util[LinuxParser::CPUStates::kSystem_])
                   + std::stol(prev_cpu_util[LinuxParser::CPUStates::kIRQ_])
                   + std::stol(prev_cpu_util[LinuxParser::CPUStates::kSoftIRQ_])
                   + std::stol(prev_cpu_util[LinuxParser::CPUStates::kSteal_]);
  return (active - prev_active) / (active + idle - prev_active - prev_idle);
}