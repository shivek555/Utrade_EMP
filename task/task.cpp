#include "task.h"
#include <string.h>

void initTask(Task &t)
{
    t.dep_count = 0;
    t.remaining_time = 0;
    t.completed = 0;
    t.started = 0;
    t.fail_probability = 0;
    t.retries = 0;
}