#include "readFat.h"


/* This function is to check type of the FAT files */
FatTypes checkFatTypes(const char *filePath){
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
		int i = 0;
        /* the FILE pointer offset 0x36 from the beginning of file */
        fseek( f, FAT_TYPE_POSITION , 1); 
        for (i = 0; i < BS_NAME_OF_FAT; i++)
        {
            fatName[i] = fgetc(f);
			// printf("%c", fatName[i]);
        }
	    printf("\n");
        
		if(fatName[0] == 'F' && fatName[1] == 'A' && fatName[2] == 'T' && fatName[5] == ' ' && fatName[6] == ' ' && fatName[7] == ' ')
		{
			if(fatName[3] == '1')
			{
				switch(fatName[4])
				{
					case '2': return retVal = FAT12;
					break;
					case '6': return retVal = FAT16;
					break;
					default:  goto ERROR;
					break;
				}
			}
		}
		else
		{
            rewind(f);
            fseek( f, FAT32_FAT_TYPE_POSITION , SEEK_SET);
            
            for (int i = 0; i < BS_NAME_OF_FAT; i++){
                fatName[i] = fgetc(f);
            }
            if(fatName[0] == 'F' && fatName[1] == 'A' && fatName[2] == 'T' && fatName[5] == ' ' && fatName[6] == ' ' && fatName[7] == ' ')
			{
				if(fatName[3] == '3' && fatName[4] == '2')
				{
					return retVal = FAT32;
				}
			}
			else
			{
				goto ERROR;
			}
		}
		ERROR:
		return retVal = ERROR;
        fclose(f);
    }
    return retVal;
}

/* This function is to read boot sector in FAT12 files */
FAT12BootTypes readBootSector12(const char *filePath){
	FAT12BootTypes boot;
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
        fread(&boot, sizeof(FAT12BootTypes), 1, f);
        fclose(f);
    }
    return boot;
}

/* This function is to read boot sector in FAT16 files */
FAT16BootTypes readBootSector16(const char *filePath){
    FAT16BootTypes boot;
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
        fread(&boot, sizeof(FAT16BootTypes), 1, f);
        fclose(f);
    }
    return boot;
}

/* This function is to read boot sector in FAT32 files */
FAT32BootTypes readBootSector32(const char *filePath){
    FAT32BootTypes boot;
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
        fread(&boot, sizeof(FAT32BootTypes), 1, f);
        fclose(f);
    }
    return boot;
} 

/* This function is to read root directory in FAT12 files */
uint32_t readRootDirectory12(const char *filePath){
	uint32_t number = 0;
	/* Read Boot */
	FAT12BootTypes boot = readBootSector12(filePath);
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
        uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
		/* Calculate fist byte in root location */ 
        uint32_t firstRootLocation = (NUMBER_OF_BOOT_SECTORS + fatCopy * sectorPerFAT ) * bytePerSector;
		/* Move pointer of file to first byte in root location */
        fseek(f, firstRootLocation, 1);
		/* Check elements of root, if it's file or folder, print */
		printf("\n\n");
		printf("\t|Name|\t\t|Type|\t\t|Size|\t\t|Date|\t\t|Time|\n");
		printf("\n");
		EntryFormat root;
        for (int i = 0; i < numberOfRootDirEntry; i++){
			/* Read each entry (32 bytes)*/ 
            fread(&root, sizeof(EntryFormat), 1, f);
			/* If it is file or folder, print */ 
            if (isFileOrFolder(root)){
				number++;
            	/* Get name of file and root directory */
                printf("\t%d. ", number);
				int j = 0;
                for (j = 0; j < RD_FILE_NAME; j++)
                {
					printf("%c", root.fileName[j]);
                }
                printf("\t");

                /* If it is a file, print extension */ 
                if (root.fileAttributes[0] != ATT_DIRECTORY)
				{
					printf("File ");
					for (j = 0; j < RD_FILENAME_EXTENSION; j++)
					{
						printf("%c", root.fileNameExtension[j]);
					}
					printf("\t");
				}

				/* If it's a folder, print file folder */
				if (root.fileAttributes[0] == ATT_DIRECTORY)
				{
					printf("File Folder");
					printf("\t");
				} 
				
				/* Get size of file */
				if (root.fileAttributes[0] != ATT_DIRECTORY)
				{
					
					printf("%d bytes", reverseByte(root.fileSize, RD_FILE_SIZE));
					printf("\t");
				}
				if (root.fileAttributes[0] == ATT_DIRECTORY)
				{
					
					printf("\t\t");
				}

				/* Get date of file */
        		uint32_t DateAfterReverse = reverseByte(root.fileDate, RD_FILE_DATE);
        		uint16_t year = CALC_YEAR(DateAfterReverse);
        		uint16_t month = CALC_MONTH(DateAfterReverse);
        		uint16_t day = CALC_DAY(DateAfterReverse);
 
                // printf("%04d/%02d/%02d\t", year + BIN_YEAR_OFFSET_FROM, month, day);
                printf("%02d/%02d/%04d\t", day, month, year + BIN_YEAR_OFFSET_FROM);

                /* Get time of file */
				uint32_t TimeAfterReverse = reverseByte(root.fileTime, RD_FILE_TIME);
				uint16_t hours = CALC_HOURS(TimeAfterReverse);
				uint16_t minutes = CALC_MINUTES(TimeAfterReverse);
				uint16_t seconds = CALC_SECONDS(TimeAfterReverse);

				// printf("%02d:%02d:%02d", hours, minutes, seconds);
				printf("%02d:%02d:%02d", hours, minutes, seconds);
               
				printf(" \n");
            }	
        }
        fclose(f);
    }
	return number;
}

