#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SHM_KEY 0x1234

int main()
{
    int shm_id;
    int *shared_memory;
    int *multiplier;
    int *counter;
    int cycle = 0;
    int SHM_MODE = 0666;

    shm_id = (shmget(SHM_KEY, 2 * sizeof(int),  SHM_MODE));
    if (shm_id == -1) {  
        perror("shmget error");
    } 
    shared_memory = (int *)shmat (shm_id, (char *)0, 0 );
    if (shared_memory == (int *)-1) {
        perror("shmat error for shared memory");
    }

    multiplier = &shared_memory[0];
    counter = &shared_memory[1];

    while (*counter <= 100) sleep(1);
    while (*counter <= 500)
    {
        if (*counter % *multiplier == 0) printf("P2 - Cycle number: %d | %d is a multiple of %d \n", cycle, *counter, *multiplier);
        else printf("P2 - Cycle number: %d | %d \n", cycle, *counter);
        (*counter)--;
        cycle++;
        sleep(1);
    }

    shmdt(shared_memory);
    return 0;
}