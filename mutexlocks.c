#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int NUM_READERS=5;
int NUM_WRITERS=2;
#define MAX_NUM_READS 10
/********************************************************************
* Program: Readers-Writers Problem
* Description: This program demonstrates the Readers-Writers Problem
*              solution using mutex locks in C programming language.
* Author: 21k4656, 21k4654, 21k3413
********************************************************************/

pthread_mutex_t mutex_lock;
pthread_mutex_t writers_lock;
int shared_variable = 0;
int num_readers = 0;
int num_writers = 0;
  
void *reader_func(void *arg) {
    int id = *(int *)arg;
    int num_reads = 0;

    while (num_reads < MAX_NUM_READS) {
        // Lock mutex for readers
        pthread_mutex_lock(&mutex_lock);
        num_readers++;
	syscall(335,pthread_self());
        if (num_readers == 1) {
            pthread_mutex_lock(&writers_lock); // Lock mutex for writers
        }
        pthread_mutex_unlock(&mutex_lock);

        // Read shared variable
        printf("Hello ! I am Reader %d\nI will read the value as: %d\n", id, shared_variable);

        // Unlock mutex for readers
        pthread_mutex_lock(&mutex_lock);
        num_readers--;
	syscall(335,pthread_self());
        if (num_readers == 0) {
            pthread_mutex_unlock(&writers_lock); // Unlock mutex for writers
        }
        pthread_mutex_unlock(&mutex_lock);

        num_reads++;
    }

    free(arg);
    pthread_exit(NULL);
}

void *writer_func(void *arg) {
    int id = *(int *)arg;
    int i;

    for (i = 0; i < MAX_NUM_READS; i++) {
        // Lock mutex for writers
        pthread_mutex_lock(&writers_lock);
	syscall(335,pthread_self());
        while(num_readers > 0 || num_writers > 0) { // Wait while readers or writers are holding the mutex
            pthread_mutex_unlock(&writers_lock);
            usleep(100);
            pthread_mutex_lock(&writers_lock);
        }

        num_writers++;

        // Write to shared variable
        shared_variable++;
        printf("Hello I am Writer %d\nAnd I wrote the value as: %d\n", id, shared_variable);

        num_writers--;
        // Unlock mutex for writers
        pthread_mutex_unlock(&writers_lock);
    }

    free(arg);
    pthread_exit(NULL);
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int i, rc;
    printf("\033[1;35m--------------------------------------------------------\n");
    printf("       READERS-WRITERS PROBLEM Using System Call \n");
    printf("--------------------------------------------------------\033[0m\n\n");
  printf("       Enter the number of readers: \n");
    scanf("%d",&NUM_READERS);	
	if(NUM_READERS<0)
	{
printf("       Wrong value! \n");
	 exit(1);
	}
    printf("       Enter the number of writers: \n");
    scanf("%d",&NUM_WRITERS);
	if(NUM_WRITERS<0)
	{
printf("       Wrong value! \n");
	 exit(1);
	}
    pthread_mutex_init(&mutex_lock, NULL);
    pthread_mutex_init(&writers_lock, NULL);
    // Create reader threads
    for (i = 0; i < NUM_READERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        rc = pthread_create(&readers[i], NULL, reader_func, id);
        if (rc) {
            fprintf(stderr, "Error creating reader thread %d\n", i+1);
            exit(-1);
        }
    }

    // Create writer threads
    for (i = 0; i < NUM_WRITERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        rc = pthread_create(&writers[i], NULL, writer_func, id);
        if (rc) {
            fprintf(stderr, "Error creating writer thread %d\n", i+1);
            exit(-1);
        }
    }

    // Wait for threads to finish
    for (i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&mutex_lock);
    pthread_mutex_destroy(&writers_lock);
    return 0;
}