/* This function is to read root directory in FAT16 files */
uint32_t readRootDirectory16(const char *filePath)
{
	uint32_t number = 0;
	/* Read Boot */
	FAT16BootTypes boot = readBootSector16(filePath);
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
        uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
		/* Calculate fist byte in root location */ 
        uint32_t firstRootLocation = (NUMBER_OF_BOOT_SECTORS + fatCopy * sectorPerFAT ) * bytePerSector;
		/* Move pointer of file to first byte in root location */
        fseek(f, firstRootLocation, 1);
		/* Check elements of root, if it's file or folder, print */
		printf("\n\n");
		printf("\t|Name|\t\t|Type|\t\t|Size|\t\t|Date|\t\t|Time|\n");
		printf("\n");
		EntryFormat root;
        for (int i = 0; i < numberOfRootDirEntry; i++)
        {
			/* Read each entry (32 bytes)*/ 
            fread(&root, sizeof(EntryFormat), 1, f);
			/* If it is file or folder, print */ 
            if ((ATT_LONG_FILE_NAME != (root.fileAttributes[0])) && checkFileName(root)){
				number++;
            	/* Get name of file and root directory */
                printf("\t%d. ", number);
				int j = 0;
                for (j = 0; j < RD_FILE_NAME; j++)
                {
					printf("%c", root.fileName[j]);
                }
                printf("\t");

                /* If it is a file, print extension */ 
                if (root.fileAttributes[0] != ATT_DIRECTORY)
				{
					printf("File ");
					for (j = 0; j < RD_FILENAME_EXTENSION; j++)
					{
						printf("%c", root.fileNameExtension[j]);
					}
					printf("\t");
				}

				/* If it's a folder, print file folder */
				if (root.fileAttributes[0] == ATT_DIRECTORY)
				{
					printf("File Folder");
					printf("\t");
				} 
				
				/* Get size of file */
				if (root.fileAttributes[0] != ATT_DIRECTORY)
				{
					
					printf("%d bytes", reverseByte(root.fileSize, RD_FILE_SIZE));
					printf("\t");
				}
				if (root.fileAttributes[0] == ATT_DIRECTORY)
				{
					
					printf("\t\t");
				}

				/* Get date of file */
        		uint32_t DateAfterReverse = reverseByte(root.fileDate, RD_FILE_DATE);
        		uint16_t year = CALC_YEAR(DateAfterReverse);
        		uint16_t month = CALC_MONTH(DateAfterReverse);
        		uint16_t day = CALC_DAY(DateAfterReverse);
 
                // printf("%04d/%02d/%02d\t", year + BIN_YEAR_OFFSET_FROM, month, day);
                printf("%02d/%02d/%04d\t", day, month, year + BIN_YEAR_OFFSET_FROM);

                /* Get time of file */
				uint32_t TimeAfterReverse = reverseByte(root.fileTime, RD_FILE_TIME);
				uint16_t hours = CALC_HOURS(TimeAfterReverse);
				uint16_t minutes = CALC_MINUTES(TimeAfterReverse);
				uint16_t seconds = CALC_SECONDS(TimeAfterReverse);

				// printf("%02d:%02d:%02d", hours, minutes, seconds);
				printf("%02d:%02d:%02d", hours, minutes, seconds);
               
				printf(" \n");
            }	
        }
        fclose(f);
    }
	return number;
}

/* This function is to read root directory in FAT32 files */
uint32_t readRootDirectory32(const char *filePath){
	/* SHALL BE DEVELOPED IN THE FUTURE! */
	printf("NOT SUPPORT TYPE FAT32!\n");
	return 0;
}

/* This function is to get data from Root to DataTypes in FAT12 files */
DataTypes getRootData12(const char *filePath, uint32_t index){
	DataTypes data;
	/* Read Boot */
	FAT12BootTypes boot = readBootSector12(filePath);
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
        uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
		/* Calculate first byte postion in root directory */
        uint32_t firstRootLocation = (NUMBER_OF_BOOT_SECTORS + fatCopy * sectorPerFAT) * bytePerSector;
		/* Move file pointer to the position */
        fseek(f, firstRootLocation, 1);
		/* Check elements of root, if it's folder location of user's input, return it's data */
		EntryFormat root;
		uint8_t number = 0;
        for (int i = 0; i < numberOfRootDirEntry; i++){
			/* Read each entry(32 bytes) */ 
            fread(&root, sizeof(EntryFormat), 1, f);
			/* Save file or folder which users select to data struct */ 
            if (isFileOrFolder(root)){
				number++;
            	data.fileAttributes = reverseByte(root.fileAttributes, RD_FILE_ATTRIBUTES);
				data.startClusNum = reverseByte(root.startClusNum, RD_STARTING_CLUSTER_NUMBER);
				data.fileSize = reverseByte(root.fileSize, RD_FILE_SIZE);
				if (number == index){
					fclose(f);
					return data;
				}
            }
        }
	}
}

