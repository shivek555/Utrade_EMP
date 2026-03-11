#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include "graph.h"

#define MAX_WORKERS 10

struct Worker
{
    int busy;
    int task_index;
};

class Scheduler
{
private:

    Task tasks[MAX_TASKS];
    int task_count;

    Graph graph;

    Worker workers[MAX_WORKERS];
    int worker_count;

    int ready[MAX_TASKS];
    int ready_count;

public:

    Scheduler();

    void loadTasks(const char *file);

    void buildGraph();

    void initWorkers(int n);

    void run();

    void updateReady();

    void printReport();

    void exportDOT();

};

#endif