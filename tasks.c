#include "tasks.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/wait.h>
#include <signal.h>

struct Task* string_to_task(char string[])
{
    struct Task* task = malloc(sizeof(struct Task));
    const char d[2] = ":";
    char * str;

    task->time = malloc(sizeof(struct TaskTime));

    str = strtok (string, d);
    task->time->hour = atoi(str);
    str = strtok (NULL, d);
    task->time->minute = atoi(str);
    str = strtok (NULL, d);
    task->command = (char *)malloc(strlen(str) * (sizeof(char) + 1));
    strcpy(task->command, str);
    str = strtok (NULL, d);
    task->info = atoi(str);
    return task;
}

TaskNode* get_tasks(char* path)
{
    FILE * file;
    TaskNode* current;
    TaskNode* prev;
    TaskNode* first = NULL;
    file = fopen(path, "r");

    if(file == NULL)
        return first;

    char buffer[500];
    while(fgets(buffer, 500, file) != NULL)
    {
        current = malloc(sizeof(TaskNode));
        if(first == NULL)
            first = current;
        else
            prev->next = current;
        
        current->task = string_to_task(buffer);
        
        prev = current;
    }
    current->next = NULL;
    fclose(file);

    if (first == NULL)
        printf("Gotten tasks are NULL");

    return first;
}

int time_to_minutes(struct TaskTime* time)
{
    return (time->hour * 60) + time->minute;
}

int get_remaining_time(struct TaskTime* TaskTime)
{
    time_t now = time(0);
    struct tm local;
    int localSeconds;
    int remainingSeconds;
    localtime_r(&now, &local);

    localSeconds = local.tm_hour * 3600 + local.tm_min * 60 + local.tm_sec;
    remainingSeconds = get_tasktime_seconds(*TaskTime) - localSeconds;
    return remainingSeconds;
}

int get_tasktime_seconds(struct TaskTime t)
{
    return t.hour * 3600 + t.minute * 60;
}

char** get_program_and_args(char* command)
{
    int len = 0;
    for(int i = 0; i < strlen(command); i++)
    {
        if(command[i] == ' ')
        {
            len++;
        }
    }
    char** args = malloc((len+2) * sizeof(char*));
    char* commandCopy = malloc(strlen(command) * sizeof(char));
    strcpy(commandCopy, command);
    char* str = strtok(commandCopy," ");
    int i = 0;
    while(str != NULL)
    {
        args[i] = malloc((strlen(str) + 1) * (sizeof(char)));
        strcpy(args[i], str);
        str = strtok(NULL, " ");
        fflush(stdout);
        i++;
    }
    args[i] = malloc(sizeof(char));
    args[i] = NULL;

    free(commandCopy);
    return args;
}

void write_to_file(struct Task task)
{
    int outputFile = open("output.txt", O_WRONLY|O_CREAT|O_APPEND, 0666);

    size_t len = 0;
    len = snprintf(NULL, len, "\n\n\n%d:%d:%s:%d\n\n", task.time->hour, task.time->minute, task.command, task.info);
    char *command = calloc (1, sizeof *command * len + 1);
    snprintf(command, len + 1, "\n\n\n%d:%d:%s:%d\n\n", task.time->hour, task.time->minute, task.command, task.info);
    
    write(outputFile, command, strlen(command));

    if(task.info == 0)
    {
        close(1); //Close stdout
        dup(outputFile);
    }
    if(task.info == 1)
    {
        close(2); //Close stderr
        dup(outputFile);
    }
    else if(task.info == 2)
    {
        close(1); //Close stdout
        dup(outputFile);
        close(2); //Close stderr
        dup(outputFile);
    }
}

int run_task(struct Task task)
{
    printf("Runnig task: %s\n", task.command);
    char** programAndArgs = get_program_and_args(task.command);
    int status;
    pid_t child_pid;
    pid_t tpid;

    int i = 1;
    char* program = programAndArgs[0];


    while(programAndArgs[i] != NULL)
        i++;

    char* args[i];
    i = 0; 

    while(programAndArgs[i] != NULL)
    {
        args[i] = programAndArgs[i];
        i++;
    }
    args[i] = NULL;
    
    child_pid = fork();
    if(child_pid == -1)
        return -1;
    else if(child_pid == 0){
        //child process
        signal(SIGINT, SIG_IGN);
        write_to_file(task);
        status = execvp(program, args);   
        if(status == -1)
            perror("execvp");
        exit(1);
    }

    return child_pid;
}

void print_tasks(TaskNode* tasks)
{
    TaskNode* current = tasks;
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
}

void go_to_current(TaskNode* tasks, TaskNode** current_pointer, int* remainingTime)
{
    printf("\ngoing to current task\n");
    *remainingTime = get_remaining_time((*current_pointer)->task->time);
    while(*remainingTime < -59)
    {
        *current_pointer = (*current_pointer)->next;
        if (*current_pointer == NULL)
        {
            *current_pointer = tasks;
            struct TaskTime nextDay;
            nextDay.hour = 23;
            nextDay.minute = 59;
            *remainingTime = get_remaining_time(&nextDay) + 60;
            break;
        }
        *remainingTime = get_remaining_time((*current_pointer)->task->time);
    }
}

void free_tasks(TaskNode* tasks)
{
    printf("\nfreeing tasks\n");
    TaskNode* tmp;
    while (tasks != NULL)
    {
        tmp = tasks;
        tasks = tasks->next;
        free(tmp);
    }
}

void send_left_to_log(TaskNode* tasks)
{
    TaskNode* current = tasks;
    int remainingTime = 0;
    go_to_current(tasks, &current, &remainingTime);
    openlog("Tycicron", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Listing upcoming tasks...");
    while (current != NULL)
    {
        syslog(LOG_INFO, "At [%d:%d] execute [%s:%d]", current->task->time->hour, current->task->time->minute, current->task->command, current->task->info);
        current = current->next;
    }
    closelog();
}