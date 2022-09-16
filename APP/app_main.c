#include "app_main.h"

void main(int argc, char* argv[]){
	const char *filePath = argv[1];
	app_process_file_FAT(filePath);
}