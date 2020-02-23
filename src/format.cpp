#include <string>
#include <iomanip>
#include <sstream>
#include "format.h"

using std::string;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int h = seconds / 3600;
    int hs = h * 3600;
    int m = (seconds - hs) / 60;
    int s = seconds - m * 60 - hs; 
    std::ostringstream hh;
    hh << std::setw(2) << std::setfill('0') << h;
    std::ostringstream mm;
    mm << std::setw(2) << std::setfill('0') << m;
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << s;

    
    return hh.str() + ":" + mm.str() + ":" + ss.str();
    }