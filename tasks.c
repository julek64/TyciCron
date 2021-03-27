#include "tasks.h"
#include <stdlib.h>
#include <string.h>

struct Task string_to_task(char* string)
{
    struct Task task;
    const char d[2] = ":";
    char *token;
    token = strtok(string, d);
    task.time.hour = atoi(token);
    token = strtok(NULL, d);
    task.time.minute = atoi(token);
    token = strtok(NULL, d);
    strcpy(task.command, token);
    token = strtok(NULL, d);
    task.info = token[0];
}