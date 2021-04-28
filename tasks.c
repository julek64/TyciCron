#include "tasks.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/wait.h>
#include <signal.h>

// convert string from input line to task struct
struct Task* string_to_task(char string[])
{
    struct Task *task = malloc(sizeof(struct Task));
    const char d[2] = ":";
    char* str;
    char* command;

    task->time = malloc(sizeof(struct TaskTime));

    str = strtok(string, d);
    task->time->hour = atoi(str);
    str = strtok(NULL, d);
    task->time->minute = atoi(str);

    str = strtok(NULL, d);
    command = (char *)malloc(strlen(str) * (sizeof(char) + 1));
    strcpy(command, str);
    task->strCommand = (char *)malloc(strlen(str) * (sizeof(char) + 1));
    strcpy(task->strCommand, str);

    str = strtok(NULL, d);
    task->info = atoi(str);

    task->commands = str_to_commands(command);
    free(command);
    return task;
}

// convert string to list of commands
CommandNode* str_to_commands(char *str)
{
    CommandNode* head = malloc(sizeof(CommandNode));
    CommandNode* current = head;
    CommandNode* prev;
    char* savePtr = str;
    char* token;

    const char d[2] = "|";
    token = strtok_r(str, d, &savePtr);
    while(token != NULL)
    {
        current->command = str_to_command(token);
        prev = current;
        current = current->next;
        token = strtok_r(NULL, d, &savePtr);
        if(token != NULL)
        {
            current = malloc(sizeof(CommandNode));
            prev->next = current;
        }
    }

    return head;
}

