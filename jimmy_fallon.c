#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t operators;
sem_t callers;
sem_t connected_lock;
int next_id;

void* phonecall(void* vargs){
    int id;
    id = next_id;
    next_id++;
    static int NUM_OPERATORS = 3;
    static int NUM_LINES = 5;
    static int connected = 0; 
    //tells how many callers are currently connected
    while(1){
        printf("Thread %d attempting to call!\n", id);
        //opening the connected sem that will be checked in the critical section
        sem_wait(&connected_lock);
        
        if(connected != NUM_LINES){ 
            connected++;
            sem_post(&connected_lock);
            printf("Thread %d has available line,call ringing\n",id);
            sem_wait(&operators);
            printf("Thread %d is talking to the operator\n",id);
            sleep(2);
            printf("Thread %d has got a ticket\n", id);
            sem_post(&operators);
            sem_wait(&connected_lock);
            connected--;
            sem_post(&connected_lock);
            printf("Thread %d has hung up\n", id);
            break;
        }
        else{
            //lock it
            sem_post(&connected_lock);
            printf("The lines are busy right now!\n"); 
            sleep(2);  
        }

    }
   
   }

int main(int argc, char* argv[]){
     sem_init(&connected_lock, 0, 1);
    //initialize the 3 operators
    sem_init(&operators,0,3);
    sem_init(&callers, 0, 5);
    int numCallers;
    next_id = 1;
    if(argc == 1){
        numCallers = 10;
    }
    else{
        numCallers = atoi(argv[1]);
    }
    
   
    pthread_t tid[numCallers];
    for(int i = 0; i < numCallers; i++){
        int num1 = pthread_create(&tid[i],NULL,phonecall,NULL);
        if(num1 != 0){
            EXIT_FAILURE;
        }
    }
   
   for(int j = 0; j < numCallers; j++){
      int num2 = pthread_join(tid[j],NULL);
      if(num2 != 0){
          EXIT_FAILURE;
      }
   }
    sem_destroy(&connected_lock);
    sem_destroy(&operators);
    sem_destroy(&callers);


    return 0;




}

