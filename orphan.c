#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() 
{ 
    int pid = fork(); 

    if (pid > 0) {
        printf("Parent process (PID: %d)\n", getpid());
        sleep(5); 
        printf("Parent process (PID: %d) is terminating\n", getpid());
    } 
    else if (pid == 0) { 
        printf("Child process (PID: %d, Parent PID: %d) is running\n", getpid(), getppid());
        sleep(10);
        printf("Child process (PID: %d) after sleep, Parent PID: %d\n", getpid(), getppid());
        printf("Child process (PID: %d) is terminating\n", getpid());
        exit(0);
    } 
    else {
        perror("Fork failed");
        exit(1);
    }

    return 0; 
}
