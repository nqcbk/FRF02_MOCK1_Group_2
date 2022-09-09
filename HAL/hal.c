#include "hal.h"


/* ---------------------------------- */

void addNode(Generation **headGen, Node **head, uint32_t data, uint32_t fatherIndex)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode != NULL)
    {
        newNode->data = data;
		newNode->fatherIndex = fatherIndex;
        newNode->next = NULL;
        if (NULL == (*head))
        {
			addGeneration(headGen, newNode);
			*head = newNode;
			newNode->selfIndex = 0;
        }
        else
        {
            Node *temp = *head;
            while( temp->next != NULL )
            {
                temp = temp->next;
            }
			newNode->next = NULL;  
            temp->next = newNode;
			newNode->selfIndex = temp->selfIndex + 1;
        }
    }   
}

/* This function is to add a node */
void addNodeRoot(Generation **headGen, Node **headRoot, uint32_t data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode != NULL)
    {
        newNode->data = data;
		newNode->fatherIndex = 0;
        newNode->next = NULL;
        if (NULL == (*headRoot))
        {
			addGeneration(headGen, newNode);
			*headRoot = newNode;
			newNode->selfIndex = 0;
        }
        else
        {
            Node *temp = *headRoot;
            while( temp->next != NULL )
            {
                temp = temp->next;
            }
			newNode->next = NULL;  
            temp->next = newNode;
			newNode->selfIndex = temp->selfIndex + 1;
        }  
    }
}

/* This function is to add a generation */
void addGeneration(Generation **head, Node *firstNodeInGeneration)
{
    Generation *newGeneration = (Generation *)malloc(sizeof(Generation));
    if (newGeneration != NULL)
    {
        newGeneration->ptrGen = firstNodeInGeneration;
        newGeneration->next = NULL;
        if (NULL == (*head))
        {
            *head = newGeneration;
        }
        else
        {
            Generation *temp = (*head);
            while( temp->next != NULL )
            {
                temp = temp->next;
            }
            temp->next = newGeneration;
            newGeneration->next = NULL;
        }
    }
}


// // OpenFile
// void openFile(Generation *gen, uint32_t levelOfFile, uint32_t selfIndex) {
	// g_currentLevel++;
	// Generation *currentGen = gen;
	// uint32_t i = 0;
	// for (i = 0; i < levelOfFile - 1; i++) {
		// currentGen = currentGen->next;
	// }
	// Node * temp = currentGen->ptrGen;
    // while (temp != NULL)
    // {
		// if (temp->fatherIndex == selfIndex) {
			// printf("%d. Data in Generation: %c, fatherIndex: %d\n", count, temp->data, temp->fatherIndex);
		// }
        // temp = temp->next;
    // }
// }

/* This function is to open a directory	*/
uint32_t openDirectory(Generation *gen, uint32_t levelOfFolder, uint32_t selfIndex) {
	g_currentLevel++;
	
	int32_t count = 0;
	Generation *currentGen = gen;
	uint32_t i = 0;
	for (i = 0; i < levelOfFolder; i++) {
		currentGen = currentGen->next;
	}
	
	Node * temp = currentGen->ptrGen;
    while (temp != NULL)
    {
		if (temp->fatherIndex == selfIndex) {
			printf("%d. Data in Generation: %c, fatherIndex: %d\n", count, temp->data, temp->fatherIndex);
			count++;
		}
        temp = temp->next;
    }
	if (count == 0) {
		printf("Empty folder!\n");
	}
	
	return count;
}

/* This function is to get data from readFat level	*/
Generation *getData() {
	Generation *genHead = NULL;
    Node *headRoot = NULL;
	addNodeRoot(&genHead, &headRoot, 'a');
	addNodeRoot(&genHead, &headRoot, 'b');
	addNodeRoot(&genHead, &headRoot, 'c');
	
	Node *subDir = NULL;
	addNode(&genHead, &subDir, '1', 0);
	addNode(&genHead, &subDir, '2', 0);
	addNode(&genHead, &subDir, '3', 0);
	
	return genHead;
}

/* 
 * This function is to check the number inputed by user	
 * The number is a positive integer from 0 to maxNumber
 */
uint32_t inputNumberByUser(uint32_t maxNumber) {
	uint32_t number = 0;
	do{
		printf("Input a number to open: ");
		scanf("%d", &number);
		fflush(stdin);
	}while((number < 0) || (number >= maxNumber));
	return number;
}
