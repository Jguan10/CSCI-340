/*
    Name: Jiaxiong Guan
    ID: 24157665
*/

#ifndef SimOS_H
#define SimOS_H

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include "Process.hpp"
#include "Disk.hpp"
#include <stdexcept>

//If a disk with the requested number doesn’t exist throw std::out_of_range exception.
//If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.

struct FileReadRequest
{
    int PID{0};
    std::string fileName {""};
};
 
struct MemoryItem
{
    unsigned long long pageNumber;
    unsigned long long frameNumber;
    int PID; // PID of the process using this frame of memory
};
 
using MemoryUsage = std::vector<MemoryItem>;
 
constexpr int NO_PROCESS{ 0 };

class SimOS {

public:

    /*
        The parameters specify number of hard disks in the simulated computer, amount of memory, and page size.
        Disks, frame, and page enumerations all start from 0.
    */
    SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize);

    /*
        Creates a new process in the simulated system. 
        The new process takes place in the ready-queue or immediately starts using the CPU.
        Every process in the simulated system has a PID. 
        Your simulation assigns PIDs to new processes starting from 1 and increments it by one for each new process. 
        Do not reuse PIDs of the terminated processes.
    */
    void NewProcess();

    /*
        The currently running process forks a child. 
        The child is placed in the end of the ready-queue.
    */
    void SimFork();

    /*
        The process that is currently using the CPU terminates. 
        Make sure you release the memory used by this process immediately. 
        If its parent is already waiting, the process terminates immediately 
            and the parent becomes runnable (goes to the ready-queue). 
        If its parent hasn't called wait yet, the process turns into zombie.
        To avoid the appearance of the orphans, the system implements the cascading termination. 
        Cascading termination means that if a process terminates, all its descendants terminate with it.
    */
    void SimExit();

    /*
        The process wants to pause and wait for any of its child processes to terminate. 
        Once the wait is over, the process goes to the end of the ready-queue or the CPU. 
        If the zombie-child already exists, the process proceeds right away (keeps using the CPU) 
            and the zombie-child disappears. 
        If more than one zombie-child exists, the system uses one of them (any!) to immediately resumes the parent, 
            while other zombies keep waiting for the next wait from the parent.
    */
    void SimWait();

    /*
        Interrupt arrives from the timer signaling that the time slice of the currently running process is over.
    */
    void TimerInterrupt();

    /*
        Currently running process requests to read the specified file from the disk with a given number. 
        The process issuing disk reading requests immediately stops using the CPU, even if the ready-queue is empty.
    */
    void DiskReadRequest(int diskNumber, std::string fileName);

    /*
        A disk with a specified number reports that a single job is completed. 
        The served process should return to the ready-queue.
    */
    void DiskJobCompleted(int diskNumber);

    /*
        Currently running process wants to access the specified logical memory address. 
        System makes sure the corresponding page is loaded in the RAM. 
        If the corresponding page is already in the RAM, its “recently used” information is updated.
    */
    void AccessMemoryAddress(unsigned long long address);

    /*
        GetCPU returns the PID of the process currently using the CPU. 
        If CPU is idle it returns NO_PROCESS (see the supplied definitions above).
    */
    int GetCPU();

    /*
        GetReadyQueue returns the std::deque with PIDs of processes in the ready-queue where 
            element in front corresponds start of the ready-queue.
    */
    std::deque<int> GetReadyQueue();

    /*
        GetMemory returns MemoryUsage vector describing all currently used frames of RAM. 
        Remember, Terminated “zombie” processes don’t use memory, so they don’t contribute to memory usage.
        MemoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high).
    */
    MemoryUsage GetMemory();

    /*
        GetDisk returns an object with PID of the process served by specified disk 
            and the name of the file read for that process. 
        If the disk is idle, GetDisk returns the default FileReadRequest object (with PID 0 and empty string in fileName) 
    */
    FileReadRequest GetDisk(int diskNumber);

    /*
        GetDiskQueue returns the I/O-queue of the specified disk starting from the “next to be served” process.
    */
    std::deque<FileReadRequest> GetDiskQueue(int diskNumber); 

    /*
        Removes the top PID in the ready queue if the process is terminated
    */
    void removeTerminated();

private:

    int CPU;                                          // PID of process using CPU
    int numberOfDisks_;
    unsigned long long amountOfRAM_;                  // Available RAM
    unsigned int pageSize_;                           
    int frameTracker;                                 // Keep track of frames
    int trackPID;                                     // Counter for PID, initialized to 1 when constructor is called
    std::deque<int> readyQueue;                       // Ready queue containing PID of processes
    std::vector<Process> processes;                   // Vector containing all processes
    std::vector<Disk> disks;                          // Vector containing all disks
    MemoryUsage mu;                                   // Memory usage vector                         
};

#endif