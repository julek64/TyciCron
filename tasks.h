#ifndef TASKS_H
#define TASKS_H

struct TaskTime
{
    int hour;
    int minute;
};

struct Task
{
    struct TaskTime time;
    char* command;
    char info;
};

struct Task string_to_task(char* string);
#endif