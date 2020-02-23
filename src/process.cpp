#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;
using std::stof;

Process :: Process(int pid, Processor & cpu){
    Pid(pid);
    Ram();
    SetCommand();
    CalculateCpuUtilization(cpu);
};

void Process::Update(Processor & cpu){
    Ram();
    CalculateCpuUtilization(cpu);
}
// Return this process's ID
int Process::Pid() { return pid_; }
void Process::Pid(int pid) { pid_ = pid; }

// Return this process's CPU utilization
float & Process::CpuUtilization() {
    return cpuUtilization_;
}

void Process::CalculateCpuUtilization(Processor &cpu) {
    long currentActive = LinuxParser::ActiveJiffies(pid_);
    double totalDifference = double(cpu.TotalDifference());
    double activeDifference = 0.0;
    long processJiffiesDifference = currentActive - prevActive_;
    activeDifference = (totalDifference - processJiffiesDifference)/totalDifference;
    activeDifference = 1.0 - activeDifference;
    double hertz = sysconf(_SC_CLK_TCK);
    cpuUtilization_ = hertz * activeDifference;
    prevActive_ = currentActive;
}
// Return the command that generated this process
string Process::Command() {
    return command_;
}

void Process::SetCommand(){
    command_ = LinuxParser::Command(pid_);
}
// Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(pid_, ram_); 
}

// Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid_);
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const &a) {
    return a.cpuUtilization_ < cpuUtilization_;
}