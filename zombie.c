#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t p = fork();
    if (p > 0) {
        printf("Parent process PID: %d, sleeping...\n", getpid());
        sleep(10); // Reduced sleep duration for quick observation
        printf("Parent process PID: %d, awake and terminating...\n", getpid());
    } else if (p == 0) {
        printf("Child process PID: %d, terminating...\n", getpid());
        exit(0);
    } else {
        perror("Fork failed");
        exit(1);
    }
    return 0;
}
