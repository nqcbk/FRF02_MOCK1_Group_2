#ifndef _READFAT_H_
#define _READFAT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_BOOT_SECTORS						1



#define BS_BOOT_STRAP_JUMP                          3 
#define BS_OEM                                      8
#define BS_BYTE_PER_SECTOR                          2
#define BS_SECTOR_PER_CLUSTER                       1
#define BS_RESERVED_SECTOR                          2
#define BS_FAT_COPY                                 1
#define BS_ROOT_DIR_ENTRY                           2
#define BS_TOTAL_SECTOR_IN_FILE_SYSTEM              2
#define BS_MEDIA_TYPE                               1
#define BS_SECTOR_PER_FAT                           2
#define BS_SECTOR_PER_TRACK                         2
#define BS_HEAD                                     2
#define BS_HIDDEN_SECTOR                            4
#define BS_TOTAL_BLOCK                              4
#define BS_PHYSICAL_DRIVE_NUMBER                    2
#define BS_EXTENDED_BOOT_SIGNATURE                  1
#define BS_VOLUME_SERIAL_NUMBER                     4
#define BS_VOLUME_LABEL                             11
#define BS_NAME_OF_FAT                              8
#define BS_BOOT_STRAP                               448
#define BS_SIGNATURE                                2

typedef enum Bool
{
    FALSE,
    TRUE
} Bool;

/* Define FAT type */
typedef enum FAT_TYPES
{
    ERROR,
    FAT12, 
    FAT16, 
    FAT32 
} FatTypes;


/* struct for FAT12 boot block*/
typedef struct FAT12_BOOT
{
    /* data */
    uint8_t bootstrapJump[BS_BOOT_STRAP_JUMP];                  /* 0-2     code to jump to bootstrap */
    uint8_t oem[BS_OEM];                                        /* 3-10    OEM name/ version */
    uint8_t bytePerSector[BS_BYTE_PER_SECTOR];                  /* 11-12   byte per sector */
    uint8_t sectorPerCluster[BS_SECTOR_PER_CLUSTER];            /* 13      serctor per cluster */
    uint8_t reservedSector[BS_RESERVED_SECTOR];                 /* 14-15   number of reserved sectors */
    uint8_t fatCopy[BS_FAT_COPY];                               /* 16      number of FAT copies */
    uint8_t rdetEntry[BS_ROOT_DIR_ENTRY];                       /* 17-18   number of root directory entries*/
    uint8_t sectorInFS[BS_TOTAL_SECTOR_IN_FILE_SYSTEM];         /* 19-20   total of number sector in filesystem*/
    uint8_t mediaType[BS_MEDIA_TYPE];                           /* 21      media descriptior type*/
    uint8_t sectorPerFAT[BS_SECTOR_PER_FAT];                    /* 22-23   sector per FAT, 0 for FAT32*/
    uint8_t sectorPerTrack[BS_SECTOR_PER_TRACK];                /* 24-25   sector per track*/
    uint8_t head[BS_HEAD];                                      /* 26-27   number of heads*/
    uint8_t hiddenSector[BS_HIDDEN_SECTOR];                     /* 28-31   number of hidden sectors*/
    uint8_t totalBlock[BS_TOTAL_BLOCK];                         /* 32-35   Total number of blocks in the entire disk*/
    uint8_t physicalDriveNumber[BS_PHYSICAL_DRIVE_NUMBER];      /* 36-37   Physical drive number*/
    uint8_t extendedBootSignature[BS_EXTENDED_BOOT_SIGNATURE];  /* 38      Extended Boot Record Signature*/
    uint8_t volumeSerialNumber[BS_VOLUME_SERIAL_NUMBER];        /* 39-42   	Volume Serial Number*/
    uint8_t volumeLabel[BS_VOLUME_LABEL];                       /* 43-53   Volume Label*/
    uint8_t fileSystemType[BS_NAME_OF_FAT];                     /* 54-61   name of FAT type*/
    uint8_t bootstrap1[BS_BOOT_STRAP];                          /* 62-509  bootstrap code*/
    uint8_t signature[BS_SIGNATURE];                            /* 510-511 signature 0x55 0xaa*/

} FAT12BootTypes;

