//Producer file
//Andrew Cash
//11/7/2023

#include "shared.h"

int main(){
    /* shared memory file descriptor */
    int shm_fd;
 
    /* pointer to shared memory object */
    struct Thing *sharedMem;
 
    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0600);//0666
 
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);
 
    /* memory map the shared memory object */
    sharedMem = mmap(NULL, sizeof(*sharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //Thing = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    //WORKSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS^^
    sharedMem->test = 9;
    //printf("%i", sharedMem->test);
    //WORKSSSSSSS
    fprintf(stderr, "Initializing...");
    sem_init(&sharedMem->lock, 1, 1);



///////////////////////////////////////////////////////////////////

    sharedMem->in = 0;
    sharedMem->out = 0;

    //sharedMem->buff[0] = 1;
    //Put semaphore into shared memory
    int limit = 10;
    do{
        //if (sharedMem->buff[0] == 1){
        //fprintf(stderr, "Producer about to wait...\n");
        if (!((sharedMem->in + 1) % bufSize == sharedMem->out)){//if not full ;
            sem_wait(&sharedMem->lock);
            printf("%s", "Producer critical: ");
            sharedMem->buff[sharedMem->in] = 1 + (rand()%9); //random number 1-9
            printf("%s", "Produced item: ");
            printf("%i", sharedMem->buff[sharedMem->in]);

            printf("%s", "\n");
            fprintf(stderr, "Producer signaled...\n");
            sharedMem->in = (1 + sharedMem->in) % bufSize;
            //sharedMem->buff[0] = 0;
            sem_post(&sharedMem->lock);
            
            --limit;
            if(limit == 0) {shm_unlink(name); exit(0);};
        }
    } while(limit>0);
    //wait for consumer to signal

    //sem_t lock;
    //ptr[2] = lock;
    //sem_init(&lock, 0, 1);









    //sem_wait(&lock);
    //sem_post(&lock);
    //((int*)ptr)[0] = 123;
    //((int*)ptr)[1] = 4321;
    /* write to the shared memory object */
    //sprintf(ptr, "%s", message_0);
 
    //ptr += strlen(message_0);
    //sprintf(ptr, "%s", message_1);
    //ptr += strlen(message_1);
    
    return 0;
}
