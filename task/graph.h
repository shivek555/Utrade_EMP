#ifndef GRAPH_H
#define GRAPH_H

#include "task.h"

struct Graph
{
    int adj[MAX_TASKS][MAX_TASKS];
    int indegree[MAX_TASKS];
    int task_count;
};

void initGraph(Graph &g);
void addEdge(Graph &g, int u, int v);
int detectCycle(Graph &g);

#endif