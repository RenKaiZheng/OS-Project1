#include "process.h"

int SetCpuAffinity(pid_t pid, int core_number);
int run_process(Process *pcb);
int wait_process(pid_t pid);
int wake_process(pid_t pid);

int start_process(Process **processList, int processNum, int sched_timer, int readyNum){
	for (; readyNum < processNum; readyNum++) {
		if (processList[readyNum]->start_time > sched_timer)
			break;
		else if (processList[readyNum]->pid == -1) {
			processList[readyNum]->pid = run_process(processList[readyNum]);
		//	printf("%s %u\n", processList[readyNum]->name, processList[readyNum]->pid);
			wait_process(processList[readyNum]->pid);
		}
	}
	return readyNum;
}

////////////////////

int Next_FIFO(Process **processList, int processNum, int ptrNow) {
	if (ptrNow != -1)
		return ptrNow;
	else 
		for (int i = 0; i < processNum; i++)
			if (processList[i]->pid != -1 && processList[i]->runtime > 0)
				return i;
	return -1;
}

int Next_PSJF(Process **processList, int processNum, int ptrNow) {
	int out = -1;
	for (int i = 0; i < processNum; i++) {
		if (processList[i]->runtime <= 0)
			continue;
		else if (out == -1 || processList[i]->runtime < processList[out]->runtime)
			out = i;
	}
	return out;
}

int Next_SJF(Process **processList, int processNum, int ptrNow) {
	if (ptrNow != -1)
		return ptrNow;
	else
		return Next_PSJF(processList, processNum, ptrNow);
}

int Next_RR(Process **processList, int processNum, int ptrNow, int RR_timer) {
	if (ptrNow == -1) {
		for (int i = 0; i < processNum; i++)
			if (processList[i]->runtime > 0)
				return i;
		return -1;
	}
	else if (RR_timer < 500)
		return ptrNow;
	else {
		for (int i = (ptrNow+1)%processNum; i != ptrNow; i = (i+1)%processNum)
			if (processList[i]->pid != -1 && processList[i]->runtime > 0)
				return i;
		return (processList[ptrNow]->runtime > 0)?ptrNow:-1;
	}
}

////////////////////

void process_scheduling(Process** processList, int processNum, int sched_type){
	
	int doneNum = 0;
	int ptrNext = -1;
	int ptrNow = -1;
	int sched_timer = 0;
	int RR_timer = 0;
	int readyNum = 0;

	SetCpuAffinity(getpid(), 0);
	wake_process(getpid());

	while(doneNum < processNum){
		if(ptrNow != -1 && processList[ptrNow]->runtime <= 0){
			waitpid(processList[ptrNow]->pid, NULL, 0);
			ptrNow = -1;
			doneNum++;

			if(doneNum >= processNum)
				return;
		}
		
		readyNum = start_process(processList, processNum, sched_timer, readyNum);

		if (sched_type == 1)
			ptrNext = Next_FIFO(processList, readyNum, ptrNow);
		else if (sched_type == 2)
			ptrNext = Next_SJF(processList, readyNum, ptrNow);
		else if (sched_type == 3)
			ptrNext = Next_PSJF(processList, readyNum, ptrNow);
		else if (sched_type == 4)
			ptrNext = Next_RR(processList, readyNum, ptrNow, RR_timer);

		if (ptrNext == ptrNow && ptrNow != -1)
			RR_timer += 1;
		else
			RR_timer = 0;

		if (ptrNext != -1 && ptrNow != ptrNext && ptrNow != -1) {
			wake_process(processList[ptrNext]->pid);
			wait_process(processList[ptrNow]->pid);
			ptrNow = ptrNext;	
		}
		else if(ptrNow == -1 && ptrNext != -1) {
			wake_process(processList[ptrNext]->pid);
			ptrNow = ptrNext;
		}


		run_unit_time();
		if(ptrNow != -1){
			processList[ptrNow]->runtime--;
		}

		sched_timer++;
		
		//if (sched_timer % 1000 == 0)
		//	printf("%d\n", sched_timer);
	}
}
