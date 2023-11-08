#ifndef SHARED_H
#define SHARED_H

//Andrew Cash
//Operating Systems Fall 2023
//11/7/2023
//This header files contains information regarding the shared files
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

#define size 3

/* the size (in bytes) of shared memory object */
const int SIZE = 4096;

//size of the buffer for producer consumer problem
const int bufSize = 3;
 
/* name of the shared memory object */
const char* name = "OS";

/* shared memory object */
struct Thing{
    sem_t lock;
    int test;
    int in;
    int out;
    int buff[size];
};

#endif