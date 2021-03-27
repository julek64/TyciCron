#include "tasks.h"
#include <stdlib.h>
#include <string.h>

struct Task string_to_task(char string[])
{
    struct Task task;
    const char d[2] = ":";
    char * str;
    str = strtok (string, d);
    task.time.hour = atoi(str);
    str = strtok (NULL, d);
    task.time.minute = atoi(str);
    str = strtok (NULL, d);
    task.command = (char *)malloc(strlen(str) * (sizeof(char) + 1));
    strcpy(task.command, str);
    str = strtok (NULL, d);
    task.info = atoi(str);
    return task;
}