#include "headers.h"
#include "queue.h"
#include <math.h>
#include "Priority_Queue.h"
#include "Priority_Queue_Mem.h"



void receive(int msgqid_sch,struct msgbuff P_data);
void handler_sigusr1(int signum);
void handler_sigchild (int signum);
void handler_sigint (int signum);
void execute_Process();
char* algoritm_sent;
char* quantum_sent;
int algorithm;
int quantum;
int status;
int receving_flag;
struct msgbuff P_data;
int msgqid_sch,rec_val;
///////////JEEEEEEEEEEE
double rounding_ya_albi(double num);


void out_log(int status,int time, int id, int arr, int total, int remain, int wait, int ta, int wta ); //the first output file log
void out_pref();
void handler_close_files();
struct Process p_inner;

int total_WTA;
int total_waiting;
int totlal_runtime;
int final_time;

FILE *out_file1;
FILE *out_file2;

struct Queue * queue_RR;
struct Priority_Queue * per_q_HPF;


int total_WTA;
int total_waiting;
FILE *out_file1;
FILE *out_file2;
///// SRTN ALgorithm Attributes
int num_of_processes;
struct Priority_Queue * SRTN_Q;
struct Process process_rec;
int flag_all_rec=0;
struct Process p_inner;
struct Process running_process;
struct Process ready_process;
int stop;
//MEMORY MANAGMENT 
struct Priority_Queue_mem * L1024 ;
struct Priority_Queue_mem * L512 ;
struct Priority_Queue_mem * L256 ;
struct Priority_Queue_mem * L128 ;
struct Priority_Queue_mem * L64  ;
struct Priority_Queue_mem * L32 ;
struct Priority_Queue_mem * L16 ;
struct Priority_Queue_mem * L8 ;

