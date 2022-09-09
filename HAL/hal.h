#ifndef _HAL_H_
#define _HAL_H_


#include "readFat.h"


typedef enum Bool
{
    FALSE,
    TRUE
} Bool;


typedef struct Node{
    uint32_t data;
    uint32_t selfIndex;
    uint32_t fatherIndex;
    struct Node *next;
} Node;

typedef struct Generation{
	Node *ptrGen;
	struct Generation *next;
} Generation; 

/* ----------Function prototype------ */

Generation *getData();

uint32_t openDirectory(Generation *gen, uint32_t levelOfFolder, uint32_t selfIndex);

void addGeneration(Generation **head, Node *firstNodeInGeneration);

void addNodeRoot(Generation **headGen, Node **headRoot, uint32_t data);

void addNode(Generation **headGen, Node **head, uint32_t data, uint32_t fatherIndex);


/* --------- GLOBAL VALUABLE------- */

uint32_t g_currentLevel = 0;



#endif /*_HAL_H_*/

