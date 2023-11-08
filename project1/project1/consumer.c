//Consumer file
//Andrew Cash
//Operating Systems
//11/7/2023

#include "shared.h"
 
int main(){

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

//Producer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int limit = 3;                                                        //No need to go on forever.

    do{
        //sleep(0.2);
        if (sharedMem->in != sharedMem->out){                             //if buffer is not empty: Begin waiting. Will continue if the lock
            sem_wait(&sharedMem->lock);                                   // equals 1, meaning nothing is within its critical section

            //Critical Section//////////////////////////////
            printf("%s", "Consumer is in its critical section:\n");
            printf("%s", "    Consumed item: ");
            printf("%i", sharedMem->buff[sharedMem->out]);
            printf("%s", "\n    ");
            sharedMem->buff[sharedMem->out] = 0;                          //0 denotes empty.
            sharedMem->out = (1 + sharedMem->out) % size;                 //Sets "out" to the next index of the circular array
            bufferStatus(*sharedMem);                                     //Outputs the current items in the buffer
            printf("%s", "\n");
            printf("%s", "~~Consumer signal, leaving critical section\n");
            sem_post(&sharedMem->lock);                                   //Semaphore signal. Increments lock, allowing others to enter
            ////////////////////////////////////////////////              //their critical sections
            --limit;
        }
    } while(limit > 0);

    return 0;
}
