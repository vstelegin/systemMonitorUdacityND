#include <string>
#include <iomanip>
#include <sstream>
#include "format.h"

using std::string;
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
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