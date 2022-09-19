/*** APP DISPLAY ***/
#ifndef _APP_DISPLAY_H_
#define _APP_DISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "hal.h"

typedef enum APP_CHECK_FAT
{
    APP_FAT12 = 0,
    APP_FAT16,
    APP_FAT32,
    APP_ERROR
} App_FAT_type_t;

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

void app_setTimeout(int milliseconds);
void app_printPoint(int time, int pcs);
void app_printfPoint_callback(void);
void app_printPercent(int time, int pcs);
void app_printPercent_callback(void);
void app_display_fat12();
void app_display_fat16();
void app_display_fat32();
void app_display_error();
void app_read_FAT(const char * app_filePath);
void app_process_file_FAT(const char * app_filePath);

#endif /*_MAIN_H_*/


