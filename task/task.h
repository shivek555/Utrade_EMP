#ifndef TASK_H
#define TASK_H

#include <iostream.h>

#define MAX_DEP 10
#define MAX_TASKS 100

struct Task
{
    char id[10];
    char name[50];

    int priority;
    int duration_ms;

    int dep_count;
    char depends_on[MAX_DEP][10];

    int remaining_time;

    int completed;
    int started;

    long start_time;
    long end_time;

    float fail_probability;

    int retries;
};

#endif