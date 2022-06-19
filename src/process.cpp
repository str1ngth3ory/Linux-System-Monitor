#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

#define Hertz sysconf(_SC_CLK_TCK);

Process::Process(int pid) : prev_cpu_util(5,"0"), curr_cpu_util(5,"0"), pid_(pid) {
  user_ = LinuxParser::User(pid_);
  command_ = LinuxParser::Command(pid_);
}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  prev_cpu_util = curr_cpu_util;
  curr_cpu_util = LinuxParser::CpuUtilization(pid_);
  long curr_procTime {0};
  long prev_procTime {0};
  for (int i=0; i<4; i++) {
    curr_procTime += std::stol(curr_cpu_util[i])/Hertz;
  }
  for (int i=0; i<4; i++) {
    prev_procTime += std::stol(prev_cpu_util[i])/Hertz;
  }
  long sUpTime = (std::stol(curr_cpu_util[LinuxParser::ProcessCPUStates::kstarttime_]) - std::stol(prev_cpu_util[LinuxParser::ProcessCPUStates::kstarttime_])) / Hertz;
  return (curr_procTime - prev_procTime) / sUpTime;
}

// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return cpu_utilization_ < a.cpu_utilization_;
}