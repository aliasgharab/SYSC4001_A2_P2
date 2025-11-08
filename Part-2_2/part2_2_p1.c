#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int multiplier = 3;
    int counter = 0;

    pid_t pid1;
    pid1 = fork(); //To create process 1
    

    if(pid1 < 0) //Fork failed
    { 
        fprintf(stderr, "Fork Failed for Process 1");
        return 1;
    }
    else if (pid1 == 0) //Child process
    { 
        while (1)
        {
            if (counter % multiplier == 0) printf("Cycle number: %d | %d is a multiple of 3 \n", counter, counter);
            else printf("Cycle number: %d \n", counter);
            counter++;
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
            execlp("./part2_3_p2","part2_3_p2",NULL);
        }
        else if (pid2 > 0) //Parent process
        {
            kill(pid1, 9);
            kill(pid2, 9);
            while(1) sleep(0);
        }
    }
