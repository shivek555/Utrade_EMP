#include "task.h"

int selectHighestPriority(Task tasks[],int ready[],int ready_count)
{
    int best=-1;

    for(int i=0;i<ready_count;i++)
    {
        int idx=ready[i];

        if(best==-1 || tasks[idx].priority < tasks[best].priority)
            best=idx;
    }

    return best;
}