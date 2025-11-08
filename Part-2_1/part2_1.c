#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
    pid_t pid1;
    pid1 = fork(); // To create process 1
    
    if (pid1 < 0) { // Fork failed
        fprintf(stderr, "Fork Failed for Process 1\n");
        return 1;
    }
    else if (pid1 == 0) { // Child process (Process 1)
        int counter = 0;
        while (1) {
            counter++;
            printf("Process 1 - The counter is currently at: %d\n", counter);
            sleep(2);
        }
    }
    else if (pid1 > 0) { // Parent process
        pid_t pid2;
        pid2 = fork(); // To create process 2
        
        if (pid2 < 0) { // Fork failed
            fprintf(stderr, "Fork Failed for Process 2\n");
            return 1;
        }
        else if (pid2 == 0) { // Child process (Process 2)
            int counter = 0;
            while (1) {
                counter++;
                printf("Process 2 - The counter is currently at: %d\n", counter);
                sleep(2);
            }
        }
        else if (pid2 > 0) { // Parent process

            //kill the children processes: 
            kill(pid1, 9);
            kill(pid2, 9);
            while(1) sleep(0);
        }
    }
    
    return 0;
}