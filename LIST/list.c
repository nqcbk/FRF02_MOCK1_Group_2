#include "list.h"

/* This static node is global and only used in this file */
static Node *head;

/* This function is to add an element to the last position of linked list */
void addNode(DataTypes data){
    Node *newNode = (Node *)malloc(sizeof(Node));
	/* Assign Data for Node */
	newNode->data = data;
	newNode->next = NULL;
	if (head == NULL) {
		head = newNode;
	}
    else{
		/* Find the node at end of linked list */
		Node *temp = head;
		while( temp->next != NULL )
		{
			temp = temp->next;
		}
		newNode->next = NULL;  
		temp->next = newNode;
    }   
}

/* This function is to get the number of elements in linked list */
uint32_t numberOfNode(){
	uint32_t result = 0;
	if (head != NULL) {
		Node *lastNode = head;
		while (lastNode != NULL) {
			result++;
			lastNode = lastNode->next;
		}
	}
	return result;
}

/* This function is to delete the last element of linked list */
void deleteLastNode(){
	if(head != NULL) {
		/* If there is one node, delete that one */
		if (head->next == NULL){
            head = NULL;
        }
		/* If there are 2 or more nodes, find the last node and delete it */
        else{
            Node *temp = head;
            while((temp->next)->next != NULL){
				temp = temp->next;
            }
			temp->next = NULL;
        }
	}
}

/* This function is to get data from the last element of linked list */
DataTypes getDataFromLastNode(){
	DataTypes data;
	if(head != NULL) {
		/* Find the last node and assign data */
		Node *temp = head;
		while (temp != NULL) {
			data = temp->data;
			temp = temp->next;
		}
		return data;
	}
}


