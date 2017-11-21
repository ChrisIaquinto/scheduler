// Project1.cpp : Defines the entry point for the console application.
//

#include "myscheduler.h"
#include <stdio.h>
int main(int argc, char* argv[])
{
	int numCPUS = 5;

	cout << "PBS Policy, 3 CPUs\n";
	MyScheduler ms4 = MyScheduler(Policy::PBS, 3);
	ms4.CreateThread(0, 12, 5, 0);
	ms4.CreateThread(5, 2, 2, 1);
	ms4.CreateThread(3, 7, 4, 2);
	ms4.CreateThread(9, 1, 10, 3);
	ms4.CreateThread(1, 4, 3, 4);
	ms4.CreateThread(5, 2, 3, 5);
	ms4.CreateThread(1, 4, 3, 6);

	ms4.Go();
	
	cout << "STRFwP Policy, 3 CPUs\n";
	MyScheduler ms3 = MyScheduler(Policy::STRFwP, 3);
	ms3.CreateThread(0, 12, 0, 0);
	ms3.CreateThread(5, 2, 2, 1);
	ms3.CreateThread(3, 7, 1, 2);
	ms3.CreateThread(9, 1, 10, 3);
	ms3.CreateThread(1, 4, 3, 4);
	ms3.CreateThread(5, 2, 3, 5);
	ms3.CreateThread(1, 4, 3, 6);

	ms3.Go();

	cout << "STRFwoP Policy, 3 CPUs\n";
	MyScheduler ms2 = MyScheduler(Policy::STRFwoP, 3);
	ms2.CreateThread(0, 12, 0, 0);
	ms2.CreateThread(5, 2, 2, 1);
	ms2.CreateThread(3, 7, 1, 2);
	ms2.CreateThread(9, 1, 10, 3);
	ms2.CreateThread(1, 4, 3, 4);
	ms2.CreateThread(5, 2, 3, 5);
	ms2.CreateThread(1, 4, 3, 6);

	ms2.Go();

	cout << "FCFS Policy, 2 CPUs\n";
	MyScheduler ms1 = MyScheduler(Policy::FCFS, 2);
	ms1.CreateThread(0, 12, 0, 0);
	ms1.CreateThread(1, 4, 3, 6);
	ms1.CreateThread(1, 4, 3, 4);
	ms1.CreateThread(3, 7, 1, 2);
	ms1.CreateThread(5, 2, 2, 1);
	ms1.CreateThread(5, 2, 3, 5);
	ms1.CreateThread(9, 1, 10, 3);
	
	
	

	ms1.Go();
	

	return 0;
}

