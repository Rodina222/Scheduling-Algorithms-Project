#include "headers.h"
#include "queue.h"
#include "Priority_Queue.h"
#include<string.h>

double rounding_ya_albi(double num);
void receive(int msgqid_sch,struct msgbuff P_data);
void handler_sigint(int signum);
void handler_sigchild (int signum);
int execute_Process();
char* algoritm_sent;
char* quantum_sent;
int algorithm;
int quantum;
struct Priority_Queue * SRTN_Q;

struct msgbuff P_data;
int msgqid_sch,rec_val;
int num_of_processes;

void out_log(int status,int time, int id, int arr, int total, int remain, int wait, int ta, int wta ); //the first output file log
void out_pref();
//void handler1_sigchild (int signum);
void handler_close_files(int signum);
struct Queue * queue_RR;
struct Priority_Queue * per_q_HPF;
struct Process p_inner;

int total_WTA;
int total_waiting;
int totlal_runtime;
int final_time;

FILE *out_file1;
FILE *out_file2;



int main(int argc, char * argv[])
{
    totlal_runtime = 0;
    total_WTA = 0;
    total_waiting = 0;
   
    out_file1 = fopen ("log.txt", "w+");
    out_file2 = fopen ("perf.txt", "w+");
    if(out_file1 == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file111111 ya wis5\n");
        //exit(EXIT_FAILURE);
    }
    if(out_file2 == NULL)
    {
        /* File not created hence exit */
  
        //exit(EXIT_FAILURE);
    }



    queue_RR = create_queue();
    per_q_HPF = create_pq_queue();

 

    signal (SIGUSR1,handler_sigint);
     signal (SIGINT,handler_close_files);
    signal (SIGCHLD, handler_sigchild); 

    msgqid_sch = msgget(12613,  0644);
    algoritm_sent=argv[1];
    quantum_sent=argv[2];


    initClk();
    algorithm=atoi(algoritm_sent);
    quantum=atoi(quantum_sent);
    printf("\n the Algorithm chosen is :%d \n", algorithm);
    printf("\n the quantum for RR is :%d \n", quantum);


    if (algorithm==1) //HPF
    {
        
        //struct Process p_inner;
        //signal (SIGCHLD,handler1_sigchild);
        while(true)
        {
            // printf("FUCK %d!\n", getClk());
            int a = isEmpty_pq(per_q_HPF);
            //printf("oooh %d \n", a);
            if(isEmpty_pq(per_q_HPF)!=1)
            {
                //printf("\n  AAAAAAAAAAAAAAAAAAAAAAAAAh \n");
                p_inner = peek_pq(per_q_HPF);
                pop(per_q_HPF);
                int real_id = fork();
                if (real_id == -1)
                {
                    perror("error in fork");
                }
                else if (real_id == 0)
                {
                    printf("\nI am about to execve p %d in HPF with runing %d\n", p_inner.id, p_inner.runtime);
                    //p_inner.forking_id=getpid(); //real_id;
                    p_inner.waiting_time = getClk()-p_inner.arrival;
                    p_inner.remaining_time = p_inner.runtime;
                    char remain_time[5];
                    sprintf(remain_time, "%d", p_inner.remaining_time);
                    char *argv[] = { "process.out", remain_time, 0 };
                    execve(argv[0], argv, NULL);
                }
                else
                {
                    printf("\nstart 1 pro\n");
                    //printf("\npleaaaase\n");
                    p_inner.forking_id=real_id;
                    int val, pid;
                    totlal_runtime = totlal_runtime+p_inner.runtime;
                    p_inner.waiting_time=getClk()-p_inner.arrival;
                    out_log(1 , getClk(), p_inner.id, p_inner.arrival, p_inner.runtime, p_inner.runtime, p_inner.waiting_time, 0, 0 );
                    pid = wait(&val);
                    p_inner.exit_time=getClk();
                    p_inner.TA=p_inner.exit_time-p_inner.arrival;
                    p_inner.WTA =  p_inner.TA/p_inner.runtime;
                    total_WTA = total_WTA+p_inner.WTA;
                    total_waiting = total_waiting+p_inner.waiting_time;
                    out_log(4 , getClk(), p_inner.id, p_inner.arrival, p_inner.runtime, 0, getClk()-(p_inner.arrival+p_inner.runtime), p_inner.TA, p_inner.WTA);
                    
                    final_time = getClk();
                                       //printf("\nat time %d process %d finished arr %d total %d remain %d wait %d ", getClk(), p_inner.id, p_inner.exit_time,  );
                    //signal(SIGCHLD,handler1_sigchild);
                }
            }
        }

    }

    else if(algorithm==2) //RR
    {
        while(true)
        {

        }

    }
    else  if(algorithm == 3) //SRTN
    {
        SRTN_Q = create_pq_queue();

        while(isEmpty_pq(SRTN_Q));

        while (!isEmpty_pq(SRTN_Q))
        {
            receive(msgqid_sch,P_data);
            struct Process process1= peek_pq(SRTN_Q);

            printf(" my pid is %d  my priority is %d my runtime is %d\n", process1.forking_id,process1.priority,process1.runtime);
            kill(process1.forking_id,SIGCONT);

            pop(SRTN_Q);

            struct Process process2= peek_pq(SRTN_Q);

            while(process1.remaining_time < process2.remaining_time);

            if (process1.remaining_time > process2.remaining_time)
            {
                kill(process1.forking_id,SIGSTOP);
                kill(process2.forking_id,SIGCONT);
                push(SRTN_Q,process1,process1.remaining_time);


            }
        }


    }



}

