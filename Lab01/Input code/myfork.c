#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pid = fork();
    if (pid < 0) {
        printf("Cannot create the child process\n");
        exit(-1);
    }

    if (pid == 0) {
        printf("hello from the child\n");
    } else {
        printf("hello from the parent. Child's pid=%d\n", pid);
    }
      fork();   
      fork();  
      fork(); 

      return 0;

}