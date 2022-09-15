/*** READ FAT FILE LAYER ***/
#ifndef _READFAT_H_
#define _READFAT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*** Boot Sector */
#define NUMBER_OF_BOOT_SECTORS									1
#define FAT_TYPE_POSITION										0x36		/* Used for FAT12 and FAT16 type */
#define FAT32_FAT_TYPE_POSITION									0x52		/* Only used for FAT32 type */

/** Define Number of Bytes of each elements in Boot Sector */
/* General */
#define BS_BOOT_STRAP_JUMP                          			3 
#define BS_OEM                                      			8
#define BS_BYTE_PER_SECTOR                          			2
#define BS_SECTOR_PER_CLUSTER                       			1
#define BS_RESERVED_SECTOR                          			2
#define BS_FAT_COPY                                 			1
#define BS_ROOT_DIR_ENTRY                           			2
#define BS_TOTAL_SECTOR_IN_FILE_SYSTEM              			2
#define BS_MEDIA_TYPE                               			1
#define BS_SECTOR_PER_FAT                           			2
#define BS_SECTOR_PER_TRACK                         			2
#define BS_HEAD                                     			2
#define BS_HIDDEN_SECTOR                            			4
#define BS_TOTAL_BLOCK                              			4

/* FAT12 and FAT16 */
#define BS_PHYSICAL_DRIVE_NUMBER	                   			2
#define BS_EXTENDED_BOOT_SIGNATURE                  			1
#define BS_VOLUME_SERIAL_NUMBER                     			4
#define BS_VOLUME_LABEL                             			11
#define BS_NAME_OF_FAT                              			8
#define BS_BOOT_STRAP                               			448
#define BS_SIGNATURE                                			2

/* FAT32 */
#define	FAT32_BS_SECTORS_PER_FAT								4
#define FAT32_BS_DRIVE_DESCRIPTION								2
#define FAT32_BS_VERSION										2
#define FAT32_BS_CLUSTER_NUMBER_OF_ROOT_START					4
#define FAT32_BS_SECTOR_NUMBER_FS								2
#define FAT32_BS_FIRST_SECTOR_NUMBER_OF_COPY					2
#define FAT32_BS_RESERVED										12
#define FAT32_BS_PHYSICAL_DRIVE_NUMBER							1
#define FAT32_BS_OTHER_PURPOSES									1
#define FAT32_BS_EXTENDED_BOOT_SIGNATURE						1
#define FAT32_BS_VOLUME_SERIAL_NUMBER							4
#define FAT32_BS_VOLUME_LABEL									11
#define FAT32_BS_NAME_OF_FAT									8
#define FAT32_BS_BOOT_STRAP										420
#define FAT32_BS_SIGNATURE										2

/* Define Struct FAT12 Boot Sector*/
typedef struct FAT12_BOOT{
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
    uint8_t fileSystemType[BS_NAME_OF_FAT];                     /* 54-61   name of FAT type*/
    uint8_t bootstrap1[BS_BOOT_STRAP];                          /* 62-509  bootstrap code*/
    uint8_t signature[BS_SIGNATURE];                            /* 510-511 signature 0x55 0xaa*/
} FAT12BootTypes;

/* Define Struct FAT16 Boot Sector*/
typedef struct FAT16_BOOT{
    uint8_t bootstrapJump[BS_BOOT_STRAP_JUMP];                  /* 0-2     Code to jump to bootstrap */
    uint8_t oem[BS_OEM];                                        /* 3-10    OEM name/ version */
    uint8_t bytePerSector[BS_BYTE_PER_SECTOR];                  /* 11-12   Byte per sector */
    uint8_t sectorPerCluster[BS_SECTOR_PER_CLUSTER];            /* 13      Serctor per cluster */
    uint8_t reservedSector[BS_RESERVED_SECTOR];                 /* 14-15   Number of reserved sectors */
    uint8_t fatCopy[BS_FAT_COPY];                               /* 16      Number of FAT copies */
    uint8_t rdetEntry[BS_ROOT_DIR_ENTRY];                       /* 17-18   Number of root directory entries*/
    uint8_t sectorInFS[BS_TOTAL_SECTOR_IN_FILE_SYSTEM];         /* 19-20   Total of number sector in filesystem*/
    uint8_t mediaType[BS_MEDIA_TYPE];                           /* 21      Media descriptior type*/
    uint8_t sectorPerFAT[BS_SECTOR_PER_FAT];                    /* 22-23   Sector per FAT, 0 for FAT32*/
    uint8_t sectorPerTrack[BS_SECTOR_PER_TRACK];                /* 24-25   Sector per track*/
    uint8_t head[BS_HEAD];                                      /* 26-27   Number of heads*/
    uint8_t hiddenSector[BS_HIDDEN_SECTOR];                     /* 28-31   Number of hidden sectors*/
    uint8_t totalBlock[BS_TOTAL_BLOCK];                         /* 32-35   Total number of blocks in the entire disk*/
    uint8_t physicalDriveNumber[BS_PHYSICAL_DRIVE_NUMBER];      /* 36-37   Physical drive number*/
    uint8_t extendedBootSignature[BS_EXTENDED_BOOT_SIGNATURE];  /* 38      Extended Boot Record Signature*/
    uint8_t volumeSerialNumber[BS_VOLUME_SERIAL_NUMBER];        /* 39-42   Volume Serial Number*/
    uint8_t volumeLabel[BS_VOLUME_LABEL];                       /* 43-53   Volume Label*/
    uint8_t fileSystemType[BS_NAME_OF_FAT];                     /* 54-61   File system type level */
    uint8_t bootstrap[BS_BOOT_STRAP];                      		/* 62-509  Bootstrap code*/
    uint8_t signature[BS_SIGNATURE];                            /* 510-511 Signature 0x55 0xaa*/
} FAT16BootTypes;

