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

        // printf("[INFO] FAT name %s\n", fatName);

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
        uint32_t fatCopy = reverseByte(boot->fatCopy, BS_FAT_COPY);
        uint32_t sectorPerFAT = reverseByte(boot->sectorPerFAT, BS_SECTOR_PER_FAT);
        uint32_t bytePerSector = reverseByte(boot->bytePerSector, BS_BYTE_PER_SECTOR);

        uint32_t firstRootLocation = (1 + (fatCopy) * (sectorPerFAT) ) * bytePerSector;

        // printf("\nfirstRootLocation: %x\n", firstRootLocation);

        /* calculate the last of root directory location */
        uint32_t rdetEntry = reverseByte(boot->rdetEntry, BS_ROOT_DIR_ENTRY);

        // uint32_t lastRootLocation = firstRootLocation + rdetEntry * 32;

        // printf("\nlastRootLocation: %x\n", lastRootLocation); 

        // rewind(f);

        fseek(f, firstRootLocation, 1);

        for (int i = 0; i < rdetEntry; i++)
        {
            fread(rootSector, sizeof(FAT12RootTypes), 1, f);

            if ((ATT_LONG_FILE_NAME != (rootSector->fileAttributes[0])) && 0x00 != (rootSector->fileName[0]))
            {
                printf("[INFO] Root name: ");
                for (int j = 0; j < 8; j++)
                {
                    printf("%c", rootSector->fileName[j]);
                }
                printf("\n");
                // fseek(f, 0x32, SEEK_CUR);                
                // fread(rootSector, sizeof(FAT12RootTypes), 1, f);
            }
        }
        fclose(f);
    }
    return rootSector;
}

/* */
// Bool isFile(uint8_t attByte)
// {
//     Bool retValue = TRUE;
//     if (0x10 == attByte)
//     {

//     }
// }


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
// void ReadSubDirectory()
// {

// }
// void ReadFile()
// {

// }