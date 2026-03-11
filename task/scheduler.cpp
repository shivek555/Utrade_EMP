#include "scheduler.h"
#include <fstream.h>
#include <string.h>
#include <dos.h>
#include <time.h>
#include <stdlib.h>

extern int selectHighestPriority(Task tasks[],int ready[],int ready_count);

Scheduler::Scheduler()
{
    task_count=0;
    ready_count=0;
}

void Scheduler::initWorkers(int n)
{
    worker_count=n;

    for(int i=0;i<n;i++)
    {
        workers[i].busy=0;
        workers[i].task_index=-1;
    }
}

void Scheduler::updateReady()
{
    ready_count=0;

    for(int i=0;i<task_count;i++)
    {
        if(tasks[i].completed || tasks[i].started)
            continue;

        if(graph.indegree[i]==0)
        {
            ready[ready_count++]=i;
        }
    }
}

void Scheduler::run()
{
    long global_start=clock();

    while(1)
    {
        updateReady();

        for(int w=0;w<worker_count;w++)
        {
            if(workers[w].busy==0 && ready_count>0)
            {
                int idx=selectHighestPriority(tasks,ready,ready_count);

                workers[w].busy=1;
                workers[w].task_index=idx;

                tasks[idx].started=1;
                tasks[idx].start_time=clock();
                tasks[idx].remaining_time=tasks[idx].duration_ms;

                cout<<"START worker "<<w<<" task "<<tasks[idx].id<<endl;
            }
        }

        delay(100);

        for(w=0;w<worker_count;w++)
        {
            if(workers[w].busy)
            {
                int t=workers[w].task_index;

                tasks[t].remaining_time-=100;

                if(tasks[t].remaining_time<=0)
                {
                    tasks[t].completed=1;
                    tasks[t].end_time=clock();

                    cout<<"DONE worker "<<w<<" task "<<tasks[t].id<<endl;

                    workers[w].busy=0;

                    for(int j=0;j<task_count;j++)
                        if(graph.adj[t][j])
                            graph.indegree[j]--;
                }
            }
        }

        int done=1;

        for(int i=0;i<task_count;i++)
            if(!tasks[i].completed)
                done=0;

        if(done)
            break;
    }

    cout<<"Total time "<<clock()-global_start<<endl;
}