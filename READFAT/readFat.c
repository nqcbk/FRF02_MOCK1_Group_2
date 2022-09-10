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
            /*SOS check offset of FAT 32 later*/ 
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

/* read boot sector for FAT 16*/
FAT16BootTypes *readBootSector16(const char *filePath)
{
    FAT16BootTypes *bootSector = (FAT16BootTypes *)malloc(sizeof(FAT16BootTypes));
    FILE *f = fopen(filePath, "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
        fread(bootSector, sizeof(FAT16BootTypes), 1, f);
        fclose(f);

    }
    return bootSector;
}

/* read boot sector for FAT 32*/
FAT32BootTypes *readBootSector32(const char *filePath)
{
    FAT32BootTypes *bootSector = (FAT32BootTypes *)malloc(sizeof(FAT32BootTypes));

    FILE *f = fopen(filePath, "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
        fread(bootSector, sizeof(FAT32BootTypes), 1, f);
        fclose(f);

    }
    return bootSector;
} /*READ_BOOT_SECTOR_32*/



/* Define Global Values*/
uint32_t fatCopy;
uint32_t sectorPerFAT;
uint32_t bytePerSector;
uint32_t numberOfRootDirEntry;
uint32_t rdetEntry;
uint32_t firstRootLocation;

uint32_t AdressOfClusterDataFile[224] = {0};
uint32_t FileTime[224] = {0};
uint32_t FileDate[224] = {0};
uint32_t SizeOfFileInRoot[224] = {0};

uint32_t count  = 0;
FAT12RootTypes *readRootDirectory12(const char *filePath, FAT12BootTypes *boot)
{
    FAT12RootTypes *rootSector = (FAT12RootTypes *)malloc(sizeof(FAT12RootTypes));

    FILE *f = fopen(filePath, "rb");

    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {

        /* calculate the first byte of root directory location */
        fatCopy = reverseByte(boot->fatCopy, BS_FAT_COPY);
        sectorPerFAT = reverseByte(boot->sectorPerFAT, BS_SECTOR_PER_FAT);
        bytePerSector = reverseByte(boot->bytePerSector, BS_BYTE_PER_SECTOR);
		
        firstRootLocation = (1 + (fatCopy) * (sectorPerFAT) ) * bytePerSector;
 
        numberOfRootDirEntry = reverseByte(boot->rdetEntry, BS_ROOT_DIR_ENTRY);
        
        fseek(f, firstRootLocation, 1);

        for (int i = 0; i < numberOfRootDirEntry; i++)
        {           

            fread(rootSector, sizeof(FAT12RootTypes), 1, f);


            if ((ATT_LONG_FILE_NAME != (rootSector->fileAttributes[0])) && 0x00 != (rootSector->fileName[0]))
            {

            	/* get name of file and root directory */
                printf("[INFO] Root name: ");
                for (int j = 0; j < RD_FILE_NAME; j++)
                {
                    printf("%c", rootSector->fileName[j]);
                }
                printf(" \n");
                
                /* Find File Name Extension*/

                /* find adress of file or directory from Data Sectors*/
                AdressOfClusterDataFile[count] = reverseByte(rootSector->startingClusterNumber, RD_STARTING_CLUSTER_NUMBER);

                /* find filetime*/
                FileTime[count] = reverseByte(rootSector->fileTime, RD_FILE_TIME);

                /* find filedate*/
                FileDate[count] = reverseByte(rootSector->fileDate, RD_FILE_DATE);

                /* find siez of file in root*/
                SizeOfFileInRoot[count] = reverseByte(rootSector->fileSize, RD_FILE_SIZE);

                count++;
            }	
        }

        fclose(f);
    }
    return rootSector;
}


/* reverse byte */
uint32_t reverseByte(uint8_t *byte, uint32_t count)
{
    uint32_t result = 0;
    for (int i = count-1; i >= 0; i--)
    {
        result = (result << 8) | byte[i];
    }
    return result;
} 

uint32_t firstDataLocation;
uint32_t startingOfByteDataFile;

FAT12RootTypes *ReadFile(const char *filePath, FAT12BootTypes *boot)
{	

    FILE *f = fopen(filePath, "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
	{	
        printf("count: %d ", count);
        for(int i = 0; i < count; i++)
        {
            printf("adress: %d\n", AdressOfClusterDataFile[i]);
        }
	    firstDataLocation = (1 + (fatCopy) * (sectorPerFAT) ) * bytePerSector + numberOfRootDirEntry * RD_BYTES_OF_A_ENTRY - (2 * bytePerSector);

	    startingOfByteDataFile = firstDataLocation + AdressOfClusterDataFile[1] * bytePerSector;

   		fclose(f);

   		FILE *f = fopen(filePath, "rb");
   		fseek(f, startingOfByteDataFile, 1);
   		
   		char dataFile1[SizeOfFileInRoot[1]];
   		for (int i = 0; i < SizeOfFileInRoot[1]; i++)
   		{
   			dataFile1[i] = fgetc(f);	
		}
		for (int i = 0; i < SizeOfFileInRoot[1]; i++)
		{
			printf("%c", dataFile1[i]);
		}
        printf("\n");
		fclose(f);
	}	
}

uint32_t startingOfSubDirLocation;

FAT12RootTypes *ReadSubDirectory(const char *filePath, FAT12BootTypes *boot)
{

    FAT12RootTypes *subDirSector = (FAT12RootTypes *)malloc(sizeof(FAT12RootTypes));

    FILE *f = fopen(filePath, "rb");

    
    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
    {
        
        startingOfSubDirLocation = firstDataLocation + AdressOfClusterDataFile[5] * bytePerSector;
        printf("startingOfSubDirLocation : %x \n", startingOfSubDirLocation);
        fseek(f, startingOfSubDirLocation, 1);

        for (int i = 0; i < 5; i++)
        {           
            fread(subDirSector, sizeof(FAT12RootTypes), 1, f);  

            if ((0x00 != (subDirSector->fileName[0])) && (ATT_LONG_FILE_NAME != (subDirSector->fileAttributes[0])) && (0x2e != (subDirSector->fileName[0])))
            {
                printf("[INFO] Sub name: ");
                for (int j = 0; j < RD_FILE_NAME; j++)
                {
                    printf("%c", subDirSector->fileName[j]);
                }
                printf("\n");
            }

        }
        fclose(f);
    }
}
