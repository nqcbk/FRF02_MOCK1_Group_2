/*** HARDWARE ABSTRACTION LAYER ***/
#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void setTimeout(int milliseconds);
void printPoint(int time, int pcs);
void printfPoint_callback(void);
void printPercent(int time, int pcs);
void printPercent_callback(void);

#endif /*_MAIN_H_*/


