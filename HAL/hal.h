/*** Define HAL ***/
#ifndef _HAL_H_
#define _HAL_H_


#include "readFat.h"


/*******************************************************************************
* STRUCT
*******************************************************************************/

typedef struct Node{
    uint8_t *data;
    uint32_t selfIndex;
    uint32_t fatherIndex;
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

uint32_t getLevel(Generation *head);

uint32_t inputNumberByUser(uint32_t maxNumber);

void getData(Generation *gen, char data[][12], uint32_t fatherIndex);

uint32_t printList(Generation *gen);

uint32_t closeDirectory(Generation **gen);

// uint32_t openDirectory(Generation *gen, uint32_t fatherIndex) ;
uint32_t openDirectory(Generation **gen, uint32_t fatherIndex);

static void addGeneration(Generation **head);

static void deleteGeneration(Generation **head);

void addNodeRoot(Generation **headGen, Node **headRoot, uint8_t *data);

static void addNode(Generation **headGen, uint8_t *data, uint32_t fatherIndex);


/*******************************************************************************
* GLOBAL VALUABLE
******************************************************************************/




#endif /*_HAL_H_*/

