#include "app_display.h"
/*
    Implementation simple timeout

    Input: count milliseconds as number

    Usage:
        setTimeout(1000) - timeout on 1 second
        setTimeout(10100) - timeout on 10 seconds and 100 milliseconds
 */
void app_setTimeout(int milliseconds)
{
    // If milliseconds is less or equal to 0
    // will be simple return from function without throw error
    if (milliseconds <= 0) {
        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        return;
    }

    // a current time of milliseconds
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;

    // needed count milliseconds of return from this timeout
    int end = milliseconds_since + milliseconds;

    // wait while until needed time comes
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}

void app_printPoint(int time, int pcs)
{
  int delay = 0;
  CLS
  printf("\n\n\t\tLoading File ");
    do {
		printf(".");
        app_setTimeout(time);
        delay++;
    }   while (delay <= pcs);
}

void app_printPercent(int time, int pcs)
{
  int delay = 0;
    do {
        CLS
        printf("\n\t\tLoading ............ ");
		printf("%d0%%", delay);
        app_setTimeout(time);
        if(delay == 8 || delay == 6 )
        {
            app_setTimeout(200);
        }
        delay++;
    }   while (delay <= pcs);
}

void app_printfPoint_callback(void)
{
    app_printPoint(100, 50);
    app_printPoint(250, 4); 
    app_printPoint(500, 1); 
    app_printPoint(50, 10);
}

void app_printPercent_callback(void)
{ 
    app_printPercent(100, 10); 
}

void app_display_fat12()
{
	app_setTimeout(200);
	v_TAB(1)
	app_printPercent_callback();
	v_NEWLINE(2)	v_TAB(4)
	CLS
	printf("FILE SYSTEM TYPE: FAT12");
	v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(2)	v_TAB(4)
}

void app_display_fat16()
{
    app_setTimeout(200);
	v_TAB(1)
	app_printPercent_callback();
	v_NEWLINE(2)	v_TAB(4)
	CLS
	printf("FILE SYSTEM TYPE: FAT16");
	v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(2)	v_TAB(4)
}

void app_display_fat32()
{
    app_setTimeout(200);
	v_TAB(1)
	app_printPercent_callback();
	v_NEWLINE(2)	v_TAB(4)
	CLS
	printf("FILE SYSTEM TYPE: FAT32");
	v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(2)	v_TAB(4)
}

void app_display_error()
{
    v_TAB(4)
    printf("[ERROR]This file is not FAT");
    v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(1)
}
void app_read_FAT(const char * app_filePath)
{
    uint32_t u_32numberOfElements = 0;	
	uint32_t u_32input = 0;
	int32_t u_32run = 0;
	int32_t u_32timer = 0;

    do {
			/* Open root directory when run program */
			if (u_32run == 0) {
				// CLS
				u_32numberOfElements = openRoot(app_filePath);
				v_NEWLINE(2)	v_TAB(1)	EXIT
				u_32run++;
			}
			/* The user will input a number from 0 to numberOfElements */
			u_32input = inputNumberByUser(u_32numberOfElements);
			/* If the user inputs 0, open the previous folder */
			if (u_32input == 0) {
				if (u_32run > 1){
					CLS
					u_32numberOfElements = closeDirectory(app_filePath); 
				}
				u_32run--;
			}
			else {
				CLS
				u_32numberOfElements = openDirectory(app_filePath, u_32input);
				u_32run++;
			}
			if (u_32run > 1) {
				v_NEWLINE(2)	v_TAB(1)	BACK
			}
			if (u_32run == 1) {
				v_TAB(1)	v_NEWLINE(2)	v_TAB(1) EXIT
			}
		} while (u_32run);
}

void app_process_file_FAT(const char * app_filePath)
{
	uint32_t u_32checkFAT;
	
	u_32checkFAT = checkFatTypes(app_filePath);
	
	v_NEWLINE(2)

    switch (u_32checkFAT)
    {
    case APP_FAT12:
        app_display_fat12(app_filePath);
        app_read_FAT(app_filePath);
        break;

    case APP_FAT16:
        app_display_fat16(app_filePath);
        app_read_FAT(app_filePath);
        break;
    
    case APP_FAT32:
        app_display_fat32(app_filePath);
        app_read_FAT(app_filePath);
        break;
    
    default:
        app_display_error(app_filePath);
        app_read_FAT(app_filePath);
        break;
    }
}