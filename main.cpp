#include "SimOS.h"
#include <deque>

int main()
{
	SimOS sim{ 3, 1000, 10 };          // 3 disks, 1000 RAM, 10 page size
	bool allTestsClean{ true };

	// CPU starts empty
	if (sim.GetCPU() != NO_PROCESS)
	{
		allTestsClean = false;
		std::cout << "Test on the line 9 fails!" << std::endl;
	}
	else {
		std::cout << "Passed First" << std::endl;
	}


	// New process with PID 1 is created
	sim.NewProcess();
	if (sim.GetCPU() != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 16 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Second" << std::endl;
	}
	
	// Process reading disk no longer using CPU
	sim.DiskReadRequest(0, "file1.txt");
	if (sim.GetCPU() != NO_PROCESS)
	{
		allTestsClean = false;
		std::cout << "Test on the line 23 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Third" << std::endl;
	}
	

	// Check if GetDisk returns correct PID and fileName
	FileReadRequest request{ sim.GetDisk(0) };
	if (request.PID != 1 || request.fileName != "file1.txt")
	{
		allTestsClean = false;
		std::cout << "Test on the line 30 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Fourth" << std::endl;
	}

	// Check if returns ioQueue with process 
	std::deque<FileReadRequest> ioQueue0{ sim.GetDiskQueue(0) };
	if (ioQueue0.size() != 0)
	{
		allTestsClean = false;
		std::cout << "Test on the line 37 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Fifth" << std::endl;
	}

	// Check if disk job completes and if disk is empty and if getDisk is returning correct default
	sim.DiskJobCompleted(0);
	request = sim.GetDisk(0);
	if (request.PID != NO_PROCESS || request.fileName != "")
	{
		allTestsClean = false;
		std::cout << "Test on the line 45 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Sixth" << std::endl;
	}

	// Check if process is back in readyQueue
	if (sim.GetCPU() != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 51 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Seventh" << std::endl;
	}

	// Check if readyQueue is properly returning and if it is empty (process 1 should be using CPU)
	std::deque<int> readyQueue{ sim.GetReadyQueue() };
	if (readyQueue.size() != 0)
	{
		allTestsClean = false;
		std::cout << "Test on the line 58 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Eighth" << std::endl;
	}

	// Check if fork creates process and process is in queue
	sim.SimFork();
	readyQueue = sim.GetReadyQueue();
	if (readyQueue[0] != 2)
	{
		allTestsClean = false;
		std::cout << "Test on the line 66 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Ninth" << std::endl;
	}

	// Check if interrupt places running process in ready queue and lets next process use cpu
	sim.TimerInterrupt();
	readyQueue = sim.GetReadyQueue();
	if (sim.GetCPU() != 2 || readyQueue[0] != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 74 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Tenth" << std::endl;
	}

	// Check if process exits and next process uses cpu
	sim.SimExit();
	readyQueue = sim.GetReadyQueue();
	if (sim.GetCPU() != 1 || readyQueue.size() != 0)
	{
		allTestsClean = false;
		std::cout << "Test on the line 82 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Eleventh" << std::endl;
	}

	// Check if memory usage vector is returned and if memory address req is done correctly
	sim.AccessMemoryAddress(140);
	MemoryUsage ram{ sim.GetMemory() };
	if (ram[0].pageNumber != 14 || ram[0].PID != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 90 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Twevlth" << std::endl;
	}

	sim.SimWait();
	if (sim.GetCPU() != 1)
	{
		allTestsClean = false;
		std::cout << "Test on the line 97 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Thirteenth" << std::endl;
	}

	sim.SimExit();
	ram = sim.GetMemory();
	if (sim.GetCPU() != NO_PROCESS || ram.size() != 0)
	{
		allTestsClean = false;
		std::cout << "Test on the line 105 fails!" << std::endl;
	}
	else {
		std::cout << "Passed Fourteenth" << std::endl;
	}

	if (allTestsClean)
		std::cout << "These preliminary tests are all passed" << std::endl;

	return 0;
}