#include "hal.h"



/* This function is to add a node in sub directory */
static void addNode(Generation **headGen, uint8_t *data, uint32_t fatherIndex)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode != NULL)
    {
        newNode->data = data;
		newNode->next = NULL;
		if ((*headGen)->next == NULL) {
			newNode->fatherIndex = 0;
		}
		else {
			newNode->fatherIndex = fatherIndex;
		}
        if (NULL == ((*headGen)->ptrGen))
        {
			(*headGen)->ptrGen = newNode;
			newNode->selfIndex = 0;
        }
        else
        {
            Node *temp = (*headGen)->ptrGen;
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
static void addGeneration(Generation **head)
{
    Generation *newGeneration = (Generation *)malloc(sizeof(Generation));
    if (newGeneration != NULL)
    {
        newGeneration->ptrGen = NULL;
        newGeneration->next = NULL;
		newGeneration->level = 0;
        if (NULL == (*head))
        {
            *head = newGeneration;
			newGeneration->level = 1;
        }
        else
        {
            Generation *temp = (*head);
            while(temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newGeneration;
            newGeneration->next = NULL;
			newGeneration->level = temp->level + 1;
        }
    }
}

// Lay so phan tu cua gen
uint32_t getLevel(Generation *head) {
	uint32_t result = 0;
	if (head != NULL) {
		// Tim den gen cuoi cung
		Generation *lastGen = head;
		while (lastGen->next != NULL) {
			lastGen = lastGen->next;
		}
		result = lastGen->level;
	}
	return result;
}

/* This function is to delete the last Generation */
static void deleteGeneration(Generation **head){
	if((*head) != NULL) {
		// Neu chi co 1 gen thi xoa gen do
		if ((*head)->next == NULL)
        {
            *head = NULL;
        }
		// Neu co nhieu gen thi xoa gen cuoi cung
        else
        {
            Generation *temp = (*head);
            while((temp->next)->next != NULL )
            {
				temp = temp->next;
            }
			temp->next = NULL;
        }
	}
}

/* This function is to open a file	*/


/* This function is to close directory	*/
uint32_t closeDirectory(Generation **gen) {
	uint32_t numberOfElements = 0;
	if((*gen) != NULL) {
		deleteGeneration(gen);
		// Print data
		numberOfElements = printList(*gen);
	}
	// Tra ve so phan tu cua thu muc
	return numberOfElements;
}


char data[][12] = {"app", "doc", "pic", "sample.txt", "sample1.txt", "sample2.txt", "sample3.txt"};
void getData(Generation *gen, char data[][12], uint32_t fatherIndex) {
	// Tim den phan tu cuoi cung cua gen
	Generation *lastGen = gen;
	while (lastGen->next != NULL) {
		lastGen = lastGen->next;
	}
	// Add data cho generation cuoi cung
	uint32_t i = 0;
	for (i = 0; i < 7; i++) {
		addNode(&lastGen, data[i], fatherIndex);
	}
}
// Ham in toan bo thu muc va file cua generation cuoi cung, tra ve so luong phan tu
uint32_t printList(Generation *gen){
	uint32_t count = 0;
	if(gen != NULL) {
		// Tim den gen cuoi cung
		Generation *lastGen = gen;
		while (lastGen->next != NULL) {
			lastGen = lastGen->next;
		}
		// In toan bo data cua cac node trong gen cuoi cung
		Node *temp = lastGen->ptrGen;
		while (temp != NULL) {
			printf("%d. %s\n", temp->selfIndex + 1, temp->data);
			count = temp->selfIndex;
			temp = temp->next;
		}
	}
	return count;
}

/* This function is to open a directory	*/
uint32_t openDirectory(Generation **gen, uint32_t fatherIndex) {
	uint32_t count = 0;
	uint32_t i = 0;
	// Khoi tao mot generation moi voi con tro gia tri = NULL
	addGeneration(gen);
	
	// Add data
	getData(*gen, data, fatherIndex);
	
	// Print data
	uint32_t numberOfElements = printList(*gen);
	
	return numberOfElements;
}

// /* This function is to get data from readFat level	*/

// Generation *getData() {
	// Generation *genHead = NULL;
    // Node *headRoot = NULL;
	// addNodeRoot(&genHead, &headRoot, "app");
	// addNodeRoot(&genHead, &headRoot, "doc");
	// addNodeRoot(&genHead, &headRoot, "pic");
	// addNodeRoot(&genHead, &headRoot, "sample.txt");
	// addNodeRoot(&genHead, &headRoot, "sample1.txt");
	// addNodeRoot(&genHead, &headRoot, "sample2.txt");
	// addNodeRoot(&genHead, &headRoot, "sample3.txt");
	
	// Node *subDir1 = NULL;
	// addNode(&genHead, &subDir1, "new", 1);
	// addNode(&genHead, &subDir1, "concept.doc", 1);
	// addNode(&genHead, &subDir1, "lkcd.pdf", 1);
	// addNode(&genHead, &subDir1, "cpy.png", 2);
	// addNode(&genHead, &subDir1, "elipse.png", 2);
	
	// Node *subDir2 = NULL;
	// addNode(&genHead, &subDir2, "newagin", 0);
	// addNode(&genHead, &subDir2, "newagin2", 1);
	
	// Node *subDir3 = NULL;
	// addNode(&genHead, &subDir3, "n1", 0);
	
	
	// return genHead;
// }

/* 
 * This function is to check the number inputed by user	
 * The number is a positive integer from 0 to maxNumber
 */
uint32_t inputNumberByUser(uint32_t maxNumber) {
	uint32_t number = 0;
	do{
		printf("\nInput a number to open: ");
		scanf("%d", &number);
	}while((number < 0) || (number > maxNumber));
	return number;
}

