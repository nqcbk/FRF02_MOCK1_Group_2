#include "readFat.h"

/* check FAT type */
FatTypes checkFatTypes(const char *filePath)
{
    /* FatTypes Return Value in range [0,1,2,3]*/
    FatTypes retVal = ERROR;

    FILE *f = fopen(filePath, "rb");

    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
        uint8_t fatName[BS_NAME_OF_FAT] = {0};

        /* the FILE pointer offset 0x36 from the beginning of file */
        fseek( f, 0x36 , 1); 

        for (int i = 0; i < BS_NAME_OF_FAT; i++)
        {
            fatName[i] = fgetc(f);
        }
        printf("[INFO] FAT name: ");
	    for (int i = 0; i < BS_NAME_OF_FAT; i++)
	    {
	        printf("%c", fatName[i]);
	    }
	    printf("\n");
        

        if (strncmp(fatName, "FAT12", 5) == 0)
        {
            retVal = FAT12;
            // printf("FAT12");
        }
        else if (strncmp(fatName, "FAT16", 5) == 0)
        {
            retVal = FAT16;
        }
        else
        {
            rewind(f);
            fseek( f, 0x52 , 1);
            
            for (int i = 0; i < BS_NAME_OF_FAT; i++)
            {
                fatName[i] = fgetc(f);
            }

            if (strncmp(fatName, "FAT32", 5) == 0)
            {
                retVal = FAT32;
            }
            else 
            {
                retVal = ERROR;
            }
        }
        
        fclose(f);
       
    }

    return retVal;
}


/* read boot sector for FAT 12 */
FAT12BootTypes *readBootSector12(const char *filePath)
{
    FAT12BootTypes *bootSector = (FAT12BootTypes *)malloc(sizeof(FAT12BootTypes));
    FILE *f = fopen(filePath, "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
        fread(bootSector, sizeof(FAT12BootTypes), 1, f);
        fclose(f);
    }
    return bootSector;
} 

// /* Define Global Values*/
// uint32_t fatCopy;
// uint32_t sectorPerFAT;
// uint32_t bytePerSector;
// uint32_t numberOfRootDirEntry;
// uint32_t rdetEntry;
// uint32_t firstRootLocation;

// uint32_t AdressOfClusterDataFile[224] = {0};
// uint32_t FileTime[224] = {0};
// uint32_t FileDate[224] = {0};
// uint32_t SizeOfFileInRoot[224] = {0};

// uint32_t count  = 0;
// uint32_t indexOfEntry[7] = {0};


uint32_t readRootDirectory12(const char *filePath){
	uint32_t number = 0;
	// Doc Boot
	FAT12BootTypes *boot = (FAT12BootTypes *)malloc(sizeof(FAT12BootTypes));
	boot = readBootSector12(filePath);
	
	// Doc Root
    FAT12RootTypes *rootSector = (FAT12RootTypes *)malloc(sizeof(FAT12RootTypes));

    FILE *f = fopen(filePath, "rb");

    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
        uint32_t fatCopy = reverseByte(boot->fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot->sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot->bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot->rdetEntry, BS_ROOT_DIR_ENTRY);
		// Tinh vi tri byte dau tien trong root
        uint32_t firstRootLocation = (NUMBER_OF_BOOT_SECTORS + fatCopy * sectorPerFAT ) * bytePerSector;
		// Chuyen con tro file toi vi tri byte dau tien trong root
        fseek(f, firstRootLocation, 1);
		// Duyet cac phan tu trong root, neu la file hoac thu muc thi in ra
        for (int i = 0; i < numberOfRootDirEntry; i++)
        {
			// Doc tung entry (32 byte)
            fread(rootSector, sizeof(FAT12RootTypes), 1, f);
			// Neu la file hoac thu muc thi in ra
            if ((ATT_LONG_FILE_NAME != (rootSector->fileAttributes[0])) && checkFileName(rootSector))
            {
				number++;
            	/* get name of file and root directory */
                printf("%d. ", number);
				int j = 0;
                for (j = 0; j < RD_FILE_NAME; j++)
                {
					if (rootSector->fileName[j] != ' '){
						printf("%c", rootSector->fileName[j]);
					}
                }
				// Neu la file thi in extension
				if (rootSector->fileAttributes[0] != ATT_DIRECTORY){
					printf(".");
					for (j = 0; j < RD_FILENAME_EXTENSION; j++){
						printf("%c", rootSector->fileNameExtension[j]);
					}
				}
                printf(" \n");
            }	
        }
        fclose(f);
    }
	return number;
}

