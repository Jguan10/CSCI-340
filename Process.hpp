/*
    Name: Jiaxiong Guan
    ID: 24157665
*/

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <iostream>
#include <vector>

class Process {
public:
    Process(int pid);

    /*
        @return : Returns the process's pid
    */
    int getPID();

    /*
        @return : Returns true if zombie, false if not zombie
    */
    bool isZombie();

    /*
        @post : Mutate private zombie variable
    */
    void becomeZombie();

    /*
        @return : Returns the parent pid
    */
    int getParent();

    /*
        @post : Mutate private parentpid variable
    */
    void setParent(int pid);

    /*
        @return : Returns the children pids
    */
    std::vector<int> getChildren();

    /*
        @post : Adds a child
    */
    void addChild(int pid);

    /*
        @post : Terminate the process
    */
    void terminate();

    /*
        @return : Returns true if process is terminated, else return false
    */
    bool isTerminated();

    /*
        @post : Toggle if the process is in wait mode
    */
    void toggleWait();

    /*
        @return : Returns true if process is waiting
    */
    bool isWaiting();

    /*
        @post : Set child variable to true indicating process is a child
    */
    void forkedChild();

    /*
        @return : Return true if process is a child, false otherwise
    */
    bool isChild();

private:
    int pid_;                          
    bool zombie_;                       // Zombie status
    bool terminated_;                   // Termination status
    bool wait_;                         // Status of wait
    int parentpid;                      // PID of parent 
    std::vector<int> children;          // Vector of PID of children
    bool child;                         // True if process is a child

};

#endif