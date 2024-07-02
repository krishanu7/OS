#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define N 2 // Number of threads (producer and consumer)
#define MAXBUFFER 3
bool flag[N] = {false, false};
int turn = 0;   // Variable to indicate turn
int buffer = 0; // Shared buffer

void *producer(void *arg)
{
    int j = (int)(long)arg;
    do
    {
        flag[j] = true; // Producer j is ready to produce
        turn = 1 - j;   // Allow consumer to consume
        while (flag[1 - j] && turn == 1 - j)
        {
            // Wait for consumer to finish
        }

        // Critical Section
        if (buffer < MAXBUFFER)
        {
            printf("Producer %d: Producing an item...\n", j);
            buffer++;
            printf("Buffer: %d\n", buffer);
        }else
        {
            printf("Producer %d: Buffer is full, cannot producer.\n", j);
        }
        flag[j] = false; // Producer is out of the critical section

        sleep(3);
    } while (true); // Continue indefinitely

    return NULL;
}

void *consumer(void *arg)
{
    int i = (int)(long)arg; 
    do
    {
        flag[i] = true; // Consumer i is ready to consume
        turn = 1 - i;   // Allow producer to produce
        while (flag[1 - i] && turn == 1 - i)
        {
            // Wait for producer to finish
            // Consumer waits if producer is ready and it's producer's turn
        }

        // Critical Section: Consumer consumes an item from the buffer
        if (buffer > 0)
        {
            printf("Consumer %d: Consuming an item...\n", i);
            buffer--;
            printf("Buffer: %d\n", buffer);
        }
        else
        {
            printf("Consumer %d: Buffer is empty, cannot consume.\n", i);
        }

        flag[i] = false; // Consumer is out of the critical section

        // Simulate some work
        sleep(3);
    } while (true); // Continue indefinitely

    return NULL; // Return NULL because this function does not return a result
}

int main()
{
    pthread_t producerThread, consumerThread;

    // Create producer and consumer threads
    pthread_create(&producerThread, NULL, producer, (void *)0);
    pthread_create(&consumerThread, NULL, consumer, (void *)1);

    // Wait for producer and consumer threads to finish
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    return 0;
}
