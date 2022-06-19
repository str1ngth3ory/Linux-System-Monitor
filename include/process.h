#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long UpTime();                       
  bool operator<(Process const& a) const;  

 private:
  std::vector<std::string> prev_cpu_util;
  std::vector<std::string> curr_cpu_util;
  int pid_;
  std::string user_, command_;
  float cpu_utilization_;
};

#endif