DataHalTypes getRootData(const char *filePath, uint32_t index)
{
	DataHalTypes data;
	
    // Doc Boot
	FAT12BootTypes *boot = (FAT12BootTypes *)malloc(sizeof(FAT12BootTypes));
	boot = readBootSector12(filePath);
	
	// Doc Root
    FAT12RootTypes *rootSector = (FAT12RootTypes *)malloc(sizeof(FAT12RootTypes));

    FILE *f = fopen(filePath, "rb");

    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
        uint32_t fatCopy = reverseByte(boot->fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot->sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot->bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot->rdetEntry, BS_ROOT_DIR_ENTRY);
		// Tinh vi tri byte dau tien trong root
        uint32_t firstRootLocation = (NUMBER_OF_BOOT_SECTORS + fatCopy * sectorPerFAT) * bytePerSector;
		// Chuyen con tro file toi vi tri byte dau tien trong root
        fseek(f, firstRootLocation, 1);
		// Duyet cac phan tu trong root, neu den vi tri cua thu muc nguoi dung chon thi tra ve data thu muc do
		uint8_t number = 0;
        for (int i = 0; i < numberOfRootDirEntry; i++)
        {
			// Doc tung entry (32 byte)
            fread(rootSector, sizeof(FAT12RootTypes), 1, f);
			// Neu la file hoac thu muc thi in ra
            if ((ATT_LONG_FILE_NAME != (rootSector->fileAttributes[0])) && 0x00 != (rootSector->fileName[0]))
            {
				number++;
            	data.fileAttributes = reverseByte(rootSector->fileAttributes, RD_FILE_ATTRIBUTES);
				data.startingClusterNumber = reverseByte(rootSector->startingClusterNumber, RD_STARTING_CLUSTER_NUMBER);
				data.fileSize = reverseByte(rootSector->fileSize, RD_FILE_SIZE);
		
				if (number == index) {
					fclose(f);
					return data;
				}
            }
        }
	}
}


// Ham doc sub directory
uint32_t readSubDirectory(const char *filePath, DataHalTypes *data) {
	uint32_t number = 0;
	// Doc Boot
	FAT12BootTypes *boot = (FAT12BootTypes *)malloc(sizeof(FAT12BootTypes));
	boot = readBootSector12(filePath);
	
	// Doc sub
    FAT12RootTypes *subDirSector = (FAT12RootTypes *)malloc(sizeof(FAT12RootTypes));

    FILE *f = fopen(filePath, "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
		uint32_t fatCopy = reverseByte(boot->fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot->sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot->bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot->rdetEntry, BS_ROOT_DIR_ENTRY);
		
		// Tinh so block cua fat
        uint32_t numberOfFatSectors = fatCopy * sectorPerFAT;
		
		// Tinh so block cua root
		uint32_t numberOfRootSectors = (numberOfRootDirEntry * RD_BYTES_OF_A_ENTRY) / bytePerSector;
				
		// Tinh vi tri block dau tien cua vung data
		uint32_t firstDataLocation = NUMBER_OF_BOOT_SECTORS + numberOfFatSectors + numberOfRootSectors + 1;
		
		// Tinh vi tri block dau tien cua thu muc
		uint32_t firstFolderLocation = firstDataLocation + data->startingClusterNumber - 2;
		
		// Tinh vi tri byte dau tien cua block chua thu muc
        uint32_t startingOfSubDirLocation = (firstFolderLocation - 1) * bytePerSector;
		
		// Chuyen con tro file toi vi tri byte dau tien trong block vua tinh duoc
        fseek(f, startingOfSubDirLocation, 1);
		
		// Duyet cac phan tu trong sub, neu la file hoac thu muc thi in ra
        for (int i = 0; i < 16; i++)
        {
			// Doc tung entry (32 byte)
            fread(subDirSector, sizeof(FAT12RootTypes), 1, f);
			// Neu la file hoac thu muc thi in ra
            if ((ATT_LONG_FILE_NAME != (subDirSector->fileAttributes[0])) && checkFileName(subDirSector))
            {
				number++;
            	/* get name of file and root directory */
                printf("%d. ", number);
				int j = 0;
                for (j = 0; j < RD_FILE_NAME; j++)
                {
					if (subDirSector->fileName[j] != ' '){
						printf("%c", subDirSector->fileName[j]);
					}
                }
				// Neu la file thi in extension
				if (subDirSector->fileAttributes[0] != ATT_DIRECTORY){
					printf(".");
					for (j = 0; j < RD_FILENAME_EXTENSION; j++){
						printf("%c", subDirSector->fileNameExtension[j]);
					}
				}
                printf(" \n");
            }	
        }
        fclose(f);
    }
	return number;
}


