#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define NUM_THREADS 5
#define NUM_INCREMENTS 10000

int count = 0;

void* increment_count(void* arg){
    for(int i=0; i<NUM_INCREMENTS; i++){
        int temp = count;
        temp = temp + 1;
        count = temp;
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NUM_THREADS];
    //Creation
    for(int i=0; i<NUM_THREADS; i++){
        if(pthread_create(&threads[i],NULL,increment_count,NULL)!=0){
            perror("Failed to create thread");
            exit(1);
        }
    }
    //Wait for threads for finish
    for(int i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i],NULL);
    }
    printf("Final counter: %d\n", count);
    return 0;
}