#include "tasks.h"
#include "merge_sort.h"
#include "signal_handling.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int INTStatus = 0;
int USR1Status = 0;
int USR2Status = 0;

int main(int argc, char* argv)
{
    sig_init();
    int pid = getpid();
    int status;
    int childPid;
    printf("\npid: %d\n", pid);
    TaskNode* tasks = get_tasks("input.txt");
    TaskNode* current;
    int remainingTime;

    merge_sort(&tasks);

    print_tasks(tasks);
    current = tasks;

    while(current != NULL)
    {
        go_to_current(tasks, &current, &remainingTime);

        while(remainingTime > 0)
        {
            printf("Sleeping for %d seconds\n", remainingTime);
            fflush(stdout);
            sleep(remainingTime);

            //signal handling
            if (INTStatus)
            {                
                do
                {
                    childPid = wait(NULL);
                }while(childPid > 0);

                sig_int(current->task->time, *current->task, tasks);
            }
            else if (USR1Status)
                sig_usr1(&tasks, "input.txt", &current, &remainingTime);
            else if (USR2Status)
                sig_usr2(tasks);

            break;
            remainingTime = get_remaining_time(current->task->time);
        }
        
        status = run_task(*current->task);
    }

    return 0;
}