//Producer file
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

    

    //Producer only:
    /////////////////////////////////////////////////////////////////////////////////////
    //Intializes the semaphore "lock" to 1
    //sem_wait() will make a process wait intil no process is in its critical section
    //Enforces mutual exclusion
    printf("%s", "-Initializing the semaphore-\n");
    sem_init(&sharedMem->lock, 1, 1);
    //Initializes integers "in" and "out" to be 0, meaning buffer is empty
    sharedMem->in = 0;
    sharedMem->out = 0;
    //Intializes buffer elements to be 0, meaning buffer is empty
    for(int i = 0; i < size; ++i){
        sharedMem->buff[i] = 0;
    }
    /////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Producer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    int limit = 4;                                                        //No need to go on forever.

    do{
        if (!((sharedMem->in + 1) % size == sharedMem->out)){             //if buffer is not full: Begin waiting. Will continue if the lock
            sem_wait(&sharedMem->lock);                                   // equals 1, meaning nothing is within its critical section

            //Critical Section//////////////////////////////
            printf("%s", "Producer is in its critical section:\n");
            sharedMem->buff[sharedMem->in] = 1 + (rand()%9);              //random number 1-9 to be pushed onto buffer
            printf("%s", "    Produced item: ");
            printf("%i", sharedMem->buff[sharedMem->in]);                 //Prints the item on buffer
            printf("%s", "\n    ");
            sharedMem->in = (1 + sharedMem->in) % size;                   //Sets "in" to the next index of the circular array
            bufferStatus(*sharedMem);                                     //Outputs the current items in the buffer
            printf("%s", "\n");
            printf("%s", "~~Producer signal, leaving critical section\n");
            sem_post(&sharedMem->lock);                                   //Semaphore signal. Increments lock, allowing others to enter
            ////////////////////////////////////////////////              //their critical sections
            --limit;
            //sleep(1);
        }
    } while(limit>0);
    
    return 0;
}
