#ifndef TASKS_H
#define TASKS_H

struct TaskTime
{
    int hour;
    int minute;
};

struct Command
{
    char* program;
    char** args;
    short argc;
};

typedef struct CommandListNode
{
    struct Command* command;
    struct CommandListNode* next;
} CommandNode;

struct Task
{
    struct TaskTime* time;
    CommandNode* commands;
    char* strCommand;
    char info;
};

typedef struct TaskListNode
{
    struct Task* task;
    struct TaskListNode* next;
} TaskNode;

struct Task* string_to_task(char string[]);
CommandNode* str_to_commands(char *str);
struct Command* str_to_command(char *str);
TaskNode* get_tasks(char* path);
int time_to_minutes(struct TaskTime* time);
int get_remaining_time(struct TaskTime* TaskTime);
int get_tasktime_seconds(struct TaskTime);
void write_to_file(struct Task);
int run_task(struct Task);
void print_tasks(TaskNode*);
void go_to_current(TaskNode*, TaskNode**, int*);
void free_tasks(TaskNode*);
void send_left_to_log(TaskNode*);
void send_task_to_log(struct Task, int);
int send_task_to_log_on_finish(struct Task, int);
#endif