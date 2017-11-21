/*******************************
			PROJECT 1
		 CHRIS IAQUINTO
	   ABDULMOHSIN ALMADI
********************************/

#include "myscheduler.h"

//Each time a thread is created, a new pointer thread descriptor block is created and all the attributes are filled in.
//The thread is then pushed into our vector that's used to store threads. 
void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	ThreadDescriptorBlock *thread = new ThreadDescriptorBlock; 
	thread->arriving_time = arriving_time;
	thread->priority = priority;
	thread->remaining_time = remaining_time;
	thread->tid = tid; 
	threadVec.push_back(thread); 
}

bool MyScheduler::Dispatch()
{
	switch (policy) {

	//First come, first served 
	case FCFS: {
		//Keeps track of whether or not the CPUs are still processing. This is important because in order to end the program all CPUs must be done processing
		//and there must be no more threads that need to be processed. 
		bool empty;

		
		/****************************************************************************************************
		 	Side note: I was told by a TA that the testbenches would be ran in order of arrival time. 
			This is why we don't check arrival time because we use the first available thread. This
			                       only applies to FCFS. - Chris Iaquinto
		*****************************************************************************************************/


		//This for loop checks each of the CPUs. If a CPU is empty it is filled with the next thread in line. The thread is then erased from the vector
		//because once it is in a CPU, it doesn't leave until its remaining time is zero
		for (int i = 0; i < num_cpu; i++) {
			if (CPUs[i] == NULL) {
				if (threadVec.size() != 0) {
					if (timer >= threadVec[0]->arriving_time) {
						CPUs[i] = threadVec[0];
						threadVec.erase(threadVec.begin());
					}
				}
			}
		}

		//Begin: 
		//The code between "Begin:" and "End" is used in each of our policies. This code checks that the CPUs are done processing and that the thread vector is empty
		//If both of those conditions are true (all CPUs finished and no threads left) then we can end the simulation for this policy by returning false. 
		for (int j = 0; j < num_cpu; j++) {
			if (CPUs[j] != NULL) {
				empty = false; // there is a cpu that is prcesing a thread
				break; // break from the for loop 
			}
			else empty = true; // else all CPUs are empty 
		}

		if (empty == true && threadVec.empty()) {
			return false;
		}
		//End

		break;
	}

	//Shortest time remaining first, without preemption
	case STRFwoP: {
		bool empty; 
		//integers time and position are used to compare/store the time/position of each thread so the shortest timed thread available can be found
		int time;
		int position = 0; 
		
		//Checks each CPU
		for (int i = 0; i < num_cpu; i++) {
			if (CPUs[i] == NULL) {
				//if there are no more threads, return until all CPUs are empty (Checked at the end)
				if (threadVec.size() != 0) {
					//If a thread is available at the time of checking
					if (timer >= threadVec[0]->arriving_time) {
						time = threadVec[0]->remaining_time;
						position = 0;
						//compares each available thread to find the one with the shortest remaining time. Position is set by this for loop
						for (int j = 1; j < threadVec.size(); j++) {
							if (timer >= threadVec[j]->arriving_time) {
								if (threadVec[j]->remaining_time < time) {
									time = threadVec[j]->remaining_time;
									position = j;
								}
							}
						}
						//Using position determined above, the next available CPU processes the thread with the shortest remaining time
						CPUs[i] = threadVec[position];
						//Because threads will process until completion without interruption, they can be removed from the vector ocne they're inserted into a CPU
						threadVec.erase(threadVec.begin() + position);
					}
					else {
						return true;
					}
				}
			}
		}

		//Same as FCFS
		for (int j = 0; j < num_cpu; j++) {
			if (CPUs[j] != NULL) {
				empty = false; // there is a cpu that is prcesing a thread
				break; // break from the for loop 
			}
			else empty = true; // else all CPUs are empty 
		}
		if (empty == true && threadVec.empty()) {
			return false;
		}
		break;
	}

	//Shortest Time Remaining First, with preemption
	case STRFwP: {
		//Needed to compare the times of each available process
		int timeLeft = -1;
		int position = -1; 
		bool empty; 

		for (int i = 0; i < num_cpu; i++) {
			//Different logic is needed if a CPU is currently processing another thread
			if (CPUs[i] == NULL) {
				timeLeft = -1; 
				for (int j = 0; j < threadVec.size(); j++) {
					if (threadVec[j]->arriving_time <= timer && threadVec[j]->remaining_time > 0) {
						//Nothing to compare to, so set time equal to the first available threads time. 
						if (timeLeft == -1) {
							timeLeft = threadVec[j]->remaining_time; 
							position = j; 
							CPUs[i] = threadVec[j]; 
							//Remove the thread from the vector so that it's time is not compared again. It is reinserted if it is preempted
							threadVec.erase(threadVec.begin() + j); 
						}
						//once the time comparision is available (second iteration), compare each available thread to find the one with the shortest time
						else if (threadVec[j]->remaining_time < timeLeft) {
							timeLeft = threadVec[j]->remaining_time; 
							position = j; 
							//Reinsert the thread that is being preempted
							threadVec.push_back(CPUs[i]); 
							CPUs[i] = threadVec[j];
							threadVec.erase(threadVec.begin() + j); 
						}
					}
				}
			}
			
			//If a CPU is not NULL (It is processing a thread) 
			else {
				for (int k = 0; k < threadVec.size(); k++) {
					//Check if there is an available thread whose time is shorter than the one currently being processed. 
					//After iterating through each CPU, the thread with the highest remaining time will be kicked out.
					if (threadVec[k]->arriving_time <= timer && threadVec[k]->remaining_time > 0) {
						if (CPUs[i]->remaining_time > threadVec[k]->remaining_time) {
							//Reinsert the thread into the vector. 
							threadVec.push_back(CPUs[i]);
							CPUs[i] = threadVec[k];
							threadVec.erase(threadVec.begin() + k);
						}
					}
				}
			}

			//Same as FCFS
			for (int j = 0; j < num_cpu; j++) {
				if (CPUs[j] != NULL) {
					empty = false; // there is a cpu that is prcesing a thread
					break; // break from the for loop 
				}
				else empty = true; // else all CPUs are empty 
			}
			if (empty == true && threadVec.empty()) {
				return false;
			}
		}
		break;
	} 
		

	//Priority based scheduling
	case PBS: {
		bool empty; 
		int priority = -1;
		int position;

		//Outer for loop checks each CPU
		for (int i = 0; i < num_cpu; i++) {
			//Different logic for a CPU currently processing a thread vs one that is empty
			if (CPUs[i] == NULL) {
				priority = -1; 
				//Checks each  thread in the vector (this does not include ones currently in a processor)
				for (int j = 0; j < threadVec.size(); j++) { 
					//if the thread has arrived and still needs to be processed: 
					if (threadVec[j]->arriving_time <= timer && threadVec[j]->remaining_time > 0) { 
						if (priority == -1) {
							//First iteration, set priority equal to first available threads priority 
							priority = threadVec[j]->priority; 
							position = j; 
							//Insert it and remove it so that it is not compared again. Will be reinserted if preempted.
							CPUs[i] = threadVec[j];
							threadVec.erase(threadVec.begin() + j);
						}
						else if (threadVec[j]->priority < priority) {
							//Second to final iteration compares each available thread to see if it's priority is higher. If it is, it preempts the one currently
							//being processed.
							priority = threadVec[j]->priority; 
							position = j; 
							threadVec.push_back(CPUs[i]);
							CPUs[i] = threadVec[j];
						}
					}
				}
			}

			//If a CPU is not NULL (It is processing a thread) 
			else {
				for (int k = 0; k < threadVec.size(); k++) {
					if (threadVec[k]->arriving_time <= timer && threadVec[k]->remaining_time > 0) {
						//Compare each available thread to the one being processed. If it's priority is higher, preempt the one being processed. 
						//This is done until the lowest priority thread has been kicked from the array of CPUs. 
						if (CPUs[i]->priority > threadVec[k]->priority) {
							threadVec.push_back(CPUs[i]); 
							CPUs[i] = threadVec[k];
							threadVec.erase(threadVec.begin() + k);
						}
					}
				}
			}
		}

		//Same as FCFS
		for (int j = 0; j < num_cpu; j++) {
			if (CPUs[j] != NULL) {
				empty = false; // there is a cpu that is prcesing a thread
				break; // break from the for loop 
			}
			else empty = true; // else all CPUs are empty 
		}

		if (empty == true && threadVec.empty()) {
			return false;
		}
		break;
	}

	default:
		cout << "Invalid policy!";
		throw 0;
	}

	return true;
}