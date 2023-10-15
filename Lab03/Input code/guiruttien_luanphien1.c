#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY sleep(rand() % 4)

int account; // tai khoan ngan hang
int turn = 0;

void *chaGuiTien() {  
    int x, i = 0;
    while (1) {
        if (i >= 5) {
            pthread_exit(NULL);
        }
        while (turn != 0);
        x = account; DELAY;
        x = x + 2; DELAY;
        account = x; DELAY;
        turn = 1;
        printf("Tao da gui. Account=%d\n", account);
        i++;
    }
}

void *conRutTien() {
    int y, i = 0;
    while (1) {
        if (i >= 5) {
            pthread_exit(NULL);
        }
        while (turn != 1);
        y = account; DELAY;
        y = y - 3; DELAY;
        if (y >= 0) { 
            account = y; 
            printf("Con da rut. Account=%d\n", account);
        }
        else { 
            printf("Khong du tien\n"); 
        }
        turn = 0;
        i++;
    }
}

int main() {
    pthread_t tid1, tid2;
    account = 5;
    printf("Account=%d\n", account);
    
    pthread_create(&tid1, NULL, chaGuiTien, NULL);   
    pthread_create(&tid2, NULL, conRutTien, NULL);   
    
    pthread_join(tid1, NULL);  
    pthread_join(tid2, NULL);
    
    return 0;
}