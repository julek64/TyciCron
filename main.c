#include "tasks.h"
#include "merge_sort.h"
#include "signal_handling.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv)
{
    sig_catch();
    int pid = getpid();
    printf("\npid: %d\n", pid);
    TaskNode* tasks = get_tasks("input.txt");
    TaskNode* current;
    int remainingTime;

    merge_sort(&tasks);
    current = tasks;

    while(current != NULL)
    {
        printf("%d:%d - %s - %d\n",
        current->task->time->hour,
        current->task->time->minute,
        current->task->command,
        current->task->info);
        printf("remaining sec: %d \n", get_remaining_time(current->task->time));
        current = current->next;
    }
    current = tasks;

    while(current != NULL)
    {
        remainingTime = get_remaining_time(current->task->time);
        if(remainingTime < -59)
        {
            current = current->next;
            continue;
        }
        printf("Sleeping for %d seconds\n", remainingTime);
        fflush(stdout);
        if(remainingTime > 0 )
            sleep(remainingTime);
            
        run_task(*current->task);
        current = current->next;
    }

    return 0;
}