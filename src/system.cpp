#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// Return the system's kernel identifier (string)
std::string System::Kernel() {
    return LinuxParser::Kernel();
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization();
}

// Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses();
}

// Return the total number of processes on the system
int System::TotalProcesses() {
    return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long System::UpTime() {
    return LinuxParser::UpTime();
}

void System::UpdateCpu(){
    cpu_.CalculateUtilization();
}

void System::LoadProcesses() {
    vector<int> pids = LinuxParser::Pids();
    for (auto pid : pids){
        bool newProcess = true;
        for (auto & process : processes_){
            if (process.Pid() == pid){
                process.Update(cpu_);
                newProcess = false;
                break;
            }
        }
        if (newProcess){
            Process process = Process(pid, cpu_);
            processes_.push_back(process); 
        }          
    }
    sort (processes_.begin(), processes_.end());
}