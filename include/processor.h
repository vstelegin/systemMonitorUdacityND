#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  long TotalDifference();
  void CalculateUtilization();

 private:
  long prevTotal_ = 0;
  long prevIdle_ = 0;
  long totalDifference_ = 0;
  float utilization_ = 0.0f;
};

#endif