// Ham doc sub directory
DataHalTypes getSubData(const char *filePath, DataHalTypes *data, uint32_t index) {
	DataHalTypes newData;

	// Doc Boot
	FAT12BootTypes *boot = (FAT12BootTypes *)malloc(sizeof(FAT12BootTypes));
	boot = readBootSector12(filePath);
	
	// Doc sub
    FAT12RootTypes *subDirSector = (FAT12RootTypes *)malloc(sizeof(FAT12RootTypes));

    FILE *f = fopen(filePath, "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
		uint32_t fatCopy = reverseByte(boot->fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot->sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot->bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot->rdetEntry, BS_ROOT_DIR_ENTRY);
		
		// Tinh so block cua fat
        uint32_t numberOfFatSectors = fatCopy * sectorPerFAT;
		
		// Tinh so block cua root
		uint32_t numberOfRootSectors = (numberOfRootDirEntry * RD_BYTES_OF_A_ENTRY) / bytePerSector;
				
		// Tinh vi tri block dau tien cua vung data
		uint32_t firstDataLocation = NUMBER_OF_BOOT_SECTORS + numberOfFatSectors + numberOfRootSectors + 1;
	    // uint32_t startingOfByteDataFile = firstDataLocation + AdressOfClusterDataFile[1] * bytePerSector;
		
		// Tinh vi tri block dau tien cua thu muc
		uint32_t firstFolderLocation = firstDataLocation + data->startingClusterNumber - 2;
		
		// Tinh vi tri byte dau tien cua block chua thu muc
        uint32_t startingOfSubDirLocation = (firstFolderLocation - 1) * bytePerSector;
		
        //printf("startingOfSubDirLocation : %x \n", startingOfSubDirLocation);
		
		// Chuyen con tro file toi vi tri byte dau tien trong block vua tinh duoc
        fseek(f, startingOfSubDirLocation, 1);
		
		// Duyet cac phan tu trong root, neu den vi tri cua thu muc nguoi dung chon thi tra ve data thu muc do
		uint32_t number = 0;
        for (int i = 0; i < 16; i++)
        {
			// Doc tung entry (32 byte)
            fread(subDirSector, sizeof(FAT12RootTypes), 1, f);
			// Neu la file hoac thu muc thi in ra
            if ((ATT_LONG_FILE_NAME != (subDirSector->fileAttributes[0])) && checkFileName(subDirSector))
            {
				number++;
            	newData.fileAttributes = reverseByte(subDirSector->fileAttributes, RD_FILE_ATTRIBUTES);
				newData.startingClusterNumber = reverseByte(subDirSector->startingClusterNumber, RD_STARTING_CLUSTER_NUMBER);
				newData.fileSize = reverseByte(subDirSector->fileSize, RD_FILE_SIZE);
				// printf("HERE\n");
				// printf("%x\t", subDirSector->fileAttributes);
				// printf("%x\t", subDirSector->startingClusterNumber);
				// printf("%x\t", subDirSector->fileSize);
				// printf("\n");
				if (number == index) {
					fclose(f);
					// printf("%x\t", newData.fileAttributes);
					// printf("%x\t", newData.startingClusterNumber);
					// printf("%x\t", newData.fileSize);
					printf("\n");
					return newData;
				}
            }	
        }
        fclose(f);
    }
}


