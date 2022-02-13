#include "headers.h"
#include <time.h>

/* Modify this file as needed*/
int remaining_time;
int running_time;

int main(int agrc, char * argv[])
{
    initClk();

    running_time=atoi(argv[1]);
    remaining_time=atoi(argv[1]);
    int a=getClk();
    // printf("Proccess begin %s\n",argv[2]);
    while (remaining_time > 0)
    {

        int run_time= clock() /CLOCKS_PER_SEC;
        remaining_time=running_time-run_time;


    }


    int b=getClk();

    printf("Procces with runtime %d exit at time %d\n",running_time,b);
    destroyClk(0);

    exit(getpid());





    return 0;
}