/* This function is to get data from Root to DataTypes in FAT16 files */
DataTypes getRootData16(const char *filePath, uint32_t index){
	DataTypes data;
	/* Read Boot */
	FAT16BootTypes boot = readBootSector16(filePath);
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
        uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
		/* Calculate first byte postion in root directory */
        uint32_t firstRootLocation = (NUMBER_OF_BOOT_SECTORS + fatCopy * sectorPerFAT) * bytePerSector;
		/* Move file pointer to the position */
        fseek(f, firstRootLocation, 1);
		/* Check elements of root, if it's folder location of user's input, return it's data */
		EntryFormat root;
		uint8_t number = 0;
        for (int i = 0; i < numberOfRootDirEntry; i++){
			/* Read each entry(32 bytes) */ 
            fread(&root, sizeof(EntryFormat), 1, f);
			/* Save file or folder which users select to data struct */ 
            if ((ATT_LONG_FILE_NAME != (root.fileAttributes[0])) && checkFileName(root)){
				number++;
            	data.fileAttributes = reverseByte(root.fileAttributes, RD_FILE_ATTRIBUTES);
				data.startClusNum = reverseByte(root.startClusNum, RD_STARTING_CLUSTER_NUMBER);
				data.fileSize = reverseByte(root.fileSize, RD_FILE_SIZE);
				if (number == index){
					fclose(f);
					return data;
				}
            }
        }
	}
}

/* This function is to get data from Root to DataTypes in FAT32 files */
DataTypes getRootData32(const char *filePath, uint32_t index){
	/* SHALL BE DEVELOPED IN THE FUTURE */
}

