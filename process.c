#define _GNU_SOURCE
#include "process.h"

int SetCpuAffinity(pid_t pid, int core_number) {
	int cpu_num = sizeof(cpu_set_t);
	if(core_number > cpu_num){
		printf("SetCpuAffinity error!\n");
		return -1;
	}
		
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core_number, &mask);

	sched_setaffinity(pid, cpu_num, &mask);
	return 1;
}

int run_process(Process *pcb){
	pid_t pid = fork();
	if(pid < 0){
		fprintf(stderr, "fork error!\n");
		exit(0);
	}
	else if (pid > 0) {
		SetCpuAffinity(pid, 1);
		return pid;
	}
	else {
		struct timeval start_time, end_time;

		gettimeofday(&start_time, NULL);

		for(int i = 0; i < pcb->runtime; i++) {
			run_unit_time();
		}

		gettimeofday(&end_time, NULL);

		char buffer[1024] = {};
		sprintf(buffer, "echo \"[Project1] %u %lu.%lu %lu.%lu\" > /dev/kmsg", getpid(), start_time.tv_sec, start_time.tv_usec, end_time.tv_sec, end_time.tv_usec);
		system(buffer);

		exit(0);
	}
}


int wait_process(pid_t pid) {
	struct sched_param param;
	param.sched_priority = 0;
	return sched_setscheduler(pid, SCHED_IDLE, &param);
}

int wake_process(pid_t pid) {
	struct sched_param param;
	param.sched_priority = 0;
	return sched_setscheduler(pid, SCHED_OTHER, &param);
}
