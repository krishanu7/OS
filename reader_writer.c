#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

sem_t mutex;
sem_t wrt; 
int readcnt = 0;

void* writer(void* arg) {
    while (1) {
        printf("Writer %lu is trying to write\n", (unsigned long)pthread_self());
        sem_wait(&wrt); // Request access to critical section

        // Critical section: writing
        printf("Writer %lu is writing\n", (unsigned long)pthread_self());
        printf("Writer %lu has finished writing\n", (unsigned long)pthread_self());

        sem_post(&wrt); // Release access to critical section

        // Simulate doing something else
        sleep(1);
    }
}

void* reader(void* arg) {
    while (1) {
        printf("Reader %lu is trying to read\n", (unsigned long)pthread_self());
        sem_wait(&mutex); // Request access to update readcnt

        readcnt++;
        if (readcnt == 1) {
            sem_wait(&wrt); // First reader locks the writer out
        }

        sem_post(&mutex); // Release access to update readcnt

        // Critical section: reading
        printf("Reader %lu is reading\n", (unsigned long)pthread_self());
        printf("Reader %lu has finished reading\n", (unsigned long)pthread_self());

        sem_wait(&mutex); // Request access to update readcnt

        readcnt--;
        if (readcnt == 0) {
            sem_post(&wrt); // Last reader unlocks the writer
        }

        sem_post(&mutex); // Release access to update readcnt

        // Simulate doing something else
        sleep(1);
    }
}

int main() {
    pthread_t readers[5], writers[2];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    // Create writer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    // Create reader threads
    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
    }

    // Join writer threads
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    // Join reader threads
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