/* This function is to read sub directory in FAT12 files */
uint32_t readSubDirectory12(const char *filePath, DataTypes *data) {
	uint32_t number = 0;
	/* Read Boot */
	FAT12BootTypes boot = readBootSector12(filePath);	
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
		uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
		uint32_t sectorPerCluster = reverseByte(boot.sectorPerCluster, BS_SECTOR_PER_CLUSTER);
		/* Calculate number of FAT Area sectors */
        uint32_t numberOfFatSectors = fatCopy * sectorPerFAT;
		/* Calculate number of Root Area sectors */
		uint32_t numberOfRootSectors = (numberOfRootDirEntry * RD_NUMBER_OF_BYTES_PER_DOUBLE_LINES) / bytePerSector;
		/* Calculate the first sector position of Data Area */	
		uint32_t firstDataLocation = NUMBER_OF_BOOT_SECTORS + numberOfFatSectors + numberOfRootSectors - 2;
		/* entryOfCLuster is assigned to avoid to change the value of data->startClusNum after loop */
		uint32_t entryOfCLuster = data->startClusNum;
		EntryFormat dir;
		printf("\n\n");
		printf("\t|Name|\t\t|Type|\t\t|Size|\t\t|Date|\t\t|Time|\n");
		printf("\n");
		uint32_t nextClusterPosition = 0;
		do {
			/* Calculate the first sector's position of cluster */
			uint32_t firstSectorPosition = firstDataLocation + entryOfCLuster;
			/* Calculte the first byte's position of cluster */
			uint32_t firstClusterPosition = firstSectorPosition * bytePerSector ;
			/* Move the file's poiter to cluster's position */
			fseek(f, firstClusterPosition, 1);
			uint32_t i = 0;
			/* Read a cluster */
			for (int i = 0; i < (16 * sectorPerCluster); i++){
				/* Read each entry (32 bytes) */ 
				fread(&dir, sizeof(EntryFormat), 1, f);
				/* If it's a file or folder, print */ 
				if (isFileOrFolder(dir)){
					number++;
					/* Get name of file and root directory */
					printf("\t%d. ", number);
					int j = 0;
					for (j = 0; j < RD_FILE_NAME; j++)
					{
						printf("%c", dir.fileName[j]);	
					}
					printf("\t");
					/* If it's a file, print the extension */
					if (dir.fileAttributes[0] != ATT_DIRECTORY)
					{
						printf("File ");
						for (j = 0; j < RD_FILENAME_EXTENSION; j++)
						{
							printf("%c", dir.fileNameExtension[j]);
						}
						printf("\t");
					}
					if (dir.fileAttributes[0] == ATT_DIRECTORY)
					{
						printf("File Folder");
						printf("\t");
						}
					/* Get size of file */
					if (dir.fileAttributes[0] != ATT_DIRECTORY)
					{
						
						printf("%d bytes", reverseByte(dir.fileSize, RD_FILE_SIZE));
						printf("\t");
					}
					if (dir.fileAttributes[0] == ATT_DIRECTORY)
					{
						
						printf("\t\t");
				}
					/* Get date of file */
	        		uint32_t DateAfterReverse = reverseByte(dir.fileDate, RD_FILE_DATE);
	        		uint16_t year = CALC_YEAR(DateAfterReverse);
	        		uint16_t month = CALC_MONTH(DateAfterReverse);
	        		uint16_t day = CALC_DAY(DateAfterReverse);
	 
	                printf("%02d/%02d/%04d\t", day, month, year + BIN_YEAR_OFFSET_FROM);
	                
	                /* Get time of file */
					uint32_t TimeAfterReverse = reverseByte(dir.fileTime, RD_FILE_TIME);
					uint16_t hours = CALC_HOURS(TimeAfterReverse);
					uint16_t minutes = CALC_MINUTES(TimeAfterReverse);
					uint16_t seconds = CALC_SECONDS(TimeAfterReverse);

					printf("%02d:%02d:%02d", hours, minutes, seconds);
					printf(" \n");
				}	
			}
			/* Determine the next cluster */
			/* In FAT12 Type, every 3 bytes represent 2 entries */
			/* Calculate the offset of next cluster's entry in FAT Area */
			uint32_t nextClusterOffset = (uint32_t)(1.5 * entryOfCLuster);
			uint8_t doubleEntry[3] = {0};
			uint32_t nextClusterEntry = 0;
			/* In case nextClusterOffset is not divisible by 3, nextClusterEntry is at the middle byte's position, 
			 * and we will get the second entry */
			if ((nextClusterOffset % 3) != 0) {
				/* Calculate the first position of 3 bytes */
				nextClusterPosition = nextClusterOffset - 1 + bytePerSector;
				if (nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector)) {
					/* Move the file's poiter to the position */
					rewind(f);
					fseek(f, nextClusterPosition, 1);
					/* Read 3 bytes */
					fread(doubleEntry, sizeof(doubleEntry), 1, f);
					/* Get the second entry */
					nextClusterEntry = getEntryFat12(doubleEntry, 2);
				}	
			}
			/* In case nextClusterOffset is divisible by 3, nextClusterEntry is at the first byte's position, 
			 * and we will get the first entry */
			else {
				/* Calculate the first position of 3 bytes */
				nextClusterPosition = nextClusterOffset + bytePerSector;
				if (nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector)) {
					/* Move the file's poiter to the position */
					rewind(f);
					fseek(f, nextClusterPosition, 1);
					/* Read 3 bytes */
					fread(doubleEntry, sizeof(doubleEntry), 1, f);
					/* Get the second entry */
					nextClusterEntry = getEntryFat12(doubleEntry, 1);
				}
			}
			/* Assign data to entryOfCLuster */
			entryOfCLuster = nextClusterEntry;
		}
		while ((entryOfCLuster != FAT12_ENTRY_EOC) && nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector));
        fclose(f);
    }
	return number;
}

