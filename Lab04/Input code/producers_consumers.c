//producer consumer using concurrent threads

#include<stdio.h>    
#include <stdlib.h>
#include <unistd.h>         
#include<pthread.h>
#include<semaphore.h>

#define N 5
#define R 4
#define TRUE 1
#define up(sem) sem_post(sem)
#define down(sem) sem_wait(sem)

void* producer(void* arg);
void* consumer(void* arg);
void produce_item(int* pItem);
void enter_item(int item);
int get_item();
void consume_item(int item);

sem_t full, empty, mutex;
int queue[N];
int inIdx, outIdx, count;

int main() {
    pthread_t pid, cid;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
}

void* producer(void* arg) {
    int item;
    printf("P: Hello\n");
    while (TRUE) {
        produce_item(&item);
        down(&empty);
        down(&mutex);
        enter_item(item);
        up(&mutex);
        up(&full);
        sleep(rand() % R);
    }
    return NULL;
}

void* consumer(void* arg) {
    int item;
    printf("C: Hello\n");
    while (TRUE) {
        down(&full);
        down(&mutex);
        item = get_item();
        up(&mutex);
        up(&empty);
        consume_item(item);
        sleep(rand() % R);
    }
    return NULL;
}

void produce_item(int* pItem) {
    (*pItem) = rand() % 100; // generate a random integer up to 99
    printf("P: Produce item %d\n", *pItem);
}

void enter_item(int item) {
    queue[inIdx] = item;
    inIdx = (inIdx + 1) % N;
    count++;
    printf("P: Enter item %d\n", item);
}

int get_item() {
	int item;
	item=queue[outIdx];
	outIdx = (outIdx+1) % N;
	count--;
	printf("C: Get item %d\n", item);
	if (count==0) printf("C: The queue is empty\n");

	return item;
}

void consume_item(int item) {
	printf("C: Item %d is yum yum!\n", item);
}