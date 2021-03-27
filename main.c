#include "tasks.h"
#include "merge_sort.h"
#include <stdlib.h>

int main(int argc, char* argv)
{
    TaskNode* tasks = get_tasks("input.txt");
    merge_sort(&tasks);
    while(tasks != NULL)
    {
        printf("%d:%d - %s - %d\n",
        tasks->task->time->hour,
        tasks->task->time->minute,
        tasks->task->command,
        tasks->task->info);
        tasks = tasks->next;
    }
    return 0;
}