/*
    Name: Jiaxiong Guan
    ID: 24157665
*/

#include "Disk.hpp"

Disk::Disk() {

}

/*
    @post : Add process PID to disk I/O queue
*/
void Disk::addPID(int pid) {
    pidQueue.push_back(pid);
}

/*
    @return : Returns the PIDs in disk I/O queue
*/
std::deque<int> Disk::getPIDQueue() {
    return pidQueue;
}

/*
    @post : Remove the top PID from disk I/O queue
*/
void Disk::removePID() {
    pidQueue.pop_front();
  
}

/*
    @return : Returns the top PID in I/O queue
*/
int Disk::getPID() {
    return pidQueue.front();
}

/*
    @post : Add file name request
*/
void Disk::addFile(std::string fileName) {
    fileQueue.push_back(fileName);
}

/*
    @return : Returns file name request queue
*/
std::deque<std::string> Disk::getFileQueue() {
    return fileQueue;
}

/*
    @post : Remove the top file name request
*/
void Disk::removeFile() {
    
    fileQueue.pop_front();
    
}

/*
    @return : Returns the top file name in the queue
*/
std::string Disk::getFile() {
    return fileQueue.front();
}

/*
    @return: Returns true if disk is empty, false if something is in disk
*/
bool Disk::isEmpty() {
    if(pidQueue.empty() && fileQueue.empty()) {
        return true;
    }
    else   
        return false;
}

/*
    @return: Returns the size of the queue
*/
int Disk::getSize() {
    return pidQueue.size();
}