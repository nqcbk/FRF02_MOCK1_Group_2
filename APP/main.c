#include "hal.h"

int main(int argc, char* argv[]){

    // const char *path = "floppy.img";
	const char *path = argv[1];
	Generation *genHead = getData();
	// In data trong Root
	uint32_t a = openDirectory(genHead, g_currentLevel, 0);
	uint32_t input = inputNumberByUser(a);
	a = openDirectory(genHead, g_currentLevel, input);
	// while (g_currentLevel > 0) {
		// Nguoi dung nhap vao mot so trong doan 0 - a
		// uint32_t input = inputNumberByUser(a);
		// if (input == 0) {
			// Viet 1 ham tra hien thi thu muc cha o day
			// a = closeDirectory(genHead); 
		// }
		// else {
			// a = openDirectory(genHead, g_currentLevel, input);
			// if (isDirectory(input)) {
				// a = openDirectory(genHead, g_currentLevel, input);
			// }
			// if (isFile(input)) {
				// // Viet mot ham open File
				// //openFile(genHead, g_currentLevel, input);
			// }
		// }
	// }
	showDirectory()
	

    
    return 0;
}

