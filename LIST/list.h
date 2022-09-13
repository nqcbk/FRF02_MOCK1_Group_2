/*** LINKED LISH ***/
#ifndef _LIST_H_
#define _LIST_H_

#include "readFat.h"

/** Struct */
typedef struct Node{
    DataTypes data;
    struct Node *next;
} Node;

/** Function Prototype */

/* This function is to delete the last element of linked list */
void deleteLastNode();

/* This function is to get the number of elements in linked list */
uint32_t numberOfNode();

/* This function is to add an element to the last position of linked list */
void addNode(DataTypes data);

/* This function is to get data from the last element of linked list */
DataTypes getDataFromLastNode();

#endif /*_LIST_H_*/


