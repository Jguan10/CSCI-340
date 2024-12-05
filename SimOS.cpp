/*
    Name: Jiaxiong Guan
    ID: 24157665
*/

#include "SimOS.h"

/*
    The parameters specify number of hard disks in the simulated computer, amount of memory, and page size.
    Disks, frame, and page enumerations all start from 0.
*/
SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize)
    :numberOfDisks_{numberOfDisks}, amountOfRAM_{amountOfRAM}, pageSize_{pageSize} 
{
    trackPID = 1;
    CPU = 0;
    for(int i = 0; i < numberOfDisks; i++) {
        Disk d = Disk();
        disks.push_back(d);
    }
    
    // Add empty process to placehold
    Process p = Process(0);
    processes.push_back(p);
    frameTracker = amountOfRAM / pageSize + 1;
}

 /*
    Creates a new process in the simulated system. 
    The new process takes place in the ready-queue or immediately starts using the CPU.
    Every process in the simulated system has a PID. 
    Your simulation assigns PIDs to new processes starting from 1 and increments it by one for each new process. 
    Do not reuse PIDs of the terminated processes.
*/
void SimOS::NewProcess() {
    Process p = Process(trackPID);
    trackPID++;
  
    processes.push_back(p);
  
    if(readyQueue.empty()) {
        CPU = p.getPID();
        
    }
    else {
        readyQueue.push_back(p.getPID());
    }
}

/*
    The currently running process forks a child. 
    The child is placed in the end of the ready-queue.
*/
void SimOS::SimFork() {
    if(CPU != 0) {

        // Create new process 
        Process p = Process(trackPID);
        p.setParent(CPU);
        processes[CPU].addChild(p.getPID());
        trackPID++;

        // Add to back of ready queue
        readyQueue.push_back(p.getPID());
        processes.push_back(p);
    }
}

/*
    The process that is currently using the CPU terminates. 
    Make sure you release the memory used by this process immediately. 
    If its parent is already waiting, the process terminates immediately 
        and the parent becomes runnable (goes to the ready-queue). 
    If its parent hasn't called wait yet, the process turns into zombie.
    To avoid the appearance of the orphans, the system implements the cascading termination. 
    Cascading termination means that if a process terminates, all its descendants terminate with it.
*/
void SimOS::SimExit() {

    // Remove memory access
    for(int i = 0; i < mu.size(); i++) {
        if(mu[i].PID == CPU) {
            mu.erase(mu.begin()+i);
            amountOfRAM_ += pageSize_;
        }
    }

    // Cascading termination 
    std::vector<int> children = processes[CPU].getChildren();
    for(int i = 0; i < children.size(); i++) {
        processes[children[i]].terminate();
    }

    // Become zombie if parent hasn't called wait
    int parentPID = processes[CPU].getParent();
    if(!processes[parentPID].isWaiting() && parentPID != 0) {
        processes[CPU].becomeZombie();
    }

    // Terminate, free from CPU and let next process use CPU
    processes[CPU].terminate();
    CPU = 0;
    removeTerminated();
    CPU = readyQueue.front();
    readyQueue.pop_front();
    
}

/*
    The process wants to pause and wait for any of its child processes to terminate. 
    Once the wait is over, the process goes to the end of the ready-queue or the CPU. 
    If the zombie-child already exists, the process proceeds right away (keeps using the CPU) 
        and the zombie-child disappears. 
    If more than one zombie-child exists, the system uses one of them (any!) to immediately resumes the parent, 
        while other zombies keep waiting for the next wait from the parent.
*/
void SimOS::SimWait() {

    // CPU is waiting
    processes[CPU].toggleWait();
    

}

/*
    Interrupt arrives from the timer signaling that the time slice of the currently running process is over.
*/
void SimOS::TimerInterrupt() {
    
    // Add back to ready queue
    readyQueue.push_back(CPU);
    CPU = 0;
    removeTerminated();

    // Let next process use CPU
    CPU = readyQueue.front();
    readyQueue.pop_front();
}

