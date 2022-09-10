#include "hal.h"

void main(int argc, char* argv[]){

    const char *path = "floppy.img";
	// const char *path = argv[1];
	Generation *gen = NULL;
	uint32_t numberOfElements = 0;	
	uint32_t input = 0;
	
	numberOfElements = openDirectory(&gen, 0);
	do {
		if (getLevel(gen) > 1) {
			printf("0. Back\n");
		}
		else {
			printf("0. Exit\n");
		}
		// Nguoi dung nhap vao mot so trong doan 0 - a
		input = inputNumberByUser(numberOfElements);
		if (input == 0) {
			numberOfElements = closeDirectory(&gen);
		}
		else {
			numberOfElements = openDirectory(&gen, input - 1);
		}
	} while (getLevel(gen));
}