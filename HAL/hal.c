#include "hal.h"



/* This function is to add a node in sub directory */
static void addNode(Node **head, DataHalTypes data){
    Node *newNode = (Node *)malloc(sizeof(Node));
	
	// Assign Data for Node
	newNode->data = data;
	newNode->next = NULL;
	if ((*head) == NULL) {
		(*head) = newNode;
	}
    else{
		// Find the node at end of linked list
		Node *temp = *head;
		while( temp->next != NULL )
		{
			temp = temp->next;
		}
		newNode->next = NULL;  
		temp->next = newNode;
    }   
}


// Lay so phan tu cua list
uint32_t numberOfNode(Node *head) {
	uint32_t result = 0;
	if (head != NULL) {
		// Tim den phan tu cuoi cung
		Node *lastNode = head;
		while (lastNode != NULL) {
			result++;
			lastNode = lastNode->next;
		}
	}
	return result;
}

/* This function is to xoa phan tu cuoi cung */
static void deleteNode(Node **head){
	if((*head) != NULL) {
		// Neu chi co 1 node thi xoa node do
		if ((*head)->next == NULL)
        {
            *head = NULL;
        }
		// Neu co nhieu node thi xoa node cuoi cung
        else
        {
            Node *temp = (*head);
            while((temp->next)->next != NULL )
            {
				temp = temp->next;
            }
			temp->next = NULL;
        }
	}
}

/* This function is to open a file	*/



// Ham in toan bo thu muc va file cua generation cuoi cung, tra ve so luong phan tu
void printNode(Node *head){
	if(head != NULL) {
		// Tim den node cuoi cung
		Node *temp = head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		printf("fileAttributes: %x\n", temp->data.fileAttributes);
		printf("startingClusterNumber: %x\n", temp->data.startingClusterNumber);
		printf("fileSize: %x\n", temp->data.fileSize);
		printf("\n");
	}
}

/* This function is to open a directory	*/
uint32_t openRoot(Node **head, const char *filePath) {
	uint32_t count = 0;
	
	// In root
	uint32_t numberOfElements = readRootDirectory12(filePath);
	if (numberOfElements == 0) {
		printf("Empty Folder!\n");
	}	
	// Tra ve so phan tu trong thu muc root
	return numberOfElements;
}

// Ham lay data cua node cuoi cung
DataHalTypes getDataFromLastNode(Node *head){
	DataHalTypes data;
	if(head != NULL) {
		// Tim den node cuoi cung
		Node *temp = head;
		while (temp != NULL) {
			data = temp->data;
			temp = temp->next;
		}
		return data;
	}
}

/* This function is to open a directory	*/
uint32_t openDirectory(Node **head, const char *filePath, uint8_t index) {
	uint32_t count = 0;
	uint32_t numberOfElements  = 0;
	DataHalTypes data = getDataFromLastNode(*head);
	
	// Lay data
	// Neu link list chua co phan tu nao thi getdata tu root
	if ((*head) == NULL) {
		data = getRootData(filePath, index);
		addNode(head, data);
	}
	// Neu link list co phan tu roi thi get data tu sub
	else {
		data = getSubData(filePath, &data, index);
		addNode(head, data);
	}
	
	// Neu nguoi dung chon mot thu muc thi mo thu muc sub
	if (isFolder(data)) {
		printf("This is a folder!\n");
		// printNode(*head);
		numberOfElements = readSubDirectory(filePath, &data);
		if (numberOfElements == 0) {
			printf("Empty Folder!\n");
		}
	}
	// Neu nguoi dung chon mot file thi mo file
	else {
		printf("This is a file!\n");
		numberOfElements = readFile(filePath, &data);
		
	}
	
	return numberOfElements;
}

/* This function is to close a directory	*/
uint32_t closeDirectory(Node **head, const char *filePath) {
	uint32_t count = 0;
	uint32_t numberOfElements  = 0;
	
	// Xoa node cuoi cung
	deleteNode(head);
	if (numberOfNode(*head) == 0) {
		// Dung ham readRoot de doc
		numberOfElements = readRootDirectory12(filePath);
	}
	else {
		// Lay data tu node cuoi cung cua list
		DataHalTypes data = getDataFromLastNode(*head);
		
		// Hien thi noi dung thu muc, 
		// Neu 
		numberOfElements = readSubDirectory(filePath, &data);
	}
	
	// Tra ve so phan tu cua thu muc
	return numberOfElements;
}

/* 
 * This function is to check the number inputed by user	
 * The number is a positive integer from 0 to maxNumber
 */
uint32_t inputNumberByUser(uint32_t maxNumber) {
	uint32_t number = 0;
	do{
		printf("\nInput a number to open: ");
		fflush(stdin);
		scanf("%d", &number);
	}while((number < 0) || (number > maxNumber));
	return number;
}

