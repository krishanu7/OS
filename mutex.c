#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 5
#define NUM_INCREMENTS 10000

int count = 0; // shared variable | critical section
pthread_mutex_t mutex; // Mutex variable

void* increment_count(void* arg){
    for(int i=0; i<NUM_INCREMENTS; i++){
        pthread_mutex_lock(&mutex); // Lock the mutex
        count++;
        pthread_mutex_unlock(&mutex); // Unlock the mutex
    }
    pthread_exit(NULL);
}

int main(){ 
    pthread_t threads[NUM_THREADS];
    // Before creating threads mutex must be initialized
    if(pthread_mutex_init(&mutex,NULL) != 0){
        perror("Mutex initalization failed");
        exit(1);
    }
    //Creating thread
    for(int i=0; i<NUM_THREADS; i++){
       if (pthread_create(&threads[i],NULL,increment_count,NULL)!=0){
         perror("Failed to create thread");
         exit(1);
       }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Final Counter: %d\n", count);
    //Destroy the mutex
    pthread_mutex_destroy(&mutex);
    return 0;
}