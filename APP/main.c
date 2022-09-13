#include "hal.h"



void main(int argc, char* argv[]){
    const char *filePath = "floppy.img";
	// const char *path = argv[1];
	uint32_t numberOfElements = 0;	
	uint32_t input = 0;
	int32_t run = 0;
	do {
		/* Open root directory when run program */
		if (run == 0) {
			system("clear");
			numberOfElements = openRoot(filePath);
			printf("\n\n0. Exit");
			run++;
		}
		/* The user will input a number from 0 to numberOfElements */
		input = inputNumberByUser(numberOfElements);
		/* If the user inputs 0, open the previous folder */
		if (input == 0) {
			if (run > 1){
				system("clear");
				numberOfElements = closeDirectory(filePath); 
			}
			run--;
		}
		else {
			system("clear");
			numberOfElements = openDirectory(filePath, input);
			run++;
		}
		if (run > 1) {
			printf("\n\n0. Back");
		}
		if (run == 1) {
			printf("\n\n0. Exit");
		}
	} while (run);
}


