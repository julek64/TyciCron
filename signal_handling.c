#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "tasks.h"
#include "merge_sort.h"

void sig_handler(int signo);

extern int INTStatus;
extern int USR1Status;
extern int USR2Status;

void sig_init()
{
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        printf("Error handling SIGUSR1\n");
    if (signal(SIGUSR2, sig_handler) == SIG_ERR)
        printf("Error handling SIGUSR2");
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("Error handling SIGINT\n");
}

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
    {
        printf("\nCaught SIGUSR1\n");
        USR1Status = 1;
    }
    else if (signo == SIGUSR2)
    {
        printf("\nCaught SIGUSR2\n");
        USR2Status = 1;
    }
    else if (signo == SIGINT)
    {
        printf("\nCaught SIGINT\n");
        INTStatus = 1;
    }
}


void sig_int(struct TaskTime* time, struct Task task, TaskNode* tasks)
{
    printf("\nExecuting SIGINT\n");
    // implementation
    if (get_remaining_time(time) <= 0)
        run_task(task);
    INTStatus = 0;
    free_tasks(tasks);
    exit(EXIT_SUCCESS);
}

void sig_usr1(TaskNode** tasks_pointer, char* path, TaskNode** current_pointer, int* remainingTime)
{
    printf("\nExecuting SIGUSR1\n");
    // implementation
    if (*tasks_pointer != NULL)
        free_tasks(*tasks_pointer);
    *tasks_pointer = get_tasks(path);
    merge_sort(tasks_pointer);
    print_tasks(*tasks_pointer);
    *current_pointer = *tasks_pointer;
    go_to_current(*tasks_pointer, current_pointer, remainingTime);
    USR1Status = 0;
}

void sig_usr2(TaskNode* tasks)
{
    printf("\nExecuting SIGUSR2\n");
    // implementation
    print_tasks(tasks);
    USR2Status = 0;
}