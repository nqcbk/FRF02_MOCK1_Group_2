#include "main.h"

void main(int argc, char* argv[]){
    // const char *filePath = "floppy.img";
	const char *filePath = argv[1];
	uint32_t u_32numberOfElements = 0;	
	uint32_t u_32input = 0;
	int32_t u_32run = 0;
	int32_t u_32timer = 0;
	uint32_t u_32checkFAT;
	
	u_32checkFAT = checkFatTypes(filePath);
	
	v_NEWLINE(2)
	
	if(u_32checkFAT == 0)
	{
		CLS	v_NEWLINE(2)	v_TAB(4)
		printf("[INFO]Check file is: FAT12");
		v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(2)	v_TAB(4)
		printf("[INFO]Begin read file: FAT12");
		v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(1)	v_TAB(1) 
		printf("[INFO]press any key resume to read file.");
		PAUSE
		v_TAB(1)
		printfPoint_callback();
		printPercent_callback();

		do {
			/* Open root directory when run program */
			if (u_32run == 0) {
				CLS
				u_32numberOfElements = openRoot(filePath);
				v_NEWLINE(2)	v_TAB(1)	EXIT
				u_32run++;
			}
			/* The user will input a number from 0 to numberOfElements */
			u_32input = inputNumberByUser(u_32numberOfElements);
			/* If the user inputs 0, open the previous folder */
			if (u_32input == 0) {
				if (u_32run > 1){
					CLS
					u_32numberOfElements = closeDirectory(filePath); 
				}
				u_32run--;
			}
			else {
				CLS
				u_32numberOfElements = openDirectory(filePath, u_32input);
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
	else if(u_32checkFAT == 1)	
	{
		v_TAB(4)
		printf("INFO]Check file is: FAT16");
		v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(2)	v_TAB(3)
		printf("INFO]Begin read file: FAT16");
		v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(1)
	}
	else if(u_32checkFAT == 2)	
	{
		v_TAB(4)
		printf("[INFO]Check file is: FAT32");
		v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(2)	v_TAB(3)
		printf("[INFO]Begin read file: FAT32");
		v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(1);
	}
	else	
	{
		v_TAB(4)
		printf("[ERROR]This file is not FAT");
		v_NEWLINE(1)	v_TAB(1)	ENDLINE		v_NEWLINE(1)
	}
}