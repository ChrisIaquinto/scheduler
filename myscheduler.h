/*******************************
			PROJECT 1
		 CHRIS IAQUINTO
	   ABDULMOHSIN ALMADI
********************************/

#pragma once

#include "scheduler.h"
#include <vector> 

using namespace std;

//A vector is used to store threads. Vectors are easy to use and flexible. 
static std::vector<ThreadDescriptorBlock*> threadVec;


class MyScheduler : public Scheduler {
public:
	MyScheduler(Policy p, unsigned int n) : Scheduler(p, n) {}
	bool Dispatch() override; //Function to implement scheduling policy and to keep a check on processed threads
	void CreateThread(int arriving_time, int remaining_time, int priority, int tid) override; //Function to create threads and insert them in student defined data structure																			 																					 
};