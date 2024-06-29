#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t p = fork();
    if (p < 0) {
        perror("fork fail");
        exit(1);
    }
    if (p == 0) 
        printf("Process id of the child process is %d\n", getpid());
    else 
        printf("Process id of the parent process is %d\n", getpid());
    return 0;
}