/* Define Struct FAT32 Boot Sector */
typedef struct FAT32_BOOT{
    uint8_t bootstrapJump[BS_BOOT_STRAP_JUMP];                  	/* 0-2     Code to jump to bootstrap */
    uint8_t oem[BS_OEM];                                        	/* 3-10    OEM name/ version */
    uint8_t bytePerSector[BS_BYTE_PER_SECTOR];                  	/* 11-12   Byte per sector */
    uint8_t sectorPerCluster[BS_SECTOR_PER_CLUSTER];            	/* 13      Nerctor per cluster */
    uint8_t reservedSector[BS_RESERVED_SECTOR];                		/* 14-15   Number of reserved sectors */
    uint8_t fatCopy[BS_FAT_COPY];                               	/* 16      Number of FAT copies  */
    uint8_t rdetEntry[BS_ROOT_DIR_ENTRY];                       	/* 17-18   Number of root directory entries */
    uint8_t sectorInFS[BS_TOTAL_SECTOR_IN_FILE_SYSTEM];         	/* 19-20   Total of number sector in filesystem */
    uint8_t mediaType[BS_MEDIA_TYPE];                           	/* 21      Media descriptior type */
    uint8_t sectorPerFAT[BS_SECTOR_PER_FAT];                    	/* 22-23   Sector per FAT, 0 for FAT32 */
    uint8_t sectorPerTrack[BS_SECTOR_PER_TRACK];                	/* 24-25   Sector per track */
    uint8_t head[BS_HEAD];                                      	/* 26-27   Number of heads */
    uint8_t hiddenSector[BS_HIDDEN_SECTOR];                     	/* 28-31   Number of hidden sectors */
    uint8_t totalBlock[BS_TOTAL_BLOCK];                         	/* 32-35   Total number of blocks in the entire disk */
	uint8_t sectorsPerFAT[FAT32_BS_SECTORS_PER_FAT];				/* 36-39   Logical sectors per FAT */	
	uint8_t driveDescription[FAT32_BS_DRIVE_DESCRIPTION];			/* 40-41   Drive description */
	uint8_t	version[FAT32_BS_VERSION];								/* 42-43   Version */
	uint8_t	clusNumRootStart[FAT32_BS_CLUSTER_NUMBER_OF_ROOT_START];/* 44-47   Cluster number of root start */
	uint8_t	sectorNumOfFS[FAT32_BS_SECTOR_NUMBER_FS];				/* 48-49   Logical sector number of FS Information Sector*/
	uint8_t	firstSecNumOfCopy[FAT32_BS_FIRST_SECTOR_NUMBER_OF_COPY];/* 50-51   First logical sector number of a copy of the three FAT32 boot sectors */
	uint8_t	reversed[FAT32_BS_RESERVED];							/* 52-63   Reserved */
	uint8_t	physicalDriveNumber[FAT32_BS_PHYSICAL_DRIVE_NUMBER];	/* 64      Physical Drive Number */
	uint8_t	otherPurposes[FAT32_BS_OTHER_PURPOSES];					/* 65      Used for various purposes */
	uint8_t extendedBootSig[FAT32_BS_EXTENDED_BOOT_SIGNATURE];		/* 66      Extended boot signature */
	uint8_t volumeSerialNumber[FAT32_BS_VOLUME_SERIAL_NUMBER];		/* 67-70   Volume Serial Number */
	uint8_t volumeLabel[FAT32_BS_VOLUME_LABEL];						/* 71-81   Volume Label */
	uint8_t nameOfFat[FAT32_BS_NAME_OF_FAT];						/* 82-89   File system type */
	uint8_t bootstrap[FAT32_BS_BOOT_STRAP];                      	/* 90-509  Bootstrap code */
    uint8_t signature[FAT32_BS_SIGNATURE];                          /* 510-511 Signature 0x55 0xaa */
} FAT32BootTypes;


