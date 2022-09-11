#include "hal.h"

void main(int argc, char* argv[]){

    const char *filePath = "floppy.img";
	// const char *path = argv[1];
	Node *head = NULL;
	uint32_t numberOfElements = 0;	
	uint32_t input = 0;
	int32_t run = 1;
	do {
		// Khi list chua co phan tu nao thi open root
		if (run == 1) {
			system("clear");
			numberOfElements = openRoot(&head, filePath);
		}
		
		if (run > 1) {
			printf("\n0. Back\n");
		}
		else {
			printf("\n0. Exit\n");
		}
		
		// Nguoi dung nhap vao mot so trong doan 0 - numberOfElements
		input = inputNumberByUser(numberOfElements);
		if (input == 0) {
			system("clear");
			numberOfElements = closeDirectory(&head, filePath); 
			run--;
		}
		else {
			// Neu khong phai thu muc rong thi mo tiep
			if (numberOfElements != 0) {
				system("clear");
				numberOfElements = openDirectory(&head, filePath, input);
				run++;
			}
		}
		printf("run: %d\n", run);
	} while (run);
}