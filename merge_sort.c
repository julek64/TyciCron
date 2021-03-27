#include <stdio.h>
#include <stdlib.h>
#include "tasks.h"
#include "merge_sort.h"

TaskNode* merge(TaskNode* a, TaskNode* b);
void split(TaskNode* source, TaskNode** front_pointer, TaskNode** back_pointer);
int time_to_minutes(struct TaskTime*);

void merge_sort(TaskNode** root_pointer)
{
    TaskNode* root = *root_pointer;
    TaskNode* a;
    TaskNode* b;
  
    if ((root == NULL) || (root->next == NULL)) {
        return;
    }
  
    split(root, &a, &b);
  
    merge_sort(&a);
    merge_sort(&b);
  
    *root_pointer = merge(a, b);
}

TaskNode* merge(TaskNode* a, TaskNode* b)
{
    TaskNode* result = NULL;
  
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);
  
    if (time_to_minutes(a->task->time) <= time_to_minutes(b->task->time))
    {
        result = a;
        result->next = merge(a->next, b);
    }
    else 
    {
        result = b;
        result->next = merge(a, b->next);
    }
    return (result);
}

void split(TaskNode* source, TaskNode** front_pointer, TaskNode** back_pointer)
{
    TaskNode* fast;
    TaskNode* slow;
    slow = source;
    fast = source->next;
  
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
  
    *front_pointer = source;
    *back_pointer = slow->next;
    slow->next = NULL;
}