/** Define FAT sector*/
/* FAT12 */
#define FAT12_ENTRY_EOC							    0xFFF	/* Entry end of chain for directory or file in FAT12 */

/* FAT16 */
#define FAT16_ENTRY_EOC							    0xFFFF	/* Entry end of chain for directory or file in FAT16 */

/** Define Root Directory */
#define RD_NUMBER_OF_BYTES_PER_DOUBLE_LINES			32
#define RD_FILE_NAME                               	8
#define RD_FILENAME_EXTENSION                      	3
#define RD_FILE_ATTRIBUTES                         	1
#define RD_RESERVED                                	10
#define RD_FILE_TIME                               	2
#define RD_FILE_DATE                				2
#define RD_STARTING_CLUSTER_NUMBER                 	2
#define RD_FILE_SIZE                              	4

/* Define Struct Entry Format */
typedef struct ENTRY_FORMAT {
    uint8_t fileName[RD_FILE_NAME];                                      /* 0-7   file name */
    uint8_t fileNameExtension[RD_FILENAME_EXTENSION];                    /* 8-10  file name extension */
    uint8_t fileAttributes[RD_FILE_ATTRIBUTES];                          /* 11    file attributes */
    uint8_t reserved[RD_RESERVED];                                       /* 12-21 reserved */
    uint8_t fileTime[RD_FILE_TIME];                                      /* 22-23 file time */
    uint8_t fileDate[RD_FILE_DATE];                                      /* 24-25 file date */
    uint8_t startClusNum[RD_STARTING_CLUSTER_NUMBER];          			 /* 26-27 starting cluster number */
    uint8_t fileSize[RD_FILE_SIZE];                                      /* 28-31 File size (0 for directories) */
} EntryFormat;

/* Define File Atributes */
#define ATT_READ_ONLY                               0x01
#define ATT_HIDDEN_FILE                             0x02
#define ATT_SYSTEM_FILE                             0x04
#define ATT_VOLUME_LABEL                            0x08
#define ATT_LONG_FILE_NAME                          0x0F
#define ATT_DIRECTORY                               0x10
#define ATT_ARCHIVE_FLAG                            0x20

/* Define LFN */
#define LFN_SEQUENCE_NUMBER                         1
#define LFN_FILE_NAME_1                             10
#define LFN_FILE_ATTRIBUTES                         1
#define LFN_RESERVED_1                              1
#define LFN_CHECKSUM                                1
#define LFN_FILE_NAME_2                             12
#define LFN_RESERVED_2                              2                                     
#define LFN_FILE_NAME_3                             4

/* Structure for LFN Entries format */
typedef struct LFN_STRUCT
{
    uint8_t sequenceNum[LFN_SEQUENCE_NUMBER];                   /* 0    Sequence number and allocation status */
    uint8_t fileName1[LFN_FILE_NAME_1];                         /* 1-10 File name characters(uincode) */
    uint8_t fileAttributes[LFN_FILE_ATTRIBUTES];                /* 11   File Attributes */
    uint8_t reserved1[LFN_RESERVED_1];                          /* 12   Reserved */
    uint8_t checkSum[LFN_CHECKSUM];                             /* 13   Checksum */
    uint8_t fileName2[LFN_FILE_NAME_2];                         /* 14*/
    uint8_t reserved2[LFN_RESERVED_2];                          /* */
    uint8_t fileName3[LFN_FILE_NAME_3];                         /* */
} LFNtypes;

/* Structure for name of file type LFN*/
typedef struct LFN_NAME 
{
    uint8_t filename1;
    uint8_t filename2;
    uint8_t filename3;
} LFN_NameTypes;

/* Define FileName Atributes */
#define FILENAME_NEVER_USED							0x00
#define FILENAME_DELETED_1							0xE5
#define FILENAME_DELETED_2							0x05
#define FILENAME_SPECIAL_ENTRY						0x2E

/* Bit Mask of hours, minutes, seconds*/
#define BIT_MASK_OF_HOURS                           0XF800
#define BIT_MASK_OF_MINUTES                         0x7E0
#define BIT_MASK_OF_SECONDS                         0x1F

/* Shift bit mask Hour, Minute, Second */
#define SHIFT_HOURS                                 0xB
#define SHIFT_MINUTES                               0x5
#define SHIFT_SECONDS                               0x00

