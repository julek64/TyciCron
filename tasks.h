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
int time_to_minutes(struct TaskTime* time);
int get_remaining_time(struct TaskTime* TaskTime);
int get_tasktime_seconds(struct TaskTime);
void run_task(struct Task);
void print_tasks(TaskNode*);
void go_to_current(TaskNode*, TaskNode**, int*);
void free_tasks(TaskNode*);
#endif