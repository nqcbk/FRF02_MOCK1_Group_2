/*** HARDWARE ABSTRACTION LAYER ***/
#ifndef _HAL_H_
#define _HAL_H_

#include "readFat.h"
#include "list.h"

/** Function Prototype */
/* This function is to check the number inputed by user	*/
uint32_t inputNumberByUser(uint32_t maxNumber);

/* This function is to open root */
uint32_t openRoot(const char *filePath);

/* This function is to open a directory	*/
uint32_t openDirectory(const char *filePath, uint8_t index);

/* This function is to close a directory */
uint32_t closeDirectory(const char *filePath);

#endif /*_HAL_H_*/


