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
    int SHM_MODE = 0666;
    
    int sem_id;
    int SEM_MODE = 0666;


    shm_id = shmget(SHM_KEY, 2 * sizeof(int),  IPC_CREAT | SHM_MODE);
    if (shm_id == -1) {  
        perror("shmget error");
    } 
    shared_memory = (int *)shmat (shm_id, (char *)0, 0);
    if (shared_memory == (int *)-1) {
        perror("shmat error for shared memory");
    }

    sem_id = semget(SEM_KEY, 1, IPC_CREAT | SEM_MODE);
    if (sem_id == -1) {
        perror("semget failed");
    }

    union semun init_val;
    init_val.val = 1;
    semctl(sem_id, 0, SETVAL, init_val);

    multiplier = &shared_memory[0];
    counter = &shared_memory[1];

    *multiplier = 3;
    *counter = 0;

    pid_t pid1;
    pid1 = fork(); //To create process 1
    
    if(pid1 < 0) //Fork failed
    { 
        fprintf(stderr, "Fork Failed for Process 1");
        return 1;
    }
    else if (pid1 == 0) //Child process
    { 
        int cycle = 0;
        while (*counter <= 500) 
        {   
            sem_wait(sem_id);
            if (*counter % *multiplier == 0) printf("P1 - Cycle number: %d | %d is a multiple of %d \n", cycle, *counter, *multiplier);
            else printf("P1 - Cycle number: %d | %d \n", cycle, *counter);
            (*counter)++;
            cycle++;
            sem_signal(sem_id);
            sleep(1);
        }
    }
    else if (pid1 > 0) //Parent process
    {
        pid_t pid2;
        pid2 = fork(); //To create process 2

        if(pid2 < 0) //Fork failed
        {
            fprintf(stderr, "Fork Failed for Process 2");
            return 1;
        }
        else if (pid2 == 0) //Child process
        {
            execlp("./part2_5_p2","part2_5_p2",NULL);
        }
        else if (pid2 > 0) //Parent process
        {
            wait(NULL);
            shmdt(shared_memory); //deattaches memory after process completion
            shmctl(shm_id, IPC_RMID, NULL); //remove shared memory segment from main memory
            semctl(sem_id, 0, IPC_RMID, 0); // remove semaphore
            printf("Process 1 & 2 is done");
            kill(pid1, 9);
            kill(pid2, 9);
            while(1) sleep(0);
        }
    }
}