/* struct for FAT16 boot block*/
typedef struct FAT16_BOOT
{
    /* data */
    uint8_t bootstrapJump[BS_BOOT_STRAP_JUMP];                  /* 0-2     code to jump to bootstrap */
    uint8_t oem[BS_OEM];                                        /* 3-10    OEM name/ version */
    uint8_t bytePerSector[BS_BYTE_PER_SECTOR];                  /* 11-12   byte per sector */
    uint8_t sectorPerCluster[BS_SECTOR_PER_CLUSTER];            /* 13      serctor per cluster */
    uint8_t reservedSector[BS_RESERVED_SECTOR];                 /* 14-15   number of reserved sectors */
    uint8_t fatCopy[BS_FAT_COPY];                               /* 16      number of FAT copies */
    uint8_t rdetEntry[BS_ROOT_DIR_ENTRY];                       /* 17-18   number of root directory entries*/
    uint8_t sectorInFS[BS_TOTAL_SECTOR_IN_FILE_SYSTEM];         /* 19-20   total of number sector in filesystem*/
    uint8_t mediaType[BS_MEDIA_TYPE];                           /* 21      media descriptior type*/
    uint8_t sectorPerFAT[BS_SECTOR_PER_FAT];                    /* 22-23   sector per FAT, 0 for FAT32*/
    uint8_t sectorPerTrack[BS_SECTOR_PER_TRACK];                /* 24-25   sector per track*/
    uint8_t head[BS_HEAD];                                      /* 26-27   number of heads*/
    uint8_t hiddenSector[BS_HIDDEN_SECTOR];                     /* 28-31   number of hidden sectors*/
    uint8_t totalBlock[BS_TOTAL_BLOCK];                         /* 32-35   Total number of blocks in the entire disk*/
    uint8_t physicalDriveNumber[BS_PHYSICAL_DRIVE_NUMBER];      /* 36-37   Physical drive number*/
    uint8_t extendedBootSignature[BS_EXTENDED_BOOT_SIGNATURE];  /* 38      Extended Boot Record Signature*/
    uint8_t volumeSerialNumber[BS_VOLUME_SERIAL_NUMBER];        /* 39-42   Volume Serial Number*/
    uint8_t volumeLabel[BS_VOLUME_LABEL];                       /* 43-53   Volume Label*/
    uint8_t fileSystemType[BS_NAME_OF_FAT];                     /* 54-61   file system type level */
    uint8_t bootstrap1[BS_BOOT_STRAP];                          /* 62-509  bootstrap code*/
    uint8_t signature[BS_SIGNATURE];                            /* 510-511 signature 0x55 0xaa*/

} FAT16BootTypes;

/* struct for FAT32 boot block*/
typedef struct FAT32_BOOT
{
    /* FAT32 Struct */

} FAT32BootTypes;


#define RD_FILE_NAME                               8
#define RD_FILENAME_EXTENSION                      3
#define RD_FILE_ATTRIBUTES                         1
#define RD_RESERVED                                10
#define RD_FILE_TIME                               2
#define RD_FILE_DATE                               2
#define RD_STARTING_CLUSTER_NUMBER                 2
#define RD_FILE_SIZE                               4


/* define file attributes */
#define ATT_READ_ONLY                               0x01
#define ATT_HIDDEN_FILE                             0x02
#define ATT_SYSTEM_FILE                             0x04
#define ATT_VOLUME_LABEL                            0x08
#define ATT_LONG_FILE_NAME                          0x0F
#define ATT_DIRECTORY                               0x10
#define ATT_ARCHIVE_FLAG                            0x20
#define ATT_END_OF_FILE								0xFFFF

#define RD_BYTES_OF_A_ENTRY							32			/*New update*/
/* FAT 12 ROOT */
typedef struct FAT12_ROOT
{
    uint8_t fileName[RD_FILE_NAME];                                      /* 0-7   file name */
    uint8_t fileNameExtension[RD_FILENAME_EXTENSION];                    /* 8-10  file name extension */
    uint8_t fileAttributes[RD_FILE_ATTRIBUTES];                          /* 11    file attributes */
    uint8_t reserved[RD_RESERVED];                                       /* 12-21 reserved */
    uint8_t fileTime[RD_FILE_TIME];                                      /* 22-23 file time */
    uint8_t fileDate[RD_FILE_DATE];                                      /* 24-25 file date */
    uint8_t startingClusterNumber[RD_STARTING_CLUSTER_NUMBER];           /* 26-27 starting cluster number */
    uint8_t fileSize[RD_FILE_SIZE];                                      /* 28-31 File size (0 for directories) */
} FAT12RootTypes;

#define FILENAME_NEVER_USED							0x00
#define FILENAME_DELETED_1							0xE5
#define FILENAME_DELETED_2							0x05
#define FILENAME_SPECIAL_ENTRY						0x2E
		
typedef struct DATA_HAL
{
    uint8_t fileAttributes;
    uint32_t startingClusterNumber; 
    uint8_t fileSize;
} DataHalTypes;


/* check FAT type */
FatTypes checkFatTypes(const char *filePath);

/* read boot sector for FAT 12 */
FAT12BootTypes *readBootSector12(const char *filePath);

// /* read boot sector for FAT 16*/
// FAT16BootTypes *readBootSector16(const char *filePath);

// /* read boot sector for FAT 32 */
// FAT32BootTypes *readBootSector32(const char *filePath);

/* read root directory fat 12*/
uint32_t readRootDirectory12(const char *filePath);

uint32_t readSubDirectory(const char *filePath, DataHalTypes *data);

DataHalTypes getRootData(const char *filePath, uint32_t index);

DataHalTypes getSubData(const char *filePath, DataHalTypes *data, uint32_t index); 

uint32_t readFile(const char *filePath, DataHalTypes *data);

Bool isFolder(DataHalTypes data);

static Bool checkFileName(FAT12RootTypes *root);

uint32_t reverseByte(uint8_t *byte, uint32_t count);

#endif /*_READFAT_H_*/

