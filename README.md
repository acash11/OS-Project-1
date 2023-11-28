# OS-Project-1: The Producer-Consumer Problem

Andrew Cash - 11/7/2023

## Introduction

The Producer-Consumer Problem is a standard problem regarding synchronizing cooperating processes that have shared memory. The concept is simple: one process produces data and places it onto a buffer, and another process consumes it, removing it from the buffer. The issue arises with concurrent access to shared data, and possible data inconsistencies that stem from it. An abstract solution to this is the introduction of mutually exclusive critical sections. Critical sections are sections of code where sensitive shared data can be accessed. By making them mutually exclusive, processes that share memory can execute concurrently without issue.

## Compiling and Running
Use these in a Linux environment to compile and run this project:

$ gcc producer.c -pthread -lrt -o producer

$ gcc consumer.c -pthread -lrt -o consumer

$ ./producer & ./consumer &

## Implementation of Producer-Consumer Problem

### Implementing Shared Memory

Shared memory was implemented using the Linux Real-Time Library. The functions used in this project and light descriptions of them are as follows:

- smh_open()
        - Creates a shared memory object. Returns an integer which acts as a file description for the shared memory.
- ftruncate()
        - Sets the size of the shared memory. Arguments are the file description integer and the number of bits to set the size to as an integer.
- mmap()
        - "Maps" the shared memory object. Allows you to assign a pointer of any type to the shared memory, which sets the structure of the shared memory, and allows it to be accessed through that pointer.
  In the project:

```c
//Setting up shared memory
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Pointer to an instance of sharedMemStruct
    struct sharedMemStruct *sharedMem;
 
    //Open shared memory. shm_fd will be integer that describes the shared memory file
    int shm_fd;
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0600);
 
    //Configure the size of shared memory. Uses file description, and amount of bits as an integer
    ftruncate(shm_fd, sizeof(struct sharedMemStruct));
 
    //Asssign the shared memory to the pointer "sharedMem"
    //Both the producer and consumer will have a "sharedMem". They will point to the same memory location
    sharedMem = mmap(NULL, sizeof(*sharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
```

### Implementing Mutual Exclusion

Mutual exclusion was implemented using semaphores. Semaphores are stored in shared memory. The functions used in this project and light descriptions of them are as follows:

- sem_init()
        - Initializes a variable of type sem_t.
- sem_wait()
        - Used at the beginning of a critical section. Takes a semaphore as an argument. Decrements the semaphore, then if it is not 0, it waits until it is set to 0 by another process.
- sem_post()
        - Signals the end of a critical section. Takes a semaphore as an argument. Simply increments the semaphore.

  In the project:

```c
    //Intializes the semaphore "lock" to 1
    sem_init(&sharedMem->lock, 1, 1);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////
    sem_wait(&sharedMem->lock);                                                         //Only one at a time can check if its full/empty
    ////////////////////////////////////////
        printf("%s", "-A producer thread is about to check if buffer is not full- \n");
        if ((bufferIsFull(sharedMem) == 0)){                                            //if buffer is not full: Begin waiting. Will continue if the lock equals 1, meaning nothing is within its critical section

            //Critical Section//////////////////////////////
            printf("%s", "Producer is in its critical section:\n");
            sharedMem->buff[sharedMem->in] = 1 + (rand()%9);                            //random number 1-9 to be pushed onto buffer
            printf("%s", "    Produced item:    ");
            printf("%i", sharedMem->buff[sharedMem->in]);                               //Prints the item on buffer
            printf("%s", "\n    ");
            sharedMem->in = (1 + sharedMem->in) % size;                                 //Sets "in" to the next index of the circular array

            bufferStatus(*sharedMem);                                                   //Outputs the current items in the buffer
            printf("%s", "\n");
            printf("%s", "~~Producer signal, leaving critical section\n");
                                              
            ////////////////////////////////////////////////
            --limit;
        }
        ////////////////////////////////////////
        sem_post(&sharedMem->lock);
        ////////////////////////////////////////
```

### Further Details of Implementation

The buffer has a size of 3, which allows it to hold 2 items; All that is needed for a simple demonstration. It is designed as a circular array, which helps to follow the production and consumption of items. The shared memory has two integers named "in" and "out", these correlate to where the producer will produce, and where the consumer will consume, respectively. These move across the buffer index range (0-2) as needed. Here, the items are simply going to be a randomly generated integer from 1-9. 0 will represent an empty space in the buffer, which means that when the consumer consumes, it replaces the item with 0.

Apart from that, print statements are made to help follow which process enters its critical section, what it does, and what the buffer looks like afterwards.

## Example Output

A text file is attached showing an example of output from the program.
