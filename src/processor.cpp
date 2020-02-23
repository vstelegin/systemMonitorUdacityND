#include <chrono>
#include <thread>
#include "processor.h"
#include "linux_parser.h"
#include <iostream>
// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
   return  utilization_; 
}

long Processor::TotalDifference(){
    return totalDifference_;
}
void Processor::CalculateUtilization(){
    long currentIdle = LinuxParser::IdleJiffies();
    long currentActive = LinuxParser::ActiveJiffies();
    long currentTotal = currentIdle + currentActive;
    long idleDifference = currentIdle - prevIdle_;
    totalDifference_ = currentTotal - prevTotal_;
    prevTotal_ = currentTotal;
    prevIdle_ = currentIdle;
    utilization_ = ( (float)(totalDifference_ - idleDifference) )/(float)totalDifference_;   
}