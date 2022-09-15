#include "timer.h"
/*
    Implementation simple timeout

    Input: count milliseconds as number

    Usage:
        setTimeout(1000) - timeout on 1 second
        setTimeout(10100) - timeout on 10 seconds and 100 milliseconds
 */
void setTimeout(int milliseconds)
{
    // If milliseconds is less or equal to 0
    // will be simple return from function without throw error
    if (milliseconds <= 0) {
        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        return;
    }

    // a current time of milliseconds
    int milliseconds_since = clock() * 100 / CLOCKS_PER_SEC;

    // needed count milliseconds of return from this timeout
    int end = milliseconds_since + milliseconds;

    // wait while until needed time comes
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}

void printPoint(int time, int pcs)
{
  int delay = 0;
  system("clear");
  printf("\n\n\t\tLoading File ");
    do {
		printf(".");
        setTimeout(time);
        delay++;
    }   while (delay <= pcs);
}

void printPercent(int time, int pcs)
{
  int delay = 0;
    do {
        system("clear");
        printf("\n\n\t\tLoading ............ ");
		printf("%d0%%", delay);
        setTimeout(time);
        if(delay == 8 || delay == 6 )
        {
            setTimeout(1000);
        }
        delay++;
    }   while (delay <= pcs);
}

void printfPoint_callback(void)
{
    printPoint(100, 50);
    printPoint(250, 4); 
    printPoint(500, 1); 
    printPoint(50, 10);
}

void printPercent_callback(void)
{ 
    printPercent(100, 10); 
}