// Ham doc sub directory
uint32_t readFile(const char *filePath, DataHalTypes *data) {
	// Doc Boot
	FAT12BootTypes *boot = (FAT12BootTypes *)malloc(sizeof(FAT12BootTypes));
	boot = readBootSector12(filePath);
	
	// Doc sub
    FAT12RootTypes *subDirSector = (FAT12RootTypes *)malloc(sizeof(FAT12RootTypes));

    FILE *f = fopen(filePath, "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
		uint32_t fatCopy = reverseByte(boot->fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot->sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot->bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot->rdetEntry, BS_ROOT_DIR_ENTRY);
		
		uint32_t sectorPerCluster = reverseByte(boot->sectorPerCluster, BS_SECTOR_PER_CLUSTER);

		
		
		// Tinh so block cua fat
        uint32_t numberOfFatSectors = fatCopy * sectorPerFAT;
		
		// Tinh so block cua root
		uint32_t numberOfRootSectors = (numberOfRootDirEntry * RD_BYTES_OF_A_ENTRY) / bytePerSector;
				
		// Tinh vi tri block dau tien cua vung data
		uint32_t firstDataLocation = NUMBER_OF_BOOT_SECTORS + numberOfFatSectors + numberOfRootSectors + 1;
		
		// Gan gia tri data->startingClusterNumber cho mot bien bat ky de tranh bi thay doi gia tri cua data sau vong lap
		uint32_t firstBlockPosition = data->startingClusterNumber;
		
		uint32_t count = 0;
		do {
			// Tinh vi tri block dau tien cua cluster
			uint32_t firstFolderLocation = firstDataLocation + firstBlockPosition - 2;
			
			// Tinh vi tri byte dau tien cua cluster
			uint32_t startingOfSubDirLocation = (firstFolderLocation - 1) * bytePerSector;
			
			// Chuyen con tro file toi vi tri byte dau tien cua cluster
			fseek(f, startingOfSubDirLocation, 1);
			
			// In toan bo cac block cua cluster
			uint8_t text[bytePerSector];
			uint32_t i = 0;
			for (i = 0; i < sectorPerCluster; i++) {
				fread(text, sizeof(text), 1, f);
				printf("text: %s\n", text);
			}
			
			//--------------
			// Xac dinh cac cluster ke tiep
			// Tinh vi tri chua entry cua cluster ke tiep trong FAT
			uint32_t nextClusterPosition = (2 * data->startingClusterNumber) + bytePerSector + count;
			
			// Chuyen con tro file toi vi tri vua tinh duoc
			rewind(f);
			fseek(f, nextClusterPosition, 1);
			
			// Doc 2 byte tu vi tri vua tinh duoc trong file FAT
			uint8_t fat[2];
			fread(fat, sizeof(fat), 1, f);
			
			// Reverse gia tri
			uint32_t nextClusterEntry = reverseByte(fat, 2);

			printf("nextClusterPosition = %X\n", nextClusterPosition);
			// Gan lai data vua tinh duoc cho firstBlockPosition
			firstBlockPosition = nextClusterEntry;
			printf("firstBlockPosition: %X\n", firstBlockPosition);
			// Tiep tuc chay vong lap cho den khi gap ket thuc File
			count = count + 2;
		}
		while (firstBlockPosition != ATT_END_OF_FILE);
		
		
        fclose(f);
    }
	return 0;
}
// uint32_t firstDataLocation;
// uint32_t startingOfByteDataFile;

// FAT12RootTypes *ReadFile(const char *filePath, FAT12BootTypes *boot)
// {	

    // FILE *f = fopen(filePath, "rb");
    // if (f == NULL)
    // {
        // printf("[ERROR] Cannot open file\n");
    // }
    // else
	// {	
        // printf("count: %d ", count);
        // for(int i = 0; i < count; i++)
        // {
            // printf("adress: %d\n", AdressOfClusterDataFile[i]);
        // }
	    // firstDataLocation = (1 + (fatCopy) * (sectorPerFAT) ) * bytePerSector + numberOfRootDirEntry * RD_BYTES_OF_A_ENTRY - (2 * bytePerSector);

	    // startingOfByteDataFile = firstDataLocation + AdressOfClusterDataFile[1] * bytePerSector;

   		// fclose(f);

   		// FILE *f = fopen(filePath, "rb");
   		// fseek(f, startingOfByteDataFile, 1);
   		
   		// char dataFile1[SizeOfFileInRoot[1]];
   		// for (int i = 0; i < SizeOfFileInRoot[1]; i++)
   		// {
   			// dataFile1[i] = fgetc(f);	
		// }
		// for (int i = 0; i < SizeOfFileInRoot[1]; i++)
		// {
			// printf("%c", dataFile1[i]);
		// }
        // printf("\n");
		// fclose(f);
	// }	
// }


Bool isFolder(DataHalTypes data) {
	Bool result = FALSE;
	if (data.fileAttributes == ATT_DIRECTORY){
		result = TRUE;
	} 
	return result;
}

static Bool checkFileName(FAT12RootTypes *root) {
	Bool result = TRUE;
	switch(root->fileName[0]){
		case FILENAME_NEVER_USED:
			result = FALSE;
		case FILENAME_DELETED_1:
			result = FALSE;
		case FILENAME_DELETED_2:
			result = FALSE;
		case FILENAME_SPECIAL_ENTRY:
			result = FALSE;
	}
	return result;
}

/* reverse byte */
uint32_t reverseByte(uint8_t *byte, uint32_t count){
    uint32_t result = 0;
    for (int i = count-1; i >= 0; i--)
    {
        result = (result << 8) | byte[i];
    }
    return result;
} 