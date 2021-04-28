#include "tasks.h"
#include "merge_sort.h"
#include "signal_handling.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int INTStatus = 0;
int USR1Status = 0;
int USR2Status = 0;

char* inputFile;
char* outputFile;

int main(int argc, char* argv[])
{
    sig_init();
    int pid = getpid();
    int status;
    int childPid;
    //printf("Pid: %d\n", pid);
    
    TaskNode* tasks;
    TaskNode* current;
    int remainingTime;

    if(argc < 3)
    {
        //default file names
        inputFile = "/home/input.txt";
        outputFile = "/home/output.txt";
    } 
    else
    {
        //use file names from args
        inputFile = argv[1];
        outputFile = argv[2];
    }

    //start daemon mode
    pid = fork();
    if (pid == -1)
        return -1;
    else if (pid != 0)
        exit (EXIT_SUCCESS);
    
    //new session and process group
    if (setsid ( ) == -1)
        return -1;
    //set working directory to main directory
    if (chdir ("/") == -1)
        return -1;
    //set 0, 1, 2 descriptor to /dev/null
    open ("/dev/null", O_RDWR);
    dup (0);
    dup (0);

    tasks = get_tasks(inputFile);
    merge_sort(&tasks);

    //print_tasks(tasks);
    current = tasks;

    while(current != NULL)
    {
        //printf("\n");
        go_to_current(tasks, &current, &remainingTime);

        while(remainingTime > 0)
        {
            //printf("Sleeping for %d seconds\n", remainingTime);
            //fflush(stdout);

            //wait for next task
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
                sig_usr1(&tasks, inputFile, &current, &remainingTime);
            else if (USR2Status)
                sig_usr2(tasks);

            remainingTime = get_remaining_time(current->task->time);
        }
        
        status = run_task(*current->task);
        current = current->next;
    }

    return 0;
}