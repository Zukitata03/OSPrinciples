#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    char *msg = "Hello child\n";
    char buf[255];
    int fd[2];
    int pid;

    if (pipe(fd) < 0) {
        perror("pipe");
        exit(-1);
    }

    if ((pid = fork()) > 0) { // parent
        close(fd[0]);
        write(fd[1], msg, strlen(msg));
        wait(NULL); // wait for the child to exit
    } else { // child
        close(fd[1]);
        memset(buf, 0, 255);
        read(fd[0], buf, 254);
        printf("%s\n", buf);
    }

    return 0;
}