/* This function is to read sub directory in FAT16 files */
uint32_t readSubDirectory16(const char *filePath, DataTypes *data) {
	uint32_t number = 0;
	/* Read Boot */
	FAT16BootTypes boot = readBootSector16(filePath);	
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
		uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
        uint32_t sectorPerCluster = reverseByte(boot.sectorPerCluster, BS_SECTOR_PER_CLUSTER);
		/* Calculate number of FAT Area sectors */
        uint32_t numberOfFatSectors = fatCopy * sectorPerFAT;
		/* Calculate number of Root Area sectors */
		uint32_t numberOfRootSectors = (numberOfRootDirEntry * RD_NUMBER_OF_BYTES_PER_DOUBLE_LINES) / bytePerSector;
		/* Calculate the first sector position of Data Area */	
		uint32_t firstDataLocation = NUMBER_OF_BOOT_SECTORS + numberOfFatSectors + numberOfRootSectors - 2;
		/* entryOfCLuster is assigned to avoid to change the value of data->startClusNum after loop */
		uint32_t entryOfCLuster = data->startClusNum;
		EntryFormat dir;
		uint32_t nextClusterPosition = 0;
		printf("\n\n");
		printf("\t|Name|\t\t|Type|\t\t|Size|\t\t|Date|\t\t|Time|\n");
		printf("\n");
		do {
			/* Calculate the first sector's position of cluster */
			uint32_t firstFolderPosition = firstDataLocation + entryOfCLuster;
			/* Calculte the first byte's position of cluster */
			uint32_t firstClusterPosition = firstFolderPosition * bytePerSector;
			/* Move the file's poiter to cluster's position */
			fseek(f, firstClusterPosition, 1);
			
			uint32_t i = 0;
			/* Check elements of root, if it's folder location of user's input, return it's data */
			for (int i = 0; i < (16 * sectorPerCluster); i++){
				/* Read each entry (32 bytes) */ 
				fread(&dir, sizeof(EntryFormat), 1, f);
				/* If it's a file or folder, print */ 
				if (isFileOrFolder(dir)){
					number++;
					/* Get name of file and root directory */
					printf("\t%d. ", number);
					int j = 0;
					for (j = 0; j < RD_FILE_NAME; j++)
					{
						printf("%c", dir.fileName[j]);	
					}
					printf("\t");
					/* If it's a file, print the extension */
					if (dir.fileAttributes[0] != ATT_DIRECTORY)
					{
						printf("File ");
						for (j = 0; j < RD_FILENAME_EXTENSION; j++)
						{
							printf("%c", dir.fileNameExtension[j]);
						}
						printf("\t");
					}
					if (dir.fileAttributes[0] == ATT_DIRECTORY)
					{
						printf("File Folder");
						printf("\t");
						}
					/* Get size of file */
					if (dir.fileAttributes[0] != ATT_DIRECTORY)
					{
						
						printf("%d bytes", reverseByte(dir.fileSize, RD_FILE_SIZE));
						printf("\t");
					}
					if (dir.fileAttributes[0] == ATT_DIRECTORY)
					{
						
						printf("\t\t");
				}
					/* Get date of file */
	        		uint32_t DateAfterReverse = reverseByte(dir.fileDate, RD_FILE_DATE);
	        		uint16_t year = CALC_YEAR(DateAfterReverse);
	        		uint16_t month = CALC_MONTH(DateAfterReverse);
	        		uint16_t day = CALC_DAY(DateAfterReverse);
	 
	                printf("%02d/%02d/%04d\t", day, month, year + BIN_YEAR_OFFSET_FROM);
	                
	                /* Get time of file */
					uint32_t TimeAfterReverse = reverseByte(dir.fileTime, RD_FILE_TIME);
					uint16_t hours = CALC_HOURS(TimeAfterReverse);
					uint16_t minutes = CALC_MINUTES(TimeAfterReverse);
					uint16_t seconds = CALC_SECONDS(TimeAfterReverse);

					printf("%02d:%02d:%02d", hours, minutes, seconds);
					printf(" \n");
				}	
			}
			/* Determine the next cluster */
			/* In FAT16 Type, every 2 bytes represent 1 entries */
			/* Calculate the offset of next cluster's entry in FAT Area */
			nextClusterPosition = 2 * entryOfCLuster + bytePerSector;
			if (nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector)){
				/* Move the file's poiter to the position */
				rewind(f);
				fseek(f, nextClusterPosition, 1);
				/* Read 2 bytes */
				uint8_t entry[2];
				fread(entry, sizeof(entry), 1, f);
				/* Reverse data */
				uint32_t nextClusterEntry = reverseByte(entry, 2);
				/* Assign data to entryOfCLuster */
				entryOfCLuster = nextClusterEntry;
			}
		}
		while ((entryOfCLuster != FAT16_ENTRY_EOC) && nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector));
        fclose(f);
    }
	return number;
}

/* This function is to read sub directory in FAT32 files */
uint32_t readSubDirectory32(const char *filePath, DataTypes *data){
	/* SHALL BE DEVELOPED IN THE FUTURE */
	return 0;
}