/*
    Currently running process requests to read the specified file from the disk with a given number. 
    The process issuing disk reading requests immediately stops using the CPU, even if the ready-queue is empty.
*/
void SimOS::DiskReadRequest(int diskNumber, std::string fileName) {

    // Check if disk number is valid
    if(diskNumber < numberOfDisks_ && diskNumber >= 0) {
        disks[diskNumber].addPID(CPU);
        disks[diskNumber].addFile(fileName);
    }
    else {
        throw std::out_of_range("Disk number is out of range");
    }

    // Stop using CPU
    CPU = 0;
    removeTerminated();
    
}

/*
    A disk with a specified number reports that a single job is completed. 
    The served process should return to the ready-queue.
*/
void SimOS::DiskJobCompleted(int diskNumber) {

    // Remove from disk and add back to ready queue
    int pid = disks[diskNumber].getPID();
    disks[diskNumber].removePID();
    disks[diskNumber].removeFile();
    readyQueue.push_back(pid);

    // CPU is on next process
    CPU = readyQueue.front();
    readyQueue.pop_front();
}

/*
    Currently running process wants to access the specified logical memory address. 
    System makes sure the corresponding page is loaded in the RAM. 
    If the corresponding page is already in the RAM, its “recently used” information is updated.
*/
void SimOS::AccessMemoryAddress(unsigned long long address) {
    
    // Temp var for page num
    int page = address / pageSize_;
    
    // Check if pageNumber is being used
    for(int i = 0; i < mu.size(); i++) {
        if(mu[i].pageNumber == page) {
            return;
        }
    }

    // Check if there is available RAM
    if(amountOfRAM_ < pageSize_ ) {
        return;
    }

    // Create memory item
    MemoryItem mi;
    mi.pageNumber = page;
    mi.PID = CPU;
    mi.frameNumber = frameTracker;
    frameTracker++;

    // Subtract available RAM and add to vector
    amountOfRAM_ -= pageSize_;
    mu.push_back(mi);
}

/*
    GetCPU returns the PID of the process currently using the CPU. 
    If CPU is idle it returns NO_PROCESS (see the supplied definitions above).
*/
int SimOS::GetCPU() {
    if(CPU == 0)
        return NO_PROCESS;
    return CPU;
}

/*
    GetReadyQueue returns the std::deque with PIDs of processes in the ready-queue where 
        element in front corresponds start of the ready-queue.
*/
std::deque<int> SimOS::GetReadyQueue() {
    return readyQueue;
}

/*
    GetMemory returns MemoryUsage vector describing all currently used frames of RAM. 
    Remember, Terminated “zombie” processes don’t use memory, so they don’t contribute to memory usage.
    MemoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high).
*/
MemoryUsage SimOS::GetMemory() {
    
    return mu;
}

/*
    GetDisk returns an object with PID of the process served by specified disk 
        and the name of the file read for that process. 
    If the disk is idle, GetDisk returns the default FileReadRequest object (with PID 0 and empty string in fileName) 
*/
FileReadRequest SimOS::GetDisk(int diskNumber) {
    FileReadRequest req;
    if(diskNumber >= 0 && diskNumber < numberOfDisks_) {
        if(disks[diskNumber].isEmpty()) {
            req.PID = NO_PROCESS;
            req.fileName = "";
            return req;
        }

        else {
            req.PID = disks[diskNumber].getPID();
            req.fileName = disks[diskNumber].getFile();
            return req;
        }
    }

    return req;
}

/*
    GetDiskQueue returns the I/O-queue of the specified disk starting from the “next to be served” process.
*/
std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber) {
    std::deque<FileReadRequest> reqs;
    std::deque<int> pids = disks[diskNumber].getPIDQueue();
    std::deque<std::string> fileNames = disks[diskNumber].getFileQueue();
    for(int i = 0; i < disks[diskNumber].getSize(); i++) {
        FileReadRequest req;
        req.PID = pids.front();
        pids.pop_front();
        req.fileName = fileNames.front();
        fileNames.pop_front();
        reqs.push_back(req);

        // Only account for "next to be served"
        reqs.pop_front();
    }
    return reqs;
}

/*
    Removes the top PID in the ready queue if the process is terminated
*/
void SimOS::removeTerminated() {
    if(!readyQueue.empty()) {
        int pid = readyQueue.front();
        while(processes[pid].isTerminated()) {
            readyQueue.pop_front();
            pid = readyQueue.front();
        }
    }
}

