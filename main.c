#include "process.h"

int process_cmp(const void *a, const void *b){
	Process *c = (Process*)a;
	Process *d = (Process*)b;
	return c->start_time - d->start_time;
}

void process_scheduling(Process **processList, int processNum, int schedulingType);

int main() {

	char buffer[64] = {};
	int sched_type = 1;	
	scanf("%s", buffer);
	if(strcmp(buffer, "FIFO") == 0)
		sched_type = 1;
	else if(strcmp(buffer, "SJF") == 0)
		sched_type = 2;
	else if(strcmp(buffer, "PSJF") == 0)
		sched_type = 3;
	else if(strcmp(buffer, "RR") == 0)
		sched_type = 4;
	else {
		fprintf(stderr, "sched_type error!\n");
		return 0;
	}
	
	
	int processNum = 0;
	scanf("%d", &processNum);
	Process **processList = (Process**)malloc(sizeof(Process*)*processNum);
	

	for(int i = 0; i < processNum; i++) {
		processList[i] = (Process*)malloc(sizeof(Process));
		scanf("%s%d%d", processList[i]->name, &(processList[i]->start_time), &(processList[i]->runtime));
		processList[i]->pid = -1;
	}
	
	//qsort(processList, processNum, sizeof(Process*), process_cmp);

	
	for (int i = 0; i < processNum; i++)
		for (int j = i + 1; j < processNum; j++)
			if (processList[i]->start_time > processList[j]->start_time) {
				Process *tmp = processList[i];
				processList[i] = processList[j];
				processList[j] = tmp;
			}
			else if (processList[i]->start_time == processList[j]->start_time && processList[i]->runtime > processList[j]->runtime) {
				Process *tmp = processList[i];
				processList[i] = processList[j];
				processList[j] = tmp;
			}
	
	process_scheduling(processList, processNum, sched_type);
	
	for (int i = 0; i < processNum; i++)
		printf("%s %d\n", processList[i]->name, processList[i]->pid);

	return 0;
}