/* This function is to get data from sub directory to DataTypes in FAT12 files */
DataTypes getSubData12(const char *filePath, DataTypes *data, uint32_t index) {
	DataTypes newData;
	/* Read Boot */
	FAT12BootTypes boot = readBootSector12(filePath);
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
		uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
		uint32_t sectorPerCluster = reverseByte(boot.sectorPerCluster, BS_SECTOR_PER_CLUSTER);
		/* Calculate number of FAT Area sectors */
        uint32_t numberOfFatSectors = fatCopy * sectorPerFAT;
		/* Calculate number of Root Area sectors */
		uint32_t numberOfRootSectors = (numberOfRootDirEntry * RD_NUMBER_OF_BYTES_PER_DOUBLE_LINES) / bytePerSector;
		/* Calculate the first sector position of Data Area */	
		uint32_t firstDataLocation = NUMBER_OF_BOOT_SECTORS + numberOfFatSectors + numberOfRootSectors - 2;
		/* entryOfCLuster is assigned to avoid to change the value of data->startClusNum after loop */
		uint32_t entryOfCLuster = data->startClusNum;
		EntryFormat dir;
		uint32_t count = 0, number = 0;
		do {
			/* Calculate the first sector's position of cluster */
			uint32_t firstSectorOfFilePosition = firstDataLocation + entryOfCLuster;
			/* Calculte the first byte's position of cluster */
			uint32_t firstclusterPosition = firstSectorOfFilePosition * bytePerSector;
			/* Move the file's poiter to cluster's position */
			fseek(f, firstclusterPosition, 1);
			/* Check elements of 1 cluster, if it's folder location of user's input, return it's data */
			uint32_t i = 0;
			while ((i < (16 * sectorPerCluster)) && (number != index)){
				/* Read each entry(32 bytes) */ 
				fread(&dir, sizeof(EntryFormat), 1, f);
				/* If it's a file or folder, print */ 
				if (isFileOrFolder(dir)){
					number++;
					if (number == index){
						newData.fileAttributes = reverseByte(dir.fileAttributes, RD_FILE_ATTRIBUTES);
						newData.startClusNum = reverseByte(dir.startClusNum, RD_STARTING_CLUSTER_NUMBER);
						newData.fileSize = reverseByte(dir.fileSize, RD_FILE_SIZE);
					}	
				}
				i++;
			}
			/* Determine the next cluster */
			/* In FAT12 Type, every 3 bytes represent 2 entries */
			/* Calculate the offset of next cluster's entry in FAT Area */
			uint32_t nextClusterOffset = (uint32_t)(1.5 * entryOfCLuster);
			uint32_t nextClusterPosition = 0;
			uint8_t doubleEntry[3] = {0};
			uint32_t nextClusterEntry = 0;
			/* In case nextClusterOffset is not divisible by 3, nextClusterEntry is at the middle byte's position, 
			 * and we will get the second entry */
			if ((nextClusterOffset % 3) != 0) {
				/* Calculate the first position of 3 bytes */
				nextClusterPosition = nextClusterOffset - 1 + bytePerSector;
				/* Move the file's poiter to the position */
				rewind(f);
				fseek(f, nextClusterPosition, 1);
				/* Read 3 bytes */
				fread(doubleEntry, sizeof(doubleEntry), 1, f);
				/* Get the second entry */
				nextClusterEntry = getEntryFat12(doubleEntry, 2);
			}
			/* In case nextClusterOffset is divisible by 3, nextClusterEntry is at the first byte's position, 
			 * and we will get the first entry */
			else {
				/* Calculate the first position of 3 bytes */
				nextClusterPosition = nextClusterOffset + bytePerSector;
				/* Move the file's poiter to the position */
				rewind(f);
				fseek(f, nextClusterPosition, 1);
				/* Read 3 bytes */
				fread(doubleEntry, sizeof(doubleEntry), 1, f);
				/* Get the second entry */
				nextClusterEntry = getEntryFat12(doubleEntry, 1);
			}
			/* Assign data to entryOfCLuster */
			entryOfCLuster = nextClusterEntry;
			count++;
		}
		while ((entryOfCLuster != FAT12_ENTRY_EOC) && (number != index));
        fclose(f);
		return newData;
    }
}

/* This function is to get data from sub directory to DataTypes in FAT16 files */
DataTypes getSubData16(const char *filePath, DataTypes *data, uint32_t index) {
	DataTypes newData;
	/* Read Boot */
	FAT16BootTypes boot = readBootSector16(filePath);
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
		uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
		uint32_t sectorPerCluster = reverseByte(boot.sectorPerCluster, BS_SECTOR_PER_CLUSTER);
		/* Calculate number of FAT Area sectors */
        uint32_t numberOfFatSectors = fatCopy * sectorPerFAT;
		/* Calculate number of Root Area sectors */
		uint32_t numberOfRootSectors = (numberOfRootDirEntry * RD_NUMBER_OF_BYTES_PER_DOUBLE_LINES) / bytePerSector;
		/* Calculate the first sector position of Data Area */	
		uint32_t firstDataLocation = NUMBER_OF_BOOT_SECTORS + numberOfFatSectors + numberOfRootSectors - 2;
		/* entryOfCLuster is assigned to avoid to change the value of data->startClusNum after loop */
		uint32_t entryOfCLuster = data->startClusNum;
		EntryFormat dir;
		uint32_t number = 0;
		do {
			/* Calculate the first sector's position of cluster */
			uint32_t firstSectorPosition = firstDataLocation + entryOfCLuster;
			/* Calculte the first byte's position of cluster */
			uint32_t firstClusterPosition = firstSectorPosition * bytePerSector;
			/* Move the file's poiter to cluster's position */
			fseek(f, firstClusterPosition, 1);
			/* Check elements of 1 cluster, if it's folder location of user's input, return it's data */
			uint32_t i = 0;
			while ((i < (16 * sectorPerCluster)) && (number != index)){
				/* Read each entry(32 bytes) */ 
				fread(&dir, sizeof(EntryFormat), 1, f);
				/* If it's a file or folder, print */ 
				if (isFileOrFolder(dir)){
					number++;
					if (number == index){
						newData.fileAttributes = reverseByte(dir.fileAttributes, RD_FILE_ATTRIBUTES);
						newData.startClusNum = reverseByte(dir.startClusNum, RD_STARTING_CLUSTER_NUMBER);
						newData.fileSize = reverseByte(dir.fileSize, RD_FILE_SIZE);
					}	
				}
				i++;
			}
			/* Determine the next cluster */
			/* In FAT16 Type, every 2 bytes represent 1 entries */
			/* Calculate the offset of next cluster's entry in FAT Area */
			uint32_t nextClusterPosition = 2 * entryOfCLuster + bytePerSector;
			/* Move the file's poiter to the position */
			rewind(f);
			fseek(f, nextClusterPosition, 1);
			/* Read 2 bytes */
			uint8_t entry[2];
			fread(entry, sizeof(entry), 1, f);
			/* Reverse data */
			uint32_t nextClusterEntry = reverseByte(entry, 2);
			/* Assign data to entryOfCLuster */
			entryOfCLuster = nextClusterEntry;
		}
		while ((entryOfCLuster != FAT16_ENTRY_EOC) && (number != index));
        fclose(f);
		return newData;
    }
}

