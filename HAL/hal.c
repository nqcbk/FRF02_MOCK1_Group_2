#include "hal.h"

/** Function Pointers */
static uint32_t (*readRootDirectory[])(const char *filePath) = {
	&readRootDirectory12,
	&readRootDirectory16,
	&readRootDirectory32
};

static DataTypes (*getRootData[])(const char *filePath, uint32_t index) = {
	&getRootData12,
	&getRootData16,
	&getRootData32
};

static uint32_t (*readSubDirectory[])(const char *filePath, DataTypes *data) = {
	&readSubDirectory12,
	&readSubDirectory16,
	&readSubDirectory32
};

static DataTypes (*getSubData[])(const char *filePath, DataTypes *data, uint32_t index) = {
	&getSubData12,
	&getSubData16,
	&getSubData32
};

static uint32_t (*readFile[])(const char *filePath, DataTypes *data) = {
	&readFile12,
	&readFile16,
	&readFile32
};

static FatTypes g_type;

/* This function is to open root */
uint32_t openRoot(const char *filePath){
	uint32_t numberOfElements = 0;
	/* Check type of FAT file system here */
	g_type = checkFatTypes(filePath);
	if (g_type == ERROR) {
		printf("NOT A FAT TYPE!");
		numberOfElements = 0;
	}
	else {
		/* Print all elements in root directory */
		numberOfElements = (*readRootDirectory[g_type])(filePath);
	}
	/* Return number of elements in root directory */
	return numberOfElements;
}

/* This function is to open a directory	*/
uint32_t openDirectory(const char *filePath, uint8_t index) {
	uint32_t count = 0;
	uint32_t numberOfElements  = 0;
	/* Get data from last element in linked list */
	DataTypes data = getDataFromLastNode();
	/* If there is no element in linked list, get data from root directory and create an element in linked list */
	if (numberOfNode() == 0) {
		data = (*getRootData[g_type])(filePath, index);
		addNode(data);
	}
	/* Get data from sub directory and also create an element to linked list */
	else {
		data = (*getSubData[g_type])(filePath, &data, index);
		addNode(data);
	}
	/* If users select a folder, open sub directory */
	if (isFolder(data)) {
		numberOfElements = (*readSubDirectory[g_type])(filePath, &data);
	}
	/* If users select a file, open that one */
	else {
		numberOfElements = (*readFile[g_type])(filePath, &data);
	}
	/* Return number of elements in sub directory */
	return numberOfElements;
}

/* This function is to close a directory */
uint32_t closeDirectory(const char *filePath) {
	uint32_t count = 0;
	uint32_t numberOfElements  = 0;
	/* Delete the last element in linked list */
	deleteLastNode();
	/* If there is no element left in linked list, read root directory */
	if (numberOfNode() == 0) {
		numberOfElements = (*readRootDirectory[g_type])(filePath);
	}
	else {
		/* Get data from last element in linked list */
		DataTypes data = getDataFromLastNode();
		/* Show sub directory */
		numberOfElements = (*readSubDirectory[g_type])(filePath, &data);
	}
	/* Return number of elements in sub directory or root directory */
	return numberOfElements;
}

/* This function is to check the number inputed by user	
 * The number is a positive integer from 0 to maxNumber */
uint32_t inputNumberByUser(uint32_t maxNumber) {
	uint32_t number = 0;
	do{
		printf("\n\tPlease input your Select from keyboard: ");
		
		scanf("%d", &number);
		fflush(stdin);
	}while((number < 0) || (number > maxNumber));
	return number;
}


