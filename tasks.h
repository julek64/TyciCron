#ifndef TASKS_H
#define TASKS_H

struct TaskTime
{
    int hour;
    int minute;
};

struct Task
{
    struct TaskTime* time;
    char* command;
    char info;
};

typedef struct TaskListNode
{
    struct Task* task;
    struct TaskListNode* next;
} TaskNode;

struct Task* string_to_task(char string[]);
TaskNode* get_tasks(char* path);
#endif