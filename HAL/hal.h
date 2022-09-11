/*** Define HAL ***/
#ifndef _HAL_H_
#define _HAL_H_


#include "readFat.h"


/*******************************************************************************
* STRUCT
*******************************************************************************/

typedef struct Node{
    DataHalTypes data;
    struct Node *next;
} Node;

typedef struct Generation{
	uint32_t level;
	Node *ptrGen;
	struct Generation *next;
} Generation; 

/*******************************************************************************
* FUNCTION PROTOTYPE
******************************************************************************/

uint32_t inputNumberByUser(uint32_t maxNumber);

void printNode(Node *head);

uint32_t openRoot(Node **head, const char *filePath);

uint32_t openDirectory(Node **head, const char *filePath, uint8_t index);

uint32_t closeDirectory(Node **head, const char *filePath);

static void deleteNode(Node **head);

uint32_t numberOfNode(Node *head);

static void addNode(Node **head, DataHalTypes data);

DataHalTypes getDataFromLastNode(Node *head);


/*******************************************************************************
* GLOBAL VALUABLE
******************************************************************************/




#endif /*_HAL_H_*/

