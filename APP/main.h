/*** HARDWARE ABSTRACTION LAYER ***/
#ifndef _MAIN_H_
#define _MAIN_H_

#include "hal.h"
#include "timer.h"

#define NEWLINE printf("\n");
#define TAB printf("\t");
#define ENDLINE printf("------------------------------------------------------------------------");
#define EXIT printf("0. Exit");
#define CLS system("clear");
#define BACK printf("0. Back");
#define PAUSE system("pause");

#define v_NEWLINE(N)\
        {\
            for(int i=0; i<N;i++){NEWLINE}\
        }

#define v_TAB(N)\
        {\
            for(int i=0; i<N;i++){TAB}\
        }

#endif /*_MAIN_H_*/


