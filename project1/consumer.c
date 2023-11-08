//Consumer file
//Andrew Cash
//11/7/2023

#include "shared.h"
 
int main(){
    /* shared memory file descriptor */
    int shm_fd;
 
    /* pointer to shared memory object */
    struct Thing *sharedMem;
 
    /* open the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0600);//0666

    ftruncate(shm_fd, SIZE);
    /* memory map the shared memory object */
    //ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    //sleep(1);
    //printf("%i", ((int*)ptr)[0]);
    //printf("%i", ((int*)ptr)[1]);
    sharedMem = mmap(NULL, sizeof(*sharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //sleep(1);
    //sharedMem->test = 9;
    sleep(1);
    printf("%i", sharedMem->test);
    //WORKSSSSSSSSSS

    //wait for other to signal now
    //
    int limit = 5;
    do{
        fprintf(stderr, "Consumer about to wait...\n");
        sem_wait(&sharedMem->lock);
            //critical section
        fprintf(stderr, "Consumer critical...\n");
        printf("%i", sharedMem->test);
        printf("%s", "\n");
        sem_post(&sharedMem->lock);
        sharedMem->buff[0] = 1;
        fprintf(stderr, "Consumer signaled...\n");
        --limit;
        if(limit == 0) {shm_unlink(name); exit(0);};
    } while(limit > 0);


    //put semaphore into shared memory
    
    /* read from the shared memory object */
    //printf("%s", (char*)ptr);
 
    /* remove the shared memory object */
    //shm_unlink(name);
    shm_unlink(name);
    return 0;
}
