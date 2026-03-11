#include "graph.h"
#include <iostream.h>

void initGraph(Graph &g)
{
    g.task_count = 0;

    for(int i=0;i<MAX_TASKS;i++)
    {
        g.indegree[i]=0;

        for(int j=0;j<MAX_TASKS;j++)
            g.adj[i][j]=0;
    }
}

void addEdge(Graph &g,int u,int v)
{
    g.adj[u][v]=1;
    g.indegree[v]++;
}

int detectCycle(Graph &g)
{
    int q[MAX_TASKS];
    int front=0,rear=0;

    int indeg[MAX_TASKS];

    for(int i=0;i<g.task_count;i++)
        indeg[i]=g.indegree[i];

    for(i=0;i<g.task_count;i++)
        if(indeg[i]==0)
            q[rear++]=i;

    int visited=0;

    while(front<rear)
    {
        int node=q[front++];
        visited++;

        for(int j=0;j<g.task_count;j++)
        {
            if(g.adj[node][j])
            {
                indeg[j]--;
                if(indeg[j]==0)
                    q[rear++]=j;
            }
        }
    }

    if(visited!=g.task_count)
        return 1;

    return 0;
}