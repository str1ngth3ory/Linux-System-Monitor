#include <string>
#include <iomanip>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hh;
  int mm, ss;
  ss = seconds % 60;
  mm = (seconds / 60) % 60;
  hh = seconds / 3600;
  std::ostringstream linestream;
  linestream << std::setw(2) << std::setfill('0') << std::to_string(hh) << ':'
             << std::setw(2) << std::setfill('0') << std::to_string(mm) << ':'
             << std::setw(2) << std::setfill('0') << std::to_string(ss);

  return linestream.str();
}