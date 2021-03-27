#ifndef TASKS_H
#define TASKS_H
struct TaskTime
{
    int hour;
    int minute;
};

struct Task
{
    TaskTime time;
    char* command;
    char info;
};
#endif