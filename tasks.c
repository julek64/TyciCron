#include "tasks.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

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

void run_task(struct Task task)
{
    printf("Runnig task: %s\n", task.command);
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
            *current_pointer = tasks;
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