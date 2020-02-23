#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "processor.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, Processor &);
  int Pid();
  void Pid(int pid);                              // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command(); 
  void SetCommand();
  float & CpuUtilization();                 // TODO: See src/process.cpp
  void CalculateCpuUtilization(Processor &);
  void Update(Processor &);
  std::string Ram();                       // TODO: See src/process.cpp 
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(const Process & a);  // TODO: See src/process.cpp
  float ram_;
  
  // TODO: Declare any necessary private members
 private:
    int pid_;
//  std::string user_;
    std::string command_;
    long prevActive_ = 0;
    float cpuUtilization_;
//  long int uptime_;
};

#endif