#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct process {
    pthread_t thread;
    struct process* next;
} process;

typedef struct {
    process* front;
    process* rear;
} Queue;

void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

int isEmpty(Queue* q) {
    return q->front == NULL;
}

void enqueue(Queue* q, pthread_t thread) {
    process* newProcess = (process*)malloc(sizeof(process));
    newProcess->thread = thread;
    newProcess->next = NULL;
    if (isEmpty(q)) {
        q->front = q->rear = newProcess;
    } else {
        q->rear->next = newProcess;
        q->rear = newProcess;
    }
}

pthread_t dequeue(Queue* q) {
    if (isEmpty(q)) {
        return 0;
    }
    process* temp = q->front;
    pthread_t thread = temp->thread;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return thread;
}

typedef struct semaphore {
    Queue q;
    int value;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} semaphore;

void initSemaphore(semaphore* s, int value) {
    initQueue(&s->q);
    s->value = value;
    pthread_mutex_init(&s->lock, NULL);
    pthread_cond_init(&s->cond, NULL);
}

void P(semaphore* s) {
    pthread_mutex_lock(&s->lock);
    if (s->value > 0) {
        s->value--;
    } else {
        pthread_t threadID = pthread_self();
        enqueue(&s->q, threadID);
        printf("Thread %lu is going to the waiting queue\n", (unsigned long)threadID);
        pthread_cond_wait(&s->cond, &s->lock);
    }
    pthread_mutex_unlock(&s->lock);
}

void V(semaphore* s) {
    pthread_mutex_lock(&s->lock);
    if (isEmpty(&s->q)) {
        s->value++;
    } else {
        pthread_t thread = dequeue(&s->q);
        printf("Thread %lu is awakening from the waiting queue\n", (unsigned long)thread);
        pthread_cond_signal(&s->cond);
    }
    pthread_mutex_unlock(&s->lock);
}

void* test_function(void* arg) {
    semaphore* s = (semaphore*)arg;
    P(s);
    printf("Thread %lu entered critical section\n", (unsigned long)pthread_self());
    sleep(2);
    printf("Thread %lu leaving critical section\n", (unsigned long)pthread_self());
    V(s);
    return NULL;
}

int main() {
    semaphore s;
    initSemaphore(&s, 3);  // Initialize semaphore with a value of 3

    pthread_t threads[10];
    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, test_function, &s);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