/* This function is to get data from sub directory to DataTypes in FAT32 files */
DataTypes getSubData32(const char *filePath, DataTypes *data, uint32_t index) {
	/* SHALL BE DEVELOPED IN THE FUTURE */
}

/* This function is to read a file in FAT12 files */
uint32_t readFile12(const char *filePath, DataTypes *data) {
	/* Read Boot sector */
	FAT12BootTypes boot = readBootSector12(filePath);
	/* Open file */
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
		uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR);
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
		uint32_t sectorPerCluster = reverseByte(boot.sectorPerCluster, BS_SECTOR_PER_CLUSTER);
		/* Calculate the number of FAT Area sectors */
        uint32_t numberOfFatSectors = fatCopy * sectorPerFAT;
		/* Calculate the number of Root Area sectors */
		uint32_t numberOfRootSectors = (numberOfRootDirEntry * RD_NUMBER_OF_BYTES_PER_DOUBLE_LINES) / bytePerSector;
		/* Calculate the position of the first Data Area sector */
		uint32_t firstDataLocation = NUMBER_OF_BOOT_SECTORS + numberOfFatSectors + numberOfRootSectors - 2;
		/* entryOfCLuster is assigned to avoid to change the value of data->startClusNum after loop */
		uint32_t entryOfCLuster = data->startClusNum;
		uint32_t count = 0;
		uint32_t nextClusterPosition = 0;
		do {
			/* Calculate the first sector's position of cluster */
			uint32_t firstSectorPosition = firstDataLocation + entryOfCLuster;
			/* Calculte the first byte's position of cluster */
			uint32_t firstClusterPosition = firstSectorPosition * bytePerSector;
			/* Move the file's poiter to cluster's position */
			fseek(f, firstClusterPosition, 1);
			/* Print all sectors in cluster */
			uint8_t text[bytePerSector];
			uint32_t i = 0;
			for (i = 0; i < sectorPerCluster; i++) {
				fread(text, sizeof(text), 1, f);
				printf("\n\t%s", text);
			}
			/* Determine the next cluster */
			/* In FAT12 Type, every 3 bytes represent 2 entries */
			/* Calculate the offset of next cluster's entry in FAT Area */
			uint32_t nextClusterOffset = (uint32_t)(1.5 * entryOfCLuster);
			uint8_t doubleEntry[3] = {0};
			uint32_t nextClusterEntry = 0;
			/* In case nextClusterOffset is not divisible by 3, nextClusterEntry is at the middle byte's position, 
			 * and we will get the second entry */
			if ((nextClusterOffset % 3) != 0) {
				/* Calculate the first position of 3 bytes */
				nextClusterPosition = nextClusterOffset - 1 + bytePerSector;
				if (nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector)) {
					/* Move the file's poiter to the position */
					rewind(f);
					fseek(f, nextClusterPosition, 1);
					/* Read 3 bytes */
					fread(doubleEntry, sizeof(doubleEntry), 1, f);
					/* Get the second entry */
					nextClusterEntry = getEntryFat12(doubleEntry, 2);
				}
			}
			/* In case nextClusterOffset is divisible by 3, nextClusterEntry is at the first byte's position, 
			 * and we will get the first entry */
			else {
				/* Calculate the first position of 3 bytes */
				nextClusterPosition = nextClusterOffset + bytePerSector;
				if (nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector)) {
					/* Move the file's poiter to the position */
					rewind(f);
					fseek(f, nextClusterPosition, 1);
					/* Read 3 bytes */
					fread(doubleEntry, sizeof(doubleEntry), 1, f);
					/* Get the second entry */
					nextClusterEntry = getEntryFat12(doubleEntry, 1);
				}
				
			}
			/* Assign data to entryOfCLuster */
			entryOfCLuster = nextClusterEntry;
		}
		while ((entryOfCLuster != FAT12_ENTRY_EOC) && nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector));
        fclose(f);
    }
	return 0;
}