// convert string to single command
struct Command* str_to_command(char *str)
{
    struct Command* command;
    char* oldStr = malloc(strlen(str) * sizeof(char));
    char** args;
    strcpy(oldStr, str);
    command = malloc(sizeof(struct Command));
    str = strtok(str, " ");
    command->program = malloc(strlen(str) * sizeof(char));
    strcpy(command->program, str);
    int i = 1;
    while(str != NULL)
    {
        str = strtok(NULL, " ");
        i++;
    }
    command->argc = i-1;
    args = malloc(i * sizeof(char*));
    oldStr = strtok(oldStr, " ");
    i = 0;
    while(oldStr != NULL)
    {
        char* cmd = malloc(strlen(oldStr) * sizeof(char));
        strcpy(cmd, oldStr);
        args[i] = cmd;
        oldStr = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;
    command->args = args;
    return command;
}

// get tasks from path file
TaskNode* get_tasks(char *path)
{
    FILE *file;
    TaskNode *current;
    TaskNode *prev;
    TaskNode *first = NULL;
    file = fopen(path, "r");

    if (file == NULL)
        return first;

    char buffer[500];
    while (fgets(buffer, 500, file) != NULL)
    {
        current = malloc(sizeof(TaskNode));
        if (first == NULL)
            first = current;
        else
            prev->next = current;

        current->task = string_to_task(buffer);

        prev = current;
    }
    current->next = NULL;
    fclose(file);

    return first;
}

// convert time from hours and minutes format to minutes value
int time_to_minutes(struct TaskTime* time)
{
    return (time->hour * 60) + time->minute;
}

// get remaining time before execute task 
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

// convert time from hours and minutes format to seconds value
int get_tasktime_seconds(struct TaskTime t)
{
    return t.hour * 3600 + t.minute * 60;
}

// get program and args from Task.strCommand
char** get_program_and_args(char* command)
{
    int len = 0;
    for(int i = 0; i < strlen(command); i++)
    {
        if(command[i] == ' ')
        {
            len++;
        }
    }
    char** args = malloc((len+2) * sizeof(char*));
    char* commandCopy = malloc(strlen(command) * sizeof(char));
    strcpy(commandCopy, command);
    char* str = strtok(commandCopy," ");
    int i = 0;
    while(str != NULL)
    {
        args[i] = malloc((strlen(str) + 1) * (sizeof(char)));
        strcpy(args[i], str);
        str = strtok(NULL, " ");
        fflush(stdout);
        i++;
    }
    args[i] = malloc(sizeof(char));
    args[i] = NULL;

    free(commandCopy);
    return args;
}


// according to the task info, print to outfile task command and stdout/stderr or both
void write_to_file(struct Task task)
{
    int outputFile = open("output.txt", O_WRONLY|O_CREAT|O_APPEND, 0666);

    size_t len = 0;
    len = snprintf(NULL, len, "%d:%d:%s:%d\n", task.time->hour, task.time->minute, task.strCommand, task.info);
    char *command = calloc (1, sizeof *command * len + 1);
    snprintf(command, len + 1, "%d:%d:%s:%d\n", task.time->hour, task.time->minute, task.strCommand, task.info);
    
    write(outputFile, command, strlen(command));

    if(task.info == 0)
    {
        close(1); //Close stdout
        dup(outputFile);
    }
    if(task.info == 1)
    {
        close(2); //Close stderr
        dup(outputFile);
    }
    else if(task.info == 2)
    {
        close(1); //Close stdout
        dup(outputFile);
        close(2); //Close stderr
        dup(outputFile);
    }
}

// run the provided task
int run_task(struct Task task)
{
    //printf("Runnig task: %s\n", task.strCommand);
    int status;
    pid_t child_pid;
    pid_t tpid;
    CommandNode* currentCmd = task.commands;
    int p[2];
    int fdin = 0;

    //create new process
    child_pid = fork();
    if(child_pid == -1)
        return -1;
    else if(child_pid == 0){
        //child process
        signal(SIGINT, SIG_IGN); //ignore signals
        int pipeStatus = 0;
        while(currentCmd != NULL) //execute all commands
        {
            pipe(p);
            pid_t exec_pid = fork(); //create new process
            if (exec_pid == 0)
            {
                signal(SIGINT, SIG_IGN); //ignore signals

                //redirect previous output to input of next process
                dup2(fdin, 0);
                if(currentCmd->next != NULL)
                    dup2(p[1], 1);
                else
                    write_to_file(task); 
                close(p[0]);
                
                //execute command
                status = execvp(currentCmd->command->program, currentCmd->command->args);
                write_to_file(task); //save output to outfile
                if(status == -1)
                    perror("execvp");
                exit(1);
            }
            else
            {
                close(p[1]);
                fdin = p[0];
                currentCmd = currentCmd->next;
            }
            //send info to syslog
            pipeStatus = return_exit_status(exec_pid);
            if (pipeStatus != 0 || currentCmd == NULL)
            {
                send_task_to_log(task, pipeStatus);
                exit(pipeStatus);
            }
        }
        exit(0);
    }
    return child_pid;
}

// print in the console info about tasks and remaining time to execute
void print_tasks(TaskNode* tasks)
{
    TaskNode* current = tasks;
    while(current != NULL)
    {
        printf("%d:%d - %s - %d\n",
        current->task->time->hour,
        current->task->time->minute,
        current->task->strCommand,
        current->task->info);
        printf("remaining sec: %d \n", get_remaining_time(current->task->time));
        current = current->next;
    }
}

// get current task from tasks list and remaining time before execute this task
void go_to_current(TaskNode* tasks, TaskNode** current_pointer, int* remainingTime)
{
    //printf("Going to current task\n");
    *remainingTime = get_remaining_time((*current_pointer)->task->time);
    while(*remainingTime < -59)
    {
        *current_pointer = (*current_pointer)->next;
        if (*current_pointer == NULL)
        {
            *current_pointer = tasks;
            struct TaskTime nextDay;
            nextDay.hour = 23;
            nextDay.minute = 59;
            *remainingTime = get_remaining_time(&nextDay) + 60;
            break;
        }
        *remainingTime = get_remaining_time((*current_pointer)->task->time);
    }
}

// frees up space allocated by tasks
void free_tasks(TaskNode* tasks)
{
    //printf("Freeing tasks\n");
    TaskNode* tmp;
    while (tasks != NULL)
    {
        tmp = tasks;
        tasks = tasks->next;
        free(tmp->task->time);
        CommandNode* command = tmp->task->commands;
        while(command != NULL)
        {
            CommandNode* tmpCommand = command;
            command = tmpCommand->next;
            free(tmpCommand->command->program);
            free(tmpCommand->command->args);
            free(tmpCommand->command);
        }
        free(tmp->task->commands);
        free(tmp->task->strCommand);
        free(tmp->task);
        free(tmp);
    }
}

// send info about upcoming tasks to syslog
void send_left_to_log(TaskNode* tasks)
{
    TaskNode* current = tasks;
    int remainingTime = 0;
    go_to_current(tasks, &current, &remainingTime);
    openlog("Tycicron", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Listing upcoming tasks...");
    while (current != NULL)
    {
        syslog(LOG_INFO, "At [%d:%d] execute [%s:%d]", current->task->time->hour, current->task->time->minute, current->task->strCommand, current->task->info);
        current = current->next;
    }
    closelog();
}

// send info about executing task to syslog
void send_task_to_log(struct Task task, int output)
{
    openlog("TycicronChild", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Executed task [%s] with return value [%d]", task.strCommand, output);
    closelog();
}

// return exit status of a executed task
int return_exit_status(int pid)
{
    int exec_status;
    waitpid(pid, &exec_status, WUNTRACED);
    return WEXITSTATUS(exec_status);
}
