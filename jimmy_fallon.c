#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
//counting sem for the 3 operators 
sem_t operators;
//mutex for the callers that are connected
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
            //lock it with incrementing 
            connected++;
            sem_post(&connected_lock);
            printf("Thread %d has available line,call ringing\n",id);
            //open operators 
            sem_wait(&operators);
            printf("Thread %d is talking to the operator\n",id);
            //simulates that they talk
            sleep(2);
            printf("Thread %d has got a ticket\n", id);
            //closes the operator sem
            sem_post(&operators);
            //increment the connected lock 
            sem_wait(&connected_lock);
            connected--;
            sem_post(&connected_lock);
            //lock the connected until it goes again
            printf("Thread %d has hung up\n", id);
            break;
        }

         else{
            //lock it without incrementing it 
            sem_post(&connected_lock);
            printf("The lines are busy right now!\n"); 
            sleep(2);  
        }
        

    }

}
   

int main(int argc, char* argv[]){
    /* intialized the sems, thread, and destroys them*/
    sem_init(&connected_lock, 0, 1);
    //initialize the 3 operators
    sem_init(&operators,0,3);
    //number of callers 
    int numCallers;
    next_id = 1;
    if(argc == 1){
        printf("You haven't put in anything!");
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


    return 0;




}

