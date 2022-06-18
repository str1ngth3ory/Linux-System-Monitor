#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hh;
  int mm, ss;
  ss = seconds % 60;
  mm = (seconds / 60) % 60;
  hh = seconds / 3600;
  return std::to_string(hh) + ":" + std::to_string(mm) + ":" + std::to_string(ss);
}