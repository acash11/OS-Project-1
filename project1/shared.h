#ifndef SHARED_H
#define SHARED_H

//Andrew Cash
//Operating Systems Fall 2023
//11/7/2023
//This header file contains information regarding the shared files
//for the producer-consumer problem project

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

//size of the buffer
#define size 3
 
//name of shared memory for setting up shared memory
const char* name = "SM";

//Structure for shared memory
struct sharedMemStruct{
    sem_t lock;
    int in;
    int out;
    int buff[size];
};

//Function that outputs the current status of the buffer
void bufferStatus(struct sharedMemStruct ex){
    printf("%s", " | ");
    for(int i = 0; i < size; ++i){
        if(ex.buff[i] != 0){
            printf("%i", ex.buff[i]);
        }
        printf("%s", " | ");
    }
}

int bufferIsFull(struct sharedMemStruct *sharedMem){
    if (((sharedMem->in + 1) % size == sharedMem->out)){ //buffer is full
        //sem_post(&sharedMem->fullEmptyLock);
        return 1;
    }
    //sem_post(&sharedMem->fullEmptyLock);
    return 0;

}

int bufferIsEmpty(struct sharedMemStruct *sharedMem){
    if ((sharedMem->in == sharedMem->out)){ //buffer is empty
        //sem_post(&sharedMem->fullEmptyLock);
        return 1;
    }
    //sem_post(&sharedMem->fullEmptyLock);
    return 0;

}

#endif