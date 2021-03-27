#include "tasks.h"

int main(int argc, char* argv)
{
    char str[] ="12:45:test ets stdf:1";
    struct Task task = string_to_task(str);
    return 0;
}