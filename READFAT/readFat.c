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

uint32_t AdressOfClusterDataFile[225] = {0};
uint32_t FileTime[225] = {0};
uint32_t FileDate[225] = {0};
uint32_t SizeOfFileInRoot[225] = {0};

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
        
        printf("FILE *f init %x\n", f);
        // fseek(f, firstRootLocation, 1);
        fseek(f, 0x2600, 1);
        printf("FILE *f after fseek %x\n", f);

        // for (int k = 0; k < 0x2600; k++)
        // {
        //     fgetc(f);
        // }


        int count  = 0;

        for (int i = 0; i < 20; i++)//numberOfRootDirEntry
        {           

            printf("FILE *f = %x\n", f);
            fread(rootSector, sizeof(FAT12RootTypes), 1, f);


            if ((ATT_LONG_FILE_NAME != (rootSector->fileAttributes[0])) && 0x00 != (rootSector->fileName[0]))
            {

            	/* get name of file and root directory */
                printf("[INFO] Root name: ");
                for (int j = 0; j < RD_FILE_NAME; j++)
                {
                    printf("%c", rootSector->fileName[j]);
                }
                printf("\n");
                
                /* find adress of file or directory from Data Sectors*/
                AdressOfClusterDataFile[count] = reverseByte(rootSector->startingClusterNumber, RD_STARTING_CLUSTER_NUMBER);

                /* find filetime*/
                FileTime[count] = reverseByte(rootSector->fileTime, RD_FILE_TIME);

                /* find filedate*/
                FileDate[count] = reverseByte(rootSector->fileDate, RD_FILE_DATE);

                /* find siez of file in root*/
                SizeOfFileInRoot[count] = reverseByte(rootSector->fileSize, RD_FILE_SIZE);
            
        	    printf("[INFO] Adress of data: %x \t", AdressOfClusterDataFile[count]);
        		printf("[INFO] File time: %d \t", FileTime[count]);
        		printf("[INFO] File date: %d \t", FileDate[count]);
        		printf("[INFO] Size of file: %d \n", SizeOfFileInRoot[count]);
                // printf("%d ", FileTime);
                count++;

            }	

        }
        printf("\n\n\n");
        // printf("%d", FileTime);
        for (int i = 0 ; i < count ; i++)
        {
                printf("[INFO] Adress of data: %x \t", AdressOfClusterDataFile[i]);
                printf("[INFO] File time: %d \t", FileTime[i]);
                printf("[INFO] File date: %d \t", FileDate[i]);
                printf("[INFO] Size of file: %d \n", SizeOfFileInRoot[i]);
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

FAT12RootTypes *ReadFile(const char *filePath, FAT12BootTypes *boot)
{	

	uint32_t firstDataLocation;
	uint32_t AdressOfClusterDataFile;
	uint32_t startingOfByteDataFile;
	uint32_t SizeOfFileInRoot;
	
	FAT12RootTypes *rootSector = (FAT12RootTypes *)malloc(sizeof(FAT12RootTypes));
	
	
    FILE *f = fopen(filePath, "rb");
    
    fread(rootSector, sizeof(FAT12RootTypes), 1, f);
    if (f == NULL)
    {
        printf("[ERROR] Cannot open file\n");
    }
    else
	{	
	    firstDataLocation = (1 + (fatCopy) * (sectorPerFAT) ) * bytePerSector + numberOfRootDirEntry * RD_BYTES_OF_A_ENTRY - (2 * BS_BYTE_PER_SECTOR);
	     
    	AdressOfClusterDataFile = reverseByte(rootSector->startingClusterNumber, RD_STARTING_CLUSTER_NUMBER);
    	SizeOfFileInRoot = reverseByte(rootSector->fileSize, RD_FILE_SIZE);
   		startingOfByteDataFile = firstDataLocation + AdressOfClusterDataFile * BS_BYTE_PER_SECTOR;
   		printf("\nAdressOfClusterDataFile: %x\n", AdressOfClusterDataFile);
   		printf("\nstartingOfByteDataFile: %x\n", startingOfByteDataFile);
   		
   		printf("size of file: %d\n", SizeOfFileInRoot);
   		fclose(f);
   		printf("%x\n", AdressOfClusterDataFile);
   		FILE *f = fopen(filePath, "rb");
   		fseek(f, 0x4200, 1);
   		
   		char dataFile1[SizeOfFileInRoot];
   		for (int i = 0; i < SizeOfFileInRoot; i++)
   		{
   			dataFile1[i] = fgetc(f);	
		}
		for (int i = 0; i < SizeOfFileInRoot; i++)
		{
			printf("%c", dataFile1[i]);
		}
		fclose(f);
	}
	

	
}

