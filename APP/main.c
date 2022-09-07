#include "hal.h"

int main(int argc, char* argv[])
{

    const char *path = "floppy.img";

    // const char *path = "floppy32.ima";

    FatTypes fat = checkFatTypes(path);
    printf("[INFO] FAT type: %d\n", fat);

    FAT12BootTypes *bootPtr;

    if (fat == FAT12)
    {
        bootPtr = readBootSector12(path);
    }
    
    printf("[INFO] FAT name: ");
    for (int i = 0; i < 8; i++)
    {
        printf("%c", bootPtr->fileSystemType[i]);
    }


    FAT12RootTypes *rootPrt = readRootDirectory12(path, bootPtr);

    // printf("[INFO] File name: ");
    // for (int i = 0; i < 8; i++)
    // {
    //     printf("%c", rootPrt->fileName[i]);
    // }
    // printf("\n");
    
    printf("done\n");
    return 0;
}