int calculate_size(int s){
    double n=log10(s)/log10(2);
    n=ceil(n);
    return n;
}
void Allocate(struct Process p,FILE *out_file1, int time){
    int n=calculate_size(p.memory_size);
    int end=0;

    if(n==10)
    {
       int Add= peek_pq_mem(L1024);
       pop_mem(L1024);
       p.memory_Add=Add;
       end=Add+1024-1;
       fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
    }

    if(n==9)
    {
    if(!isEmpty_pq_mem(L512))
    {
     int Add= peek_pq_mem(L512);
     p.memory_Add=Add;
      pop_mem(L512);
      end=Add+512-1;
      fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
    }
    else
    {
    if(!isEmpty_pq_mem(L1024))
    {
        int Add= peek_pq_mem(L1024);
         pop_mem(L1024);
         push_mem(L512,Add+512);
         p.memory_Add=Add;
         end=Add+511;
         fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
    }
    }
    }
    ///// If n==8
    if(n==8)
    {
        if(!isEmpty_pq_mem(L256))
        {
        int Add= peek_pq_mem(L256);
        p.memory_Add=Add;
        pop_mem(L256);
        end=Add+256-1;
        fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
        }
        else
        {
            if(!isEmpty_pq_mem(L512))
            {
                int Add= peek_pq_mem(L512);
                pop_mem(L512);
                p.memory_Add=Add;
                end=Add+256-1;
                push_mem(L256,end+1);
                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
            }
            else if(isEmpty_pq_mem(L1024))
            {
                if(!isEmpty_pq_mem(L1024))
                {
                 int Add= peek_pq_mem(L1024);
                 pop_mem(L1024);
                 push_mem(L512,Add+512);
                 p.memory_Add=Add;
                 end=Add+255;
                 push_mem(L256,end+1);
                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                }
            }

        }
    }
   ///// If n==7
    if(n == 7)
    {
        if(!isEmpty_pq_mem(L128))
        {
        int Add= peek_pq_mem(L128);
        p.memory_Add=Add;
        pop_mem(L128);
        end=Add+127;
        fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
        }
        else 
            {
                if(!isEmpty_pq_mem(L256))
                {
                 int Add= peek_pq_mem(L256);
                 pop_mem(L256);
                 p.memory_Add=Add;
                 end=Add+127;
                push_mem(L128,end+1);
                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                }

        
                else if(!isEmpty_pq_mem(L512))
              {
                int Add= peek_pq_mem(L512);
                p.memory_Add=Add;
                pop_mem(L512);
                end=Add+127;
                push_mem(L256,Add+256);
                push_mem(L128,end+1);
                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
             }

                else if(!isEmpty_pq_mem(L1024))
              {
                int Add= peek_pq_mem(L1024);
                p.memory_Add=Add;
                pop_mem(L1024);
                end=Add+127;
                push_mem(L512,Add+512);
                push_mem(L256,Add+256);
                push_mem(L128,end+1);
                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
             }


                
            }


    }

 ///// If n==6
    if (n == 6)
    {
         if(!isEmpty_pq_mem(L64))
        {
        int Add= peek_pq_mem(L64);
        p.memory_Add=Add;
        pop_mem(L64);
        end=Add+63;
        fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
        }
 

            else 
            {
                if(!isEmpty_pq_mem(L128))
                {
                 int Add= peek_pq_mem(L128);
                 pop_mem(L128);
                 p.memory_Add=Add;
                 end=Add+63;
                 push_mem(L64,end+1);

                 fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                }


                else if(!isEmpty_pq_mem(L256))
                {
                 int Add= peek_pq_mem(L256);
                 pop_mem(L256);
                 p.memory_Add=Add;
                 end=Add+63;
                 push_mem(L128,Add+128);
                 push_mem(L64,end+1);

                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                }  

        
                else if(!isEmpty_pq_mem(L512))
              {
                int Add= peek_pq_mem(L512);
                p.memory_Add=Add;
                pop_mem(L512);
                end=Add+63;
                push_mem(L256,Add+256);
                push_mem(L128,Add+128);
                push_mem(L64,end+1);

                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
              }

                else if(!isEmpty_pq_mem(L1024))
              {
                int Add= peek_pq_mem(L1024);
                p.memory_Add=Add;
                pop_mem(L1024);
                end=Add+63;
                push_mem(L512,Add+512);
                push_mem(L256,Add+256);
                push_mem(L128,Add+128);
                push_mem(L64,end+1);

                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
             }  


                
            }


    }
  ///// If n==5
    if (n == 5)
    {
        if(!isEmpty_pq_mem(L32))
        {
        int Add= peek_pq_mem(L32);
        p.memory_Add=Add;
        pop_mem(L32);
        end=Add+31;
        fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
        }


        else 
            {
                 if(!isEmpty_pq_mem(L64))
                  {
                int Add= peek_pq_mem(L64);
                p.memory_Add=Add;
                pop_mem(L64);
                end=Add+31;
                push_mem(L32,end+1);
                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                  }

                else if(!isEmpty_pq_mem(L128))
                {
                 int Add= peek_pq_mem(L128);
                 pop_mem(L128);
                 p.memory_Add=Add;
                 end=Add+31;
                 push_mem(L64,Add+64);
                 push_mem(L32,end+1);

                 fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                }


                else if(!isEmpty_pq_mem(L256))
                {
                 int Add= peek_pq_mem(L256);
                 pop_mem(L256);
                 p.memory_Add=Add;
                 end=Add+31;
                 push_mem(L128,Add+128);
                 push_mem(L64,Add+64);
                 push_mem(L32,end+1);

                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                }  

        
                else if(!isEmpty_pq_mem(L512))
              {
                int Add= peek_pq_mem(L512);
                p.memory_Add=Add;
                pop_mem(L512);
                end=Add+31;
                push_mem(L256,Add+256);
                push_mem(L128,Add+128);
                push_mem(L64,Add+64);
                push_mem(L32,end+1);

                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
              }

                else if(!isEmpty_pq_mem(L1024))
              {
                int Add= peek_pq_mem(L1024);
                p.memory_Add=Add;
                pop_mem(L1024);
                 end=Add+31;
                push_mem(L512,Add+512);
                push_mem(L256,Add+256);
                push_mem(L128,Add+128);
                push_mem(L64,Add+64);
                push_mem(L32,end+1);

                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
             }  


                
            }

    }
 ///// If n==4
    if(n==4)
    {
        if(!isEmpty_pq_mem(L16))
        {
        int Add= peek_pq_mem(L16);
        p.memory_Add=Add;
        pop_mem(L16);
        end=Add+15;
        fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
        }
        


        else 
       {

                 if(!isEmpty_pq_mem(L32))
                 {
                 int Add= peek_pq_mem(L32);
                 p.memory_Add=Add;
                 pop_mem(L32);
                 end=Add+15;
                 push_mem(L16,end+1);

                 fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                  }

                else if(!isEmpty_pq_mem(L64))
                  {
                int Add= peek_pq_mem(L64);
                p.memory_Add=Add;
                pop_mem(L64);
                end=Add+15;
                push_mem(L32,end+32);
                push_mem(L16,end+1);


                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                  }

                else if(!isEmpty_pq_mem(L128))
                {
                 int Add= peek_pq_mem(L128);
                 pop_mem(L128);
                 p.memory_Add=Add;
                 end=Add+15;
                 push_mem(L64,Add+64);
                 push_mem(L32,end+32);
                 push_mem(L16,end+1);


                 fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                }


                else if(!isEmpty_pq_mem(L256))
                {
                 int Add= peek_pq_mem(L256);
                 pop_mem(L256);
                 p.memory_Add=Add;
                 end=Add+15;
                 push_mem(L128,Add+128);
                 push_mem(L64,Add+64);
                 push_mem(L32,end+32);
                 push_mem(L16,end+1);

                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
                }  

        
                else if(!isEmpty_pq_mem(L512))
              {
                int Add= peek_pq_mem(L512);
                p.memory_Add=Add;
                pop_mem(L512);
                end=Add+15;
                push_mem(L256,Add+256);
                push_mem(L128,Add+128);
                push_mem(L64,Add+64);
                push_mem(L32,end+32);
                push_mem(L16,end+1);

                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
              }

                else if(!isEmpty_pq_mem(L1024))
              {
                int Add= peek_pq_mem(L1024);
                p.memory_Add=Add;
                pop_mem(L1024);
                end=Add+15;
                push_mem(L512,Add+512);
                push_mem(L256,Add+256);
                push_mem(L128,Add+128);
                push_mem(L64,Add+64);
                push_mem(L32,end+32);
                push_mem(L16,end+1);


                fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
             }  


                
        }


    }

 ///// If n==3
    if(n==3)
    {
        if(!isEmpty_pq_mem(L8))
        {
         int Add= peek_pq_mem(L8);
        p.memory_Add=Add;
      pop_mem(L8);
      end=Add+8-1;
      fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
        }
        else
        {
            if(!isEmpty_pq_mem(L16))
            {
          int Add= peek_pq_mem(L16);
          p.memory_Add=Add;
          pop_mem(L16);
          end=Add+8-1;
          push_mem(L8,end+1);
          fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
            }
           else if(!isEmpty_pq_mem(L32))
            {
            int Add= peek_pq_mem(L32);
            p.memory_Add=Add;
             pop_mem(L32);
             end=Add+8-1;
             push_mem(L16,Add+16);
             push_mem(L8,end+1);
             fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
            }
            else if(!isEmpty_pq_mem(L64))
            {
            int Add= peek_pq_mem(L64);
            p.memory_Add=Add;
             pop_mem(L64);
             push_mem(L32, Add+32);
             push_mem(L16,Add+16);
             push_mem(L8,Add+8);
             end=Add+8-1;
             fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
            }
            else if(!isEmpty_pq_mem(L128))
            {
            int Add= peek_pq_mem(L128);
            p.memory_Add=Add;
             pop_mem(L128);
             push_mem(L64, Add+64);
             push_mem(L32,Add+32);
             push_mem(L16,Add+16);
             push_mem(L8,Add+8);
             end=Add+8-1;
             fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
            }
            else if(!isEmpty_pq_mem(L256))
            {
            int Add= peek_pq_mem(L256);
            p.memory_Add=Add;
             pop_mem(L256);
             push_mem(L128, Add+128);
             push_mem(L64,Add+64);
             push_mem(L32,Add+32);
             push_mem(L16,Add+16);
             push_mem(L8,Add+8);
             end=Add+8-1;
             fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
            }
            else if(!isEmpty_pq_mem(L512))
            {
            int Add= peek_pq_mem(L512);
            p.memory_Add=Add;
             pop_mem(L512);
              push_mem(L256, Add+256);
             push_mem(L128, Add+128);
             push_mem(L64,Add+64);
             push_mem(L32,Add+32);
             push_mem(L16,Add+16);
             push_mem(L8,Add+8);
             end=Add+8-1;
             fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
            }
            else if(!isEmpty_pq_mem(L512))
            {
            int Add= peek_pq_mem(L1024);
            p.memory_Add=Add;
             pop_mem(L1024);
            push_mem(L512, Add+512);
            push_mem(L256, Add+256);
             push_mem(L128, Add+128);
             push_mem(L64,Add+64);
             push_mem(L32,Add+32);
             push_mem(L16,Add+16);
             push_mem(L8,Add+8);
             end=Add+8-1;
             fprintf(out_file1, "At time %d,  Allocated %d,  for Process %d, from %d,  to %d\n",time ,p.memory_size ,p.id ,Add ,end );
            }



        }
    }



}
int main(int argc, char * argv[])
{
    //////////////////////////////INTIALIZATION of MEMORY QUEUES///////////////////
    L1024=create_pq_queue_mem();
    L512=create_pq_queue_mem();
    L256=create_pq_queue_mem();
    L128=create_pq_queue_mem();
    L64=create_pq_queue_mem();
    L32=create_pq_queue_mem();
    L16=create_pq_queue_mem();
    L8=create_pq_queue_mem();
    push_mem(L1024,0);
   /////////////////////////////////////
    totlal_runtime = 0;
    total_WTA = 0;
    total_waiting = 0;
    signal (SIGCHLD, handler_sigchild); // in STRN algo
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
    SRTN_Q= create_pq_queue();

    total_WTA = 0;
    total_waiting = 0;

    signal (SIGUSR1,handler_sigusr1);
    signal(SIGINT,handler_sigint);
    
    msgqid_sch = msgget(12613,  0644);
    algoritm_sent=argv[1];
    quantum_sent=argv[2];


    initClk();



    algorithm=atoi(algoritm_sent);
    quantum=atoi(quantum_sent);
    printf("\n the Algorithm chosen is :%d \n", algorithm);



    if (algorithm==1) //HPF
    {
       
        while(flag_all_rec!=1 || !isEmpty_pq(per_q_HPF))
        {
           
            int a = isEmpty_pq(per_q_HPF);
            
            if(isEmpty_pq(per_q_HPF)!=1)
            {
                
                p_inner = peek_pq(per_q_HPF);
                pop(per_q_HPF);
                int real_id = fork();
                if (real_id == -1)
                {
                    perror("error in fork");
                }
                else if (real_id == 0)
                {
                  
                    
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
                    printf("\nend 1 pro\n");
                    final_time = getClk();
                   
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
    
     
        while(!isEmpty_pq(SRTN_Q) || flag_all_rec!=1 )   // exit if queue is empty and flag_all_rec=1
        {
            receving_flag = 0;
           
            
            if(!isEmpty_pq(SRTN_Q))
            {
                 
                running_process=peek_pq(SRTN_Q);
                // RELATED TO OUTPUT FILES 
                if(running_process.remaining_time==running_process.runtime )
                {
                    
                out_log(1,getClk(),running_process.id,running_process.arrival,running_process.runtime,
                running_process.remaining_time,getClk()-running_process.arrival,0,0);
                totlal_runtime+=running_process.runtime;
                }
                ///////////////////////////////////////

                kill(running_process.forking_id, SIGCONT);
                printf("Process running noooow =%d \n",running_process.id);
                 if(running_process.remaining_time!=running_process.runtime ){
                   running_process.waiting_time += ((getClk() - running_process.arrival) - (running_process.runtime - running_process.remaining_time));
                     out_log(3,getClk(),running_process.id,running_process.arrival,running_process.runtime,running_process.remaining_time,running_process.waiting_time,0,0);
                 }

                int start=getClk();   //start time of the process
                pop(SRTN_Q);
                printf("Status %d and recieving_flag == %d \n",status,receving_flag);
                while(status == 0 && receving_flag == 0)
                {
                    pause();
                }
                status=0;
                if(!isEmpty_pq(SRTN_Q))
                {
                    ready_process=peek_pq(SRTN_Q);
                    int end=getClk();  //end time for this turn of the process
                    running_process.remaining_time-=(end-start);
                     int temp_finish = getClk();
                        running_process.waiting_time += ((temp_finish - running_process.arrival) - (running_process.runtime - running_process.remaining_time));
                       // printf("waiting time for the process with id %d time    = %d  \n",running_process.id,running_process.waiting_time);
                  
                    if(ready_process.runtime < running_process.remaining_time)
                    {
                        push(SRTN_Q,running_process,running_process.remaining_time);
                        printf("=====================Context Switching \n");
                        kill(running_process.forking_id, SIGSTOP);
                        
                         printf("Process Stoped noooow =%d \n",running_process.id);
                         out_log(2,getClk(),running_process.id,running_process.arrival,
                         running_process.runtime,running_process.remaining_time,running_process.waiting_time,0,0);
                       

                    }
                    else
                    {
                        if (running_process.remaining_time > 0)
                        {
                            push(SRTN_Q,running_process,running_process.remaining_time);
                            //printf("Process runnong has remaining time =%d\n",running_process.remaining_time);
                             kill(running_process.forking_id, SIGSTOP);
                        }
                        else{
                              printf("Process with id %d has remaining time  %d\n",running_process.id,running_process.remaining_time);
                            int turn_around=getClk()-running_process.arrival;
                            
                            out_log(4,getClk(),running_process.id,running_process.arrival,running_process.runtime,0,getClk()-running_process.arrival-running_process.runtime,turn_around,turn_around/running_process.runtime);
                            total_waiting+=getClk()-running_process.arrival;
                            total_WTA+=turn_around/running_process.runtime;
                        
                        }
                    }
                }
               
            }

        }
        int turn_around=getClk()-running_process.arrival;
        out_log(4,getClk(),running_process.id,running_process.arrival,running_process.runtime,0,getClk()-running_process.arrival-running_process.runtime,turn_around,turn_around/running_process.runtime);
        total_waiting+=getClk()-running_process.arrival;
        total_WTA+=turn_around/running_process.runtime;
        final_time=getClk();
        printf("After While \n");


    }
   handler_close_files();
    destroyClk(true);
    exit(0);
}


void receive(int msgqid_sch,struct msgbuff P_data)
{


    
    int rec_val;

    while(rec_val = msgrcv(msgqid_sch, &P_data, sizeof(P_data.pmsg),0, IPC_NOWAIT)!=-1)
    {
     if (algorithm==1) //HPF
        {
            if(P_data.pmsg.id!=-2) 
            {
            push(per_q_HPF,P_data.pmsg, P_data.pmsg.priority);
             flag_all_rec=0;
            }
            else{
                 flag_all_rec=1;
            }


        }
        else if(algorithm==2) //RR
        {
            enqueue(queue_RR,P_data.pmsg);
        }
        else if(algorithm==3) //STRN
        {
            if(P_data.pmsg.id!=-2)
            {
                process_rec.id=P_data.pmsg.id;
                process_rec.runtime=P_data.pmsg.runtime;
                process_rec.arrival=P_data.pmsg.arrival;
                process_rec.priority=P_data.pmsg.priority;
                process_rec.memory_size=P_data.pmsg.memory_size;
                process_rec.remaining_time=process_rec.runtime;
                 process_rec.waiting_time=0;
                flag_all_rec=0;
                receving_flag = 1;
                 Allocate(process_rec,out_file1,getClk());

            }
            else
            {
                flag_all_rec=1;
               
            }

        }
        ++num_of_processes;

    
    }
   

    if(rec_val == -1)
    {
        perror("Error in receving!");
        exit(-1);
    }
   
        


}

void handler_sigusr1(int signum)
{
    receive(msgqid_sch,P_data);

    if(algorithm==3) {

        execute_Process();
    }

    

    signal (SIGUSR1,handler_sigusr1);
}

void handler_sigint(int signum) {
    handler_close_files();
    destroyClk(1);
    exit(getpid());
    signal(SIGINT,handler_sigint);
}

void execute_Process()
{
    // printf(" P_DATA info run time at the top %d\n",process_rec.runtime);
    int pid_process= fork();
    if (pid_process == -1)
    {
        perror("error in fork");
    }


    else if (pid_process == 0)
    {


       // printf("\nI am the process!, my pid = %d and my parent's pid = %d\n\n", getpid(), getppid());

        char pp_id[10];
        char pp_arrival[10];
        char pp_runtime[10];
        char pp_priority[10];

        sprintf(pp_id, "%d", process_rec.id);
        sprintf(pp_arrival, "%d", process_rec.arrival);
        sprintf(pp_runtime, "%d",process_rec.runtime);
        sprintf(pp_priority, "%d",process_rec.priority);

        if(process_rec.id!=-2) {
            char *argv[] = { "process.out",pp_runtime,pp_id,pp_arrival,pp_priority,0 };

            int x=execve(argv[0], argv, NULL);

            if(x!=-1)
                printf("Error in execve\n");
        }
    }
    else
    {
        // printf("i am the scheduler\n");
        process_rec.forking_id=pid_process;
        // printf(" HELOOO   forking id == %d \n",process_rec.forking_id);
        push(SRTN_Q,process_rec,process_rec.runtime);
        kill(process_rec.forking_id,SIGSTOP);
        //printf("i am the scheduler i stoped process\n");
    }


}

void handler_sigchild (int signum)
{

    int pd;

    // printf("I have been called\n");
    pd = waitpid(-1, &status, WNOHANG);

  

    if(WIFEXITED(status) && pd!=0)
    {
        --num_of_processes;
        running_process.exit_time=getClk();
        printf("\nA PROCESS with id %d terminated with exit time %d\n", running_process.id, running_process.exit_time);
        printf("The status inside handler is %d \n",status);

    }
    
    signal(SIGCHLD,handler_sigchild);

}




////////////////JEEEEEE
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
        //fprintf(out_file1, "ouiiiiiiiiiiiiiiiiiiiiiii\n");
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
void handler_close_files()
{
    fclose (out_file1);
    out_pref();
    fclose (out_file2);
    //kill(getpid(),SIGINT);
    exit(0);
}