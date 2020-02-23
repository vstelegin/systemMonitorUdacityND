#include <dirent.h>
#include <unistd.h>
#include <vector>
#include <iomanip>
#include "linux_parser.h"
#include <iostream>
using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::getline;
using std::replace;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream ss(line);
    ss >> os >> kernel >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string memTotal, memFree; 
  string line;
  ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    StringFromStreamLine(stream, memTotal, 1);
    StringFromStreamLine(stream, memFree, 1);
  }
  float memTotalF = std::stof(memTotal);
  return (memTotalF - std::stof(memFree)) / memTotalF; 
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long systemTime;
  string line;
  ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream ss(line);
    ss >> systemTime;
  }
  stream.close();
  return systemTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string kPid = to_string(pid);
  ifstream stream(kProcDirectory + kPid + kSlash + kStatFilename);
  string line;
  long utime, stime, cutime, cstime;
  if (stream.is_open()){
    getline(stream, line);
    istringstream ss(line);
    ss.ignore(300, ')');
    SkipStreamElements(ss, 12);
    ss >> utime >> stime >> cutime >> cstime;
  }
  return utime + stime + cutime + cstime;
  }

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string user, nice, system, line;
  // string idle, iowait;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    getline(stream,line);
    istringstream ss(line);
    ss.ignore(3,' ');
    ss >> user >> nice >> system;
  }
  stream.close();
  return stoi(user) + stoi(nice) + stoi(system);
  }

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string idle, iowait, line;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    getline(stream,line);
    istringstream ss(line);
    SkipStreamElements(ss,4);
    ss >> idle >> iowait;
  }  
  return stoi(idle) + stoi(iowait);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  vector<int> pids = LinuxParser::Pids();
  return pids.size(); }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  ifstream stream(kProcDirectory + kStatFilename);
  string line;
  while (getline(stream, line))
  {
    if (line.find(nProcsRunning) != line.npos){
      int numProcs;
      istringstream ss(line);
      ss.ignore(32, ' ');
      ss >> numProcs;
      return numProcs;
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string kPid = to_string(pid);
  ifstream stream(kProcDirectory + kPid + kSlash + kCmdlineFilename);
  string line;
  if (stream.is_open()) {
    getline(stream, line);
    return line;
  }
  return "no info";
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid, float & ram) {
  string kPid = to_string(pid);
  ifstream stream(kProcDirectory + kPid + kSlash + kStatusFilename);
  if (stream.is_open()) {
    string line = "";
    while (getline(stream, line)) {
      if (line.find(nVmSize) != line.npos){
        istringstream ss(line);
        ss.ignore(32, '\t');
        string memSize;
        ss >> memSize;
        ram = stof(memSize);
        std::ostringstream memSizeStream;
        memSizeStream << std::fixed;
        memSizeStream << std::setprecision(2);
        memSizeStream << stof(memSize) * 0.001;
        stream.close();
        return memSizeStream.str(); 
      }
    }
    
  }
  ram = 0;
  return "0";
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string kPid = to_string(pid);
  ifstream stream(kProcDirectory + kPid + kSlash + kStatusFilename);
  if (stream.is_open()){
    string line = "";
    while (getline(stream, line))
    {
      if (line.find(nUid) != line.npos){
        istringstream ss(line);
        ss.ignore(32, '\t');
        string uid;
        ss >> uid;
        stream.close();
        return uid;
      }
    }
    
  }
  return string();
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  ifstream stream(kPasswordPath);
  if (stream.is_open()){
    string line;
    while (getline(stream,line))
    {
      if(line.find(nUidPrefix + uid) != line.npos){
        istringstream ss(line);
        string userName;
        getline(ss, userName, ':');
        return userName;
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string kPid = to_string(pid);
  ifstream stream(kProcDirectory + kPid + kSlash + kStatFilename);
  string line;
  long starttime;
  if (stream.is_open()){
    getline(stream, line);
    istringstream ss(line);
    ss.ignore(256,')');
    SkipStreamElements(ss, 20);
    ss >> starttime;
  }
  return LinuxParser::UpTime() - starttime/sysconf(_SC_CLK_TCK);
}

void StringFromStreamLine(std::ifstream & stream, std::string & result, int position){
  string line;
  getline(stream, line);
  istringstream ss(line);
  for (int i = 0; i < position; i++){
    ss.ignore(10,' ');
  }
  ss >> result;
  ss.clear();
}

void SkipStreamElements(istringstream & ss, int amount){
  for (int i = 0; i < amount; i++){
    ss.ignore(256, ' ');
  }
}
