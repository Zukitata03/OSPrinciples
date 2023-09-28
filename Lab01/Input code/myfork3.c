#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>  // Include for wait function


int value = 5;

int main() {
    pid_t pid;
    pid = fork();
    if (pid == 0) {  /* child process */
        value += 15;
        return 0;
    } else if (pid > 0) {  /* parent process */
        wait(NULL);  /* wait for the child process to terminate */
        printf("PARENT: value = %d\n", value);
        return 0;
    }
}
