# OS-Project-1: The Producer-Consumer Problem

Andrew Cash - 11/7/2023

## Introduction

The Producer-Consumer Problem is a standard problem regarding synchronizing cooperating processes that have shared memory. The concept is simple: one process produces data and places it onto a buffer, and another process consumes it, removing it from the buffer. The issue arises with concurrent access to shared data, and possible data inconsistencies that stem from it. An abstract solution to this is the introduction of mutually exclusive critical sections. Critical sections are sections of code where sensitive shared data can be access. By making them mutually exclusive, processes that share memory can execute concurrently without issue.

## Compiling and Running
Use these in a Linux environment to compile and run this project:

$ gcc producer.c -pthread -lrt -o producer

$ gcc consumer.c -pthread -lrt -o consumer

$ ./producer & ./consumer &

## Implementation of Producer-Consumer Problem

### Implmenting Shared Memory

Shared memory was implemented using the Linux Real-Time Library. The functions used in this project and light descriptions of them are as follows:

- smh_open()
        - Creates a shared memory object. Returns an integer which acts as a file description for the shared memory.
- ftruncate()
        - Sets the size of the shared memory. Arguments are the file desciption integer and the number of bits to set the size to as an integer.
- mmap()
        - "Maps" the shared memory object. Allows you to assign a pointer of any type to the shared memory, which sets the structure of the shared memory, and allows it to be accessed through that pointer.

### Implementing Mutual Exclusion

Mutual exclusion was implemented using semaphores. Semaphores are stored in shared memory. The functions used in this project and light descriptions of them are as follows:

- sem_init()
        - Initializes a varible of type sem_t.
- sem_wait()
        - Used at the beginning of a critical section. Takes a semaphore as an argument. Decrements the semaphore, then if it is not 0, it waits until it is set to 0 by another process.
- sem_post()
        - Signals the end of a critical section. Takes a semaphore as an argument. Simply increments the semaphore.
