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
  void Pid(int pid);                              
  std::string User();                      
  std::string Command(); 
  void SetCommand();
  float & CpuUtilization();                
  void CalculateCpuUtilization(Processor &);
  void Update(Processor &);
  std::string Ram();                       
  long int UpTime();                      
  bool operator<(const Process & a); 
  float ram_;
  

 private:
    int pid_;
    std::string command_;
    long prevActive_ = 0;
    float cpuUtilization_;
};

#endif