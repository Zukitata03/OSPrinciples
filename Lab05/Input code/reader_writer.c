#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define up(sem) sem_post(sem)
#define down(sem) sem_wait(sem)
#define R 2
#define DELAY sleep(rand() % R);

sem_t readCountAccess;
sem_t databaseAccess;
int readCount = 0;

void *reader(void *arg);
void *writer(void *arg);
void readData(int i);
void writeData(int i);

int main() {
    unsigned int i, noReaderThreads, noWriterThreads;
    pthread_t readerThread[100], writerThread[100];

    sem_init(&readCountAccess, 0, 1);
    sem_init(&databaseAccess, 0, 1);

    noReaderThreads = 10;
    assert(noReaderThreads < 100);
    noWriterThreads = 3;
    assert(noWriterThreads < 100);

    for (i = 0; i < noWriterThreads; i++) {
        pthread_create(&writerThread[i], NULL, writer, (void *)(intptr_t)i);
    }

    for (i = 0; i < noReaderThreads; i++) {
        pthread_create(&readerThread[i], NULL, reader, (void *)(intptr_t)i);
    }

    for (i = 0; i < noWriterThreads; i++) {
        pthread_join(writerThread[i], NULL);
    }

    for (i = 0; i < noReaderThreads; i++) {
        pthread_join(readerThread[i], NULL);
    }

    sem_destroy(&databaseAccess);
    sem_destroy(&readCountAccess);
    return 0;
}

void *writer(void *arg) {
    int id = (int)(intptr_t)arg;

    DELAY;

    // 1. try to get permission to access the database
    down(&databaseAccess);

    // 2. write data
    writeData(id);

    // 3. restore access permission
    up(&databaseAccess);
}

void *reader(void *arg) {
    int id = (int)(intptr_t)arg;

    DELAY;

    // 1. try to get permission to readCount variable
    down(&readCountAccess);
    if (readCount == 0) {
        down(&databaseAccess);
    }
    up(&readCountAccess);

    // 2. increase the number of Readers by 1
    readCount++;

    // 3. for the 1st reader:
    if (readCount == 1) {
        down(&databaseAccess);
    }

    // 4. restore access permission to readCount variable
    up(&readCountAccess);

    // Read the database
    readData(id);

    // 5. try to get permission to readCount variable
    down(&readCountAccess);

    // 6. decrease the number of readers by 1 as the reading is done
    readCount--;

    // 7. for the last reader:
    if (readCount == 0) {
        printf("Reader %d unlocks the database\n", id);
        up(&databaseAccess);
    }

    // 8. restore access permission to readCount variable
    up(&readCountAccess);
}

void readData(int i) {
    printf("Reader %d is reading\n", i);
    DELAY;
    printf("Reader %d is done with the reading\n", i);
}

void writeData(int i) {
    printf("Writer %d is writing\n", i);
    DELAY;
    printf("Writer %d is done with the writing\n", i);
}
