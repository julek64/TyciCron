#ifndef TASKS_H
#define TASKS_H
struct taskTime
{
    int hour;
    int minute;
};

struct task
{
    taskTime time;
    char* command;
    char info;
};
#endif