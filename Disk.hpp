/*
    Name: Jiaxiong Guan
    ID: 24157665
*/

#ifndef DISK_HPP
#define DISK_HPP

#include <iostream>
#include <deque>
#include <string>

class Disk {

public:

    Disk();

    /*
        @post : Add process PID to disk I/O queue
    */
    void addPID(int pid);

    /*
        @return : Returns the PIDs in disk I/O queue
    */
    std::deque<int> getPIDQueue();

    /*
        @post : Remove the top PID from disk I/O queue
    */
    void removePID();

    /*
        @return : Returns the top PID in I/O queue
    */
    int getPID();

    /*
        @post : Add file name request
    */
    void addFile(std::string fileName);

    /*
        @return : Returns file name request queue
    */
    std::deque<std::string> getFileQueue();

    /*
        @post : Remove the top file name request
    */
    void removeFile();

    /*
        @return : Returns the top file name in the queue
    */
    std::string getFile();

    /*
        @return: Returns true if disk is empty
    */
    bool isEmpty();

    /*
        @return: Returns the size of the queue
    */
    int getSize();

private:
    std::deque<int> pidQueue;                  // Deque keeping track of PIDs in disk I/O queue
    std::deque<std::string> fileQueue;         // Deque keeping track of file names 
    
};

#endif
