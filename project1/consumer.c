//Consumer file
//Andrew Cash
//Operating Systems
//11/7/2023

#include "shared.h"
 
 //Consumer Function
void * consumer(void *sharedMe){
    struct sharedMemStruct *sharedMem = (struct sharedMemStruct*)sharedMe;                  //Para is a void pointer, for the function, needs to be a pointer to sharedMemStruct

    int limit = 2;                                                                          //No need to go on forever.

    do{
        
        usleep((rand() % 5 + 1) * 10000);                                                   //Randomly sleep for a few miliseconds
        
        sem_wait(&sharedMem->lock);   
        printf("%s", "-A consumer thread is about to check if buffer has an item- \n");
        if (bufferIsEmpty(sharedMem) == 0){                                                //if buffer is not empty: Begin waiting. Will continue if the lock
            //sem_wait(&sharedMem->lock);                                                  // equals 1, meaning nothing is within its critical section
            

            //Critical Section//////////////////////////////
            printf("%s", "Consumer is in its critical section:\n");
            printf("%s", "    Consumed item:    ");
            printf("%i", sharedMem->buff[sharedMem->out]);
            printf("%s", "\n    ");
            sharedMem->buff[sharedMem->out] = 0;                                          //int 0 denotes empty.
            sharedMem->out = (1 + sharedMem->out) % size;                                 //Sets "out" to the next index of the circular array

            bufferStatus(*sharedMem);                                                    //Outputs the current items in the buffer
            printf("%s", "\n");
            printf("%s", "~~Consumer signal, leaving critical section\n");                                              
            ////////////////////////////////////////////////                            
            --limit;
        }

        sem_post(&sharedMem->lock);                                                     //Semaphore signal. Increments lock, allowing others to enter their critical sections

        

    } while(limit > 0);

    pthread_exit(0);

}


int main(){
    
    setbuf(stdout, NULL); //Issues with output consistency. Disables the buffer on stdout.
    srand(time(NULL));

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



 //Consumer Threads
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Create threads
    pthread_t thread3;
    pthread_t thread4;

    pthread_create(&thread3, NULL, consumer, (void*) sharedMem);
    pthread_create(&thread4, NULL, consumer, (void*) sharedMem);


    //Joins threads: will wait until threads have finished execution
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    

    return 0;
}