void receive(int msgqid_sch,struct msgbuff P_data)
{

    int rec_val;
    rec_val = msgrcv(msgqid_sch, &P_data, sizeof(P_data.pmsg),0, !IPC_NOWAIT);

    if(rec_val == -1)
    {
        perror("Error in receving!");
        exit(-1);
    }
    else
    {
        if (algorithm==1) //HPF
        {
            push(per_q_HPF,P_data.pmsg, P_data.pmsg.priority);
            int a = isEmpty_pq(per_q_HPF);
            //printf("oooh %d \n", a);
        }
        else if(algorithm==2) //RR
        {
            enqueue(queue_RR,P_data.pmsg);
        }
        if(algorithm==3) //STRN
        {
            push(SRTN_Q,P_data.pmsg,P_data.pmsg.runtime);

        }
        ++num_of_processes;
        //printf("Message received is %d\n",P_data.pmsg.id);
    }


}

void handler_sigint(int signum)
{

    //printf("at the top of the handler\n");

    //printf("Helllo from handler\n\n");
    receive(msgqid_sch,P_data);
    //P_data.pmsg.forking_id=execute_Process();
    //kill(P_data.pmsg.forking_id,SIGSTOP);
    /*if (algorithm==1) //HPF
    {
        push(per_q_HPF,P_data.pmsg, P_data.pmsg.priority);
        int a = isEmpty_pq(per_q_HPF);
        printf("oooh %d \n", a);
    }
    else if(algorithm==2) //RR
    {
        enqueue(queue_RR,P_data.pmsg);
    }
    if(algorithm==3) //STRN
    {
        push(SRTN_Q,P_data.pmsg,P_data.pmsg.runtime);

    }
    ++num_of_processes;*/

    //printf("at the end of the handler\n");
    //kill(getpid(),SIGCONT);
    signal (SIGUSR1,handler_sigint);
}


int execute_Process()
{
    int pid_process= fork();
    if (pid_process == -1)
    {
        perror("error in fork");
    }


    else if (pid_process == 0)
    {

        printf("\nI am the process!, my pid = %d and my parent's pid = %d\n\n", getpid(), getppid());
        char pp_id[10];
        char pp_arrival[10];
        char pp_runtime[10];
        char pp_priority[10];

        sprintf(pp_id, "%d", P_data.pmsg.id);
        sprintf(pp_arrival, "%d", P_data.pmsg.arrival);
        sprintf(pp_runtime, "%d",P_data.pmsg.runtime);
        sprintf(pp_priority, "%d", P_data.pmsg.priority);

        char *argv[] = { "outProcess",pp_id,pp_arrival,pp_runtime,pp_priority,0 };

        execve(argv[0], argv, NULL);
    }

    return pid_process;
}

void handler_sigchild (int signum)
{
    int pd;
    int status;
    struct Process process;

    pd = wait(&status);

    while (WIFEXITED(status))
    {
        --num_of_processes;
        printf("\nA PROCESS with pid %d terminated with exit code %d\n", pd, WEXITSTATUS(status));


    }
    signal(SIGCHLD,handler_sigchild);

}



void out_log(int status,int time, int id, int arr, int total, int remain, int wait, int ta, int wta )
{
    char mode[20];

    if(status==1) //started
    {
        strcpy(mode, "started");  
    }
    else if(status==2) //stopped
    {
        strcpy(mode, "stopped");
    }
    else if(status==3) //resumed
    {
        strcpy(mode, "resumed");
    }
    else if(status==4) //finished
    {
        strcpy(mode, "finished");
    }
    //At time x process y state arr w total z remain y wait k
    if(status==4)
    {
        fprintf(out_file1, "At time %d,  process %d,  %s arr %d,  total %d,  remain %d,  wait %d,  TA %d,  WTA %d\n",time ,id ,mode ,arr ,total ,remain ,wait ,ta , wta);
      
    }
    else
    {
        fprintf(out_file1, "At time %d,  process %d,  %s arr %d,  total %d,  remain %d,  wait %d\n",time ,id ,mode ,arr ,total, remain, wait );
    }


}
void out_pref()
{
    double a = totlal_runtime;
    double b = final_time;
    double CPU_utilization = (a*100/b);
    double rounded = rounding_ya_albi(CPU_utilization);
 
    double nums = num_of_processes;

    double f = total_WTA;
    double avg_t = f/nums;
    double AVG_T = rounding_ya_albi(avg_t);


    double e = total_waiting;
    double avg_w = e/nums;
    double AVG_W = rounding_ya_albi(avg_w);

    fprintf(out_file2, "totlal runtime is: %d, and it ended at %d, so the CPU utilization is: %0.2f%%\n", totlal_runtime, final_time, rounded);
    fprintf(out_file2, "processes count: %d\n", num_of_processes);
    fprintf(out_file2, "avg WTA is: %0.2f\n", AVG_T);
    fprintf(out_file2, "avg Waiting is: %0.2f\n", AVG_W);
    fprintf(out_file2, "std WTA\n");
}
double rounding_ya_albi(double num)
{
    num = num+0.005;
    num = num*100;
    int c = num;
    double d = c;
    num = d/100;
    return num;

}
void handler_close_files(int signum)
{
    fclose (out_file1);
    out_pref();
    fclose (out_file2);
     exit(0);
}