#include "headers.h"
#include "queue.h"
#include <string.h>
#include <stdio.h>

void clearResources(int);


/////////////////////////////Mariam//////////////////////////

int algorithm ;  // integer represent scheduling algorithm 1 for HPF , 2 RR ,3 SRTN
int quantum;    // time slot for RR;

void read_file(struct Queue *q)
{
    FILE * fptr;
    fptr=fopen("input","r");
    if(fptr==NULL)
    {
        printf("\nError in opening the file\n");
        exit(-1);
    }
    const unsigned MAX_LENGTH = 25;
    char buffer[MAX_LENGTH];
    char * token;
    while (fgets(buffer, MAX_LENGTH, fptr))
    {

        struct Process p;
        token= strtok(buffer," ");
        p.id=atoi(token);
        token= strtok(NULL," ");
        p.arrival=atoi(token);

        token= strtok(NULL," ");
        p.runtime=atoi(token);


        token= strtok(NULL," ");
        p.priority=atoi(token);
        token= strtok(NULL," ");
        p.memory_size=atoi(token);
        enqueue(q,p);
    }

    fclose(fptr);
    printf("After Read\n");

}





int MsgQ_id;

void send(struct msgbuff);

int main(int argc, char * argv[])
{

    signal(SIGINT, clearResources);

    int pd;
    int status;

    MsgQ_id = msgget(12613, IPC_CREAT | 0644);
    printf("MsgQ_is %d \n",MsgQ_id );


    if(MsgQ_id == -1)
    {
        perror("Error in create!");
        exit(-1);
    }
    // TODO Initialization
    // 1. Read the input files.
    struct Queue * queue =create_queue(); // read file into queue of processes
    struct Process process;
    read_file(queue);
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    printf("\n Please Enter the Preferred Algorithm: (1-HPF) ,(2-RR) ,(3-SRTN)\n");
    scanf("%d",&algorithm);
    while(algorithm!=1 && algorithm!=2 && algorithm !=3 )
    {
        printf("\n Please Enter the Preferred Algorithm: (1-HPF) ,(2-RR) ,(3-SRTN)\n");
        scanf("%d",&algorithm);
    }
    if(algorithm==2) {
        printf("\n Please Enter Quantum for RR\n");
        scanf("%d",&quantum);
    }

    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock

    int pid_clk = fork();


    if (pid_clk == -1)
        perror("error in fork");

    else if (pid_clk == 0)
    {
        printf("\nI am the clock!, my pid = %d and my parent's pid = %d\n\n", getpid(), getppid());

        char *argv[] = { "clk.out",0 };

        execve(argv[0], argv, NULL);


    }

    else
    {
        //printf("\nI am the process generator, my pid = %d \n\n", getpid());

        int pid_Scheduler= fork();

        if (pid_Scheduler == -1)

            perror("error in fork");

        else if (pid_Scheduler == 0)
        {
            printf("\nI am the scheduler!, my pid = %d and my parent's pid = %d\n", getpid(), getppid());
            char alg [5];
            char quant[5];
            sprintf(alg, "%d", algorithm);
            sprintf(quant, "%d", quantum);
            char *argv[] = { "scheduler.out",alg,quant, 0 };
            int n = execve(argv[0], argv, NULL);

            printf("%d\n", n);


        }

        else
        {
            initClk();
            // printf("The clock is initialized successfully inside the process generator!\n");
            int clock ;
            int checker=1;

            while(checker==1)
            {

                clock = getClk();

                process=peek(queue);  //peek the first process in the queue
               
                while( process.arrival == clock) //check if the arrival time of the process = the current time
                {
                    int n=process.arrival;
                    struct msgbuff P_data;
                    P_data.mtype=1;
                    P_data.pmsg=dequeue(queue);
                    printf("P_data.pmsg.id  %d\n",P_data.pmsg.id);

                    send(P_data);
                    printf("After send\n");

                    kill(pid_Scheduler,SIGUSR1);
                    process=peek(queue);
                    if(process.arrival!=-1)
                    sleep(process.arrival-n);

                    if(process.id== 0) {
                        P_data.pmsg.id=-2;
                        send(P_data);
                        checker=0;
                    }


                }

            }

            pd = wait(&status);

            while(WIFEXITED(status))

            {
                if(pd == pid_clk)
                {
                    pd = wait(&status);
                }


                if ( pd == pid_Scheduler)

                {
                    printf("\n The scheduler ends his job and terminated ! \n");
                    clearResources(2);


                }
            }

        }

    }





}

void send(struct msgbuff P_data)
{

    int send_val;
   printf("MsgQ_id %d,P_data.id %d and size is %ld , P_data.type %ld \n",MsgQ_id ,P_data.pmsg.id,sizeof(P_data.pmsg),P_data.mtype);
    send_val = msgsnd(MsgQ_id, &P_data, sizeof(P_data.pmsg), !IPC_NOWAIT);
    //printf("Msg send\n");

    if(send_val == -1)
    {
        perror("Error in sending!");
        exit(-1);
    }
    else
    {

    }


}



void clearResources(int signum)
{
    msgctl(MsgQ_id, IPC_RMID, (struct msqid_ds *) 0);
    destroyClk(true);
    exit(0);
    //TODO Clears all resources in case of interruption
}

