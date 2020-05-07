#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <string.h>

#define run_unit_time()		        	\
{										\
	volatile unsigned long i;			\
	for (i = 0; i < 1000000UL; i++)		\
		;								\
}										\

typedef struct Process {
    char name[64];
    int start_time;
    int runtime;
    pid_t pid;
}Process;

struct RR_list {
    int process;
    struct RR_list *next;
};
typedef struct RR_list RR_list;