/* This function is to read a file in FAT16 files */
uint32_t readFile16(const char *filePath, DataTypes *data) {
	/* Read Boot sector */
	FAT16BootTypes boot = readBootSector16(filePath);
	/* Open file */
    FILE *f = fopen(filePath, "rb");
    if (f == NULL){
        printf("[ERROR] Cannot open file\n");
    }
    else{
		uint32_t fatCopy = reverseByte(boot.fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot.sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot.bytePerSector, BS_BYTE_PER_SECTOR); 
        uint32_t numberOfRootDirEntry = reverseByte(boot.rdetEntry, BS_ROOT_DIR_ENTRY);
		uint32_t sectorPerCluster = reverseByte(boot.sectorPerCluster, BS_SECTOR_PER_CLUSTER);
		/* Calculate the number of FAT Area sectors */
        uint32_t numberOfFatSectors = fatCopy * sectorPerFAT;
		/* Calculate the number of Root Area sectors */
		uint32_t numberOfRootSectors = (numberOfRootDirEntry * RD_NUMBER_OF_BYTES_PER_DOUBLE_LINES) / bytePerSector;
		/* Calculate the position of the first Data Area sector */
		uint32_t firstDataLocation = NUMBER_OF_BOOT_SECTORS + numberOfFatSectors + numberOfRootSectors - 2;
		/* entryOfCLuster is assigned to avoid to change the value of data->startClusNum after loop */
		uint32_t entryOfCLuster = data->startClusNum;
		uint32_t nextClusterPosition = 0;
		do {
			/* Calculate the first sector's position of cluster */
			uint32_t firstSectorPosition = firstDataLocation + entryOfCLuster;
			/* Calculte the first byte's position of cluster */
			uint32_t firstClusterPosition = firstSectorPosition * bytePerSector;
			/* Move the file's poiter to cluster's position */
			fseek(f, firstClusterPosition, 1);
			/* Print all sectors in cluster */
			uint8_t text[bytePerSector];
			uint32_t i = 0;
			for (i = 0; i < sectorPerCluster; i++) {
				fread(text, sizeof(text), 1, f);
				printf("\n\t%s", text);
			}
			/* Determine the next cluster */
			/* In FAT16 Type, every 2 bytes represent 1 entries */
			/* Calculate the offset of next cluster's entry in FAT Area */
			nextClusterPosition = (2 * entryOfCLuster) + bytePerSector;
			if (nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector)) {
				/* Move the file's poiter to the position */
				rewind(f);
				fseek(f, nextClusterPosition, 1);
				/* Read 2 bytes */
				uint8_t entry[2];
				fread(entry, sizeof(entry), 1, f);
				/* Reverse data */
				uint32_t nextClusterEntry = reverseByte(entry, 2);
				/* Assign data to entryOfCLuster */
				entryOfCLuster = nextClusterEntry;
			}
		}
		while ((entryOfCLuster != FAT16_ENTRY_EOC) && nextClusterPosition < ((numberOfFatSectors + 1) * bytePerSector));
        fclose(f);
    }
	return 0;
}

uint32_t readFile32(const char *filePath, DataTypes *data) {
	/* SHALL BE DEVELOPED IN THE FUTURE */
	return 0;
}

/* This function is to check whether an element is a foler */
Bool isFolder(DataTypes data) {
	Bool result = FALSE;
	if (data.fileAttributes == ATT_DIRECTORY){
		result = TRUE;
	} 
	return result;
}

/* This function is to check the first byte of file name */
static Bool checkFileName(EntryFormat root) 
{
	Bool result = TRUE;
	switch(root.fileName[0])
	{
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

static Bool isFileOrFolder(EntryFormat entry){
	Bool result = TRUE;
	if (checkFileName(entry)){
		switch(entry.fileAttributes[0]){
			case ATT_NORMAL_FILE:
				result = TRUE;
				break;
			case ATT_READ_ONLY:
				result = TRUE;
				break;
			case ATT_HIDDEN_FILE:
				result = TRUE;
				break;
			case ATT_SYSTEM_FILE:
				result = TRUE;
				break;
			case ATT_DIRECTORY:
				result = TRUE;
				break;
			default:
				result = FALSE;
				break;
		}
	}
	else {
		result = FALSE;
	}
	return result;
}

/* This function is to reverse bytes */
uint32_t reverseByte(uint8_t *byte, uint32_t count){
    uint32_t result = 0;
    for (int i = count-1; i >= 0; i--){
        result = (result << 8) | byte[i];
    }
    return result;
} 

/* This function is to get an entry in FAT12 sectors
 * byte is an array of 3 bytes which represents 2 entries
 * position = 1: return the first entry
 * position = 2: return the second entry */
uint32_t getEntryFat12(uint8_t *byte, uint32_t position) {
	uint32_t result = 0;
	for (int i = 2; i >= 0; i--){
        result = (result << 8) | byte[i];
    }
	if (position == 1) {
		result = result & 0xFFF;
	}
	if (position == 2) {
		result = result >> 12;
	}
    return result;
}

/* This function is to clear space in a string */
static void removeSpace(uint8_t *str) {
	uint32_t pos = 0, i = 0;
	uint32_t len = strlen(str);
	for(i = 0; i < len; i++)
	{
		if ((str[i] != '\r') && (str[i] != '\n') && (str[i] != ' ')) 
		{
			str[pos] = str[i];
			pos++;
		}
	}
	str[pos] = '\0';
}
