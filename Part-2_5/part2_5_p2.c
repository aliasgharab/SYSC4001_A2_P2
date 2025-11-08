#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define SHM_KEY 0x1234
#define SEM_KEY 0x1235

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short * array;
};

void sem_wait(int sem_id) {
    struct sembuf op = {0, -1, 0};
    semop(sem_id, &op, 1);
}

void sem_signal(int sem_id) {
    struct sembuf op = {0, 1, 0};
    semop(sem_id, &op, 1);
}


int main()
{
    int shm_id;
    int *shared_memory;
    int *multiplier;
    int *counter;
    int cycle = 0;
    int SHM_MODE = 0666;

    int sem_id;
    int SEM_MODE = 0666;

    shm_id = (shmget(SHM_KEY, 2 * sizeof(int),  SHM_MODE));
    if (shm_id == -1) {  
        perror("shmget error");
    } 
    shared_memory = (int *)shmat (shm_id, (char *)0, 0 );
    if (shared_memory == (int *)-1) {
        perror("shmat error for shared memory");
    }

    sem_id = semget(SEM_KEY, 1, SEM_MODE);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }


    multiplier = &shared_memory[0];
    counter = &shared_memory[1];

    while (1) {
        sem_wait(sem_id);  // Protect the read
        int current = *counter;
        sem_signal(sem_id);
        
        if (current >= 100) {
            break;
        }
        sleep(1);
    }
    while (1) 
    {
        sem_wait(sem_id);
        
        // Check if we should stop (counter > 500)
        if (*counter > 500) {
            sem_signal(sem_id);
            break;
        }
        
        // Process the current counter value
        if (*counter % *multiplier == 0) 
            printf("P2 - Cycle: %d | %d is multiple of %d\n", cycle, *counter, *multiplier);
        else 
            printf("P2 - Cycle: %d | Counter: %d\n", cycle, *counter);
        
        (*counter)--;  // DECREMENT the counter
        cycle++;
        
        sem_signal(sem_id);
        sleep(1);
    }
    
    shmdt(shared_memory);
    return 0;
}