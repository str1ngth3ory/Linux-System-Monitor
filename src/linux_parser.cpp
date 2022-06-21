#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

#define Hertz sysconf(_SC_CLK_TCK);

// Read and return operating system
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return kernel
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// vector<int> LinuxParser::Pids() {
//   vector<int> pids;
//   DIR* directory = opendir(kProcDirectory.c_str());
//   struct dirent* file;
//   while ((file = readdir(directory)) != nullptr) {
//     // Is this a directory?
//     if (file->d_type == DT_DIR) {
//       // Is every character of the name a digit?
//       string filename(file->d_name);
//       if (std::all_of(filename.begin(), filename.end(), isdigit)) {
//         int pid = stoi(filename);
//         pids.push_back(pid);
//       }
//     }
//   }
//   closedir(directory);
//   return pids;
// }

// Read and return process IDs
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  for (auto entry : std::filesystem::directory_iterator(kProcDirectory)) {
    string filename{entry.path()};
    if (std::all_of(filename.begin()+6, filename.end(), isdigit)) {
      int pid = stoi(filename.substr(6));
      pids.push_back(pid);
    }
  }
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  long MemTotal{0};
  long MemFree{0};
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "MemTotal:") {
        linestream >> value;
        MemTotal = std::stol(value);
      }
      else if (key == "MemFree:") {
        linestream >> value;
        MemFree = std::stol(value);
        break;
      }
    }
  }
  return static_cast<float>(MemTotal - MemFree) / MemTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long upTime;
  string line, info_value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> info_value;
    upTime = std::stol(info_value);
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() {
//   return 0;
// }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() {
//   long output;
//   return 0;
// }

// Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() {
//   long output;
//   return 0;
// }


// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_usage(10,"");
  string line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        linestream >> cpu_usage[CPUStates::kUser_];
        linestream >> cpu_usage[CPUStates::kNice_];
        linestream >> cpu_usage[CPUStates::kSystem_];
        linestream >> cpu_usage[CPUStates::kIdle_];
        linestream >> cpu_usage[CPUStates::kIOwait_];
        linestream >> cpu_usage[CPUStates::kIRQ_];
        linestream >> cpu_usage[CPUStates::kSoftIRQ_];
        linestream >> cpu_usage[CPUStates::kSteal_];
        linestream >> cpu_usage[CPUStates::kGuest_];
        linestream >> cpu_usage[CPUStates::kGuestNice_];
        break;
      }
    } 
  }
  return cpu_usage;
}

// Read and return process CPU utilization
vector<string> LinuxParser::CpuUtilization(int pid) {
  vector<string> cpu_usage(5,"0");
  string line, place_holder;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i=0; i<13; i++) linestream >> place_holder;
    linestream >> cpu_usage[ProcessCPUStates::kutime_];
    linestream >> cpu_usage[ProcessCPUStates::kstime_];
    linestream >> cpu_usage[ProcessCPUStates::kcutime_];
    linestream >> cpu_usage[ProcessCPUStates::kcstime_];
    for (int i=0; i<4; i++) linestream >> place_holder;
    linestream >> cpu_usage[ProcessCPUStates::kstarttime_];
  }
  return cpu_usage;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int output;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> value;
        output = std::stoi(value);
        break;
      }
    }
  }
  return output;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int output;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> value;
        output = std::stoi(value);
        break;
      }
    }  
  }
  return output;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  string output;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> output;
    return output;
  }
  return "";
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmRSS:") {
        linestream >> value;
        value = std::to_string(std::stoi(value) / 1024);
        break;
      }
    } 
  }
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> value;
        break;
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid{Uid(pid)};
  string line;
  string key, value, place_holder;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> value >> place_holder >> key;
      if (key == uid) break;
    }
  }
  return value;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long system_uptime = UpTime();
  string line;
  string value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++) {
      linestream >> value;
    }
    return system_uptime - std::stol(value)/Hertz;
  }
  return 0;
}