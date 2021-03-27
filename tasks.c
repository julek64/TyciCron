#include "tasks.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    fclose(file);
    return first;
}