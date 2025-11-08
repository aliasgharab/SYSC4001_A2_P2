#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
    int cycle = 0;
    int counter = 0;
    while (1)
    {
        if (counter % 3 == 0) printf("Cycle number: %d | %d is a multiple of 3 \n", cycle, counter);
        else printf("Cycle number: %d \n", cycle);
        counter--;
        cycle++;
        sleep(1);
    }
}