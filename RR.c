#include "process.h"

int SetCpuAffinity(pid_t pid, int core_number);
int wait_process(pid_t pid);
int wake_process(pid_t pid);
int run_process(Process *pcb);

int start_RR_process(Process **processList, int processNum, int sched_timer, int readyNum, RR_list **head, RR_list **tail) {
	for (; readyNum < processNum; readyNum++) {
		if (processList[readyNum]->start_time > sched_timer)
			break;
		else if (processList[readyNum]->pid == -1) {
			if ((*tail) == NULL) {
				(*head) = (RR_list*)malloc(sizeof(RR_list));
				(*head)->process = readyNum;
				(*head)->next = NULL;
				(*tail) = (*head);
			}
			else {
				(*tail)->next = (RR_list*)malloc(sizeof(RR_list));
				(*tail) = (*tail)->next;
				(*tail)->process = readyNum;
				(*tail)->next = NULL;
			}

			processList[readyNum]->pid = run_process(processList[readyNum]);
			wait_process(processList[readyNum]->pid);
		}
	}
	return readyNum;
}

int Next_RR(Process **processList, int processNum, int ptrNow, int RR_timer, RR_list **head, RR_list **tail) {
	if (ptrNow == -1) {
		if ((*head) == NULL)
			return -1;
		else
			return (*head)->process;
	}
	else if (RR_timer < 500)
		return ptrNow;
	else {
		if ((*head) == (*tail) && (*head) == NULL)
			return -1;
		else if ((*head) == (*tail))
			return ptrNow;
		else {
			(*tail)->next = (*head);
			(*tail) = (*head);
			(*head) = (*head)->next;
			(*tail)->next = NULL;
			return (*head)->process;
		}
	}
}

////////////////////

void RR_scheduling(Process** processList, int processNum, int sched_type){
	
	int doneNum = 0;
	int ptrNext = -1;
	int ptrNow = -1;
	int sched_timer = 0;
	int RR_timer = 0;
	int readyNum = 0;

	RR_list *head = NULL;
	RR_list *tail = NULL;

	SetCpuAffinity(getpid(), 0);
	wake_process(getpid());

	while(doneNum < processNum){
		readyNum = start_RR_process(processList, processNum, sched_timer, readyNum, &head, &tail);

		if(ptrNow != -1 && processList[ptrNow]->runtime <= 0){
			waitpid(processList[ptrNow]->pid, NULL, 0);
			doneNum++;
			ptrNow = -1;

			if (tail == head) {
				free(head);
				tail = NULL;
				head = NULL;
			}
			else {
				RR_list *trash = head;
				head = head->next;
				free(trash);
			}
			
			if(doneNum == processNum)
				return;
		}

		ptrNext = Next_RR(processList, readyNum, ptrNow, RR_timer, &head, &tail);
		
		if (RR_timer == 500 || ptrNext != ptrNow)
			RR_timer = 0;

		if (ptrNext != ptrNow && ptrNext != -1 && ptrNow != -1) {
			wake_process(processList[ptrNext]->pid);
			wait_process(processList[ptrNow]->pid);
			ptrNow = ptrNext;	
		}
		else if(ptrNext != -1 && ptrNow == -1) {
			wake_process(processList[ptrNext]->pid);
			ptrNow = ptrNext;
		}


		run_unit_time();
		if(ptrNow != -1) {
			processList[ptrNow]->runtime--;
		}

		sched_timer++;
		RR_timer++;

/*		if (sched_timer%100 == 0) {
			printf("RR_timer: %d\n", RR_timer);
			for (int i = 0; i < processNum; i++)
				printf("%d ", processList[i]->runtime);
			puts("");
			RR_list *tmp = head;
			while (tmp != NULL) {
				printf("%d->", tmp->process);
				tmp = tmp->next;
			}
			puts("");
		}*/
	}
}
