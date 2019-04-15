#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t lock;

int main(){
    sem_init(&lock, 0, 1);
    sem_init(&lock);
    sem_init()
    pthread_t tid1, tid2;
    pthread_create(&tid1,NULL,phonecall,NULL);
    pthread_create(&tid2, NULL, phonecall,NULL);



}

void* phonecall(void* vargs){
    static int NUM_OPERATORS = 3;
    static int NUM_LINES = 5;
    static int connected = 0; 
    static sem_t connected_lock;
    static sem_t operators;

}