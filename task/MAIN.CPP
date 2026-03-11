#include <iostream.h>
#include <stdlib.h>
#include <scheduler.h>
#include<string.h>

int main(int argc,char* argv[])
{   int k;
    int workers=4;

    if(argc>2)
    {
	if(strcmp(argv[1],"--workers")==0)
	    workers=atoi(argv[2]);
    }

    scheduler s;

    s.loadTasks("example_tasks.json");

    s.buildGraph();

    s.initWorkers(workers);

    s.run();

    s.printReport();

    s.exportDOT();

    cin>>k;
    return 0;
}