/* Calculate hours, minutes, seconds */
#define CALC_HOURS(x)                               (((BIT_MASK_OF_HOURS) & (x)) >> (SHIFT_HOURS))
#define CALC_MINUTES(x)                             (((BIT_MASK_OF_MINUTES) & (x)) >> (SHIFT_MINUTES))
#define CALC_SECONDS(x)                             ((BIT_MASK_OF_SECONDS) & (x) >> (SHIFT_SECONDS))

/* Bit mask of year, month, day */
#define BIT_MASK_OF_YEAR                            0xFE00
#define BIT_MASK_OF_MONTH                           0x1E0
#define BIT_MASK_OF_DAY                             0x1F

/* Shif bit mask year, month, day */
#define SHIFT_YEAR                                  9
#define SHIFT_MONTH                                 5
#define SHIFT_DAY                                   0x00

#define BIN_YEAR_OFFSET_FROM                        1980
/* Calculate year, month, day */
#define CALC_YEAR(y)                                (((BIT_MASK_OF_YEAR) & (y)) >> (SHIFT_YEAR))
#define CALC_MONTH(y)                               (((BIT_MASK_OF_MONTH) & (y)) >> (SHIFT_MONTH))
#define CALC_DAY(y)                                 (((BIT_MASK_OF_DAY) & (y)) >> (SHIFT_DAY))


/* Define Struct Data Type */
typedef struct DATA {
    uint8_t fileAttributes;
    uint32_t startClusNum;
    uint32_t filetime;
    uint32_t filedate;
    uint8_t fileSize;
} DataTypes;


/* Define Enum FAT Types */
typedef enum FAT_TYPES {
    FAT12, 
    FAT16, 
    FAT32,
	ERROR
} FatTypes;
		
/** Define Enum Bool */
typedef enum Bool {
    FALSE,
    TRUE
} Bool;

/*** Function Prototype */
/* This function is to check type of the FAT files */
FatTypes checkFatTypes(const char *filePath);

/* This function is to read boot sector in FAT12 files */
FAT12BootTypes readBootSector12(const char *filePath);

/* This function is to read boot sector in FAT16 files */
FAT16BootTypes readBootSector16(const char *filePath);

/* This function is to read boot sector in FAT32 files */
FAT32BootTypes readBootSector32(const char *filePath);

/* This function is to read root directory in FAT12 files */
uint32_t readRootDirectory12(const char *filePath);

/* This function is to read root directory in FAT16 files */
uint32_t readRootDirectory16(const char *filePath);

/* This function is to read root directory in FAT32 files */
uint32_t readRootDirectory32(const char *filePath);

/* This function is to read sub directory in FAT12 files */
uint32_t readSubDirectory12(const char *filePath, DataTypes *data);

/* This function is to read sub directory in FAT16 files */
uint32_t readSubDirectory16(const char *filePath, DataTypes *data); 

/* This function is to read sub directory in FAT32 files */
uint32_t readSubDirectory32(const char *filePath, DataTypes *data); 

/* This function is to get data from Root to DataTypes in FAT12 files */
DataTypes getRootData12(const char *filePath, uint32_t index);

/* This function is to get data from Root to DataTypes in FAT16 files */
DataTypes getRootData16(const char *filePath, uint32_t index);

/* This function is to get data from Root to DataTypes in FAT32 files */
DataTypes getRootData32(const char *filePath, uint32_t index);

/* This function is to get data from sub directory to DataTypes in FAT12 files */
DataTypes getSubData12(const char *filePath, DataTypes *data, uint32_t index); 

/* This function is to get data from sub directory to DataTypes in FAT16 files */
DataTypes getSubData16(const char *filePath, DataTypes *data, uint32_t index); 

/* This function is to get data from sub directory to DataTypes in FAT32 files */
DataTypes getSubData32(const char *filePath, DataTypes *data, uint32_t index); 

/* This function is to read a file in FAT12 files */
uint32_t readFile12(const char *filePath, DataTypes *data);

/* This function is to read a file in FAT16 files */
uint32_t readFile16(const char *filePath, DataTypes *data);

/* This function is to read a file in FAT16 files */
uint32_t readFile32(const char *filePath, DataTypes *data);

/* This function is to check whether an element is a foler */
Bool isFolder(DataTypes data);

/* This function is to check the first byte of file name */
static Bool checkFileName(EntryFormat root);

/* This function is to reverse bytes */
uint32_t reverseByte(uint8_t *byte, uint32_t count);

/* This function is to get an entry in FAT12 sectors  */
uint32_t getEntryFat12(uint8_t *byte, uint32_t position);

/* This function is to clear space in a string */
static void removeSpace(uint8_t *str);


#endif /*_READFAT_H_*/

