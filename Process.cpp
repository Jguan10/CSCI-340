/*
    Name: Jiaxiong Guan
    ID: 24157665
*/

#include "Process.hpp"

Process::Process(int pid) {
    pid_ = pid;
    zombie_ = false;
    terminated_ = false;
    wait_ = false;
    parentpid = 0;
    child = false;
}

/*
    @return : Returns the process's pid
*/
int Process::getPID() {
    return pid_;
}

/*
    @return : Returns true if zombie, false if not zombie
*/
bool Process::isZombie() {
    return zombie_;
}

/*
    @post : Mutate private zombie variable
*/
void Process::becomeZombie() {
    zombie_ = true;
}

/*
    @return : Returns the parent pid
*/
int Process::getParent() {
    return parentpid;
}

/*
    @post : Mutate private parentpid variable
*/
void Process::setParent(int pid) {
    parentpid = pid;
}

/*
    @return : Returns the children pids
*/
std::vector<int> Process::getChildren() {
    return children;
}

/*
    @post : Adds a child
*/
void Process::addChild(int pid) {
    children.push_back(pid);
}

/*
    @post : Terminate the process
*/
void Process::terminate() {
    terminated_ = true;
}

/*
    @return : Returns true if process is terminated, else return false
*/
bool Process::isTerminated() {
    return terminated_;
}

/*
    @post : Toggle if the process is in wait mode
*/
void Process::toggleWait() {
    wait_ = !wait_;
}

/*
    @return : Returns true if process is waiting
*/
bool Process::isWaiting() {
    return wait_;
}

/*
    @post : Set child variable to true indicating process is a child
*/
void Process::forkedChild() {
    child = true;
}

/*
    @return : Return true if process is a child, false otherwise
*/
bool Process::isChild() {
    return child;
}