#include <stdio.h>
#include <stdlib.h>

// Define node
typedef struct node {
	unsigned int index;
	double min;
	double max;
	struct node* next;
} node_t;

// Declare functions
void push(node_t** head, unsigned int index, double min, double max);
void placeNodeSorted(node_t** head, node_t* newNode);
void pop(node_t** head, unsigned int index);
void popHead(node_t** head);
void printList(node_t* head);
void flushInput();

int main() {

	// Declare variables
	char action;	// User command
	char errorCheck;
	int running = 1;

	unsigned int index;
	double min;
	double max;

	// Initialize list with empty head
	node_t* head = NULL;

	// Program loop
	while(running) {
		printf("Enter command: ");
		scanf(" %c", &action); // Scan first command A, D, P, or Q
		switch(action) {
			case 'A':
				scanf("%u", &index);
				scanf("%lf", &min);
				scanf("%lf", &max);
				scanf("%c", &errorCheck);
				if(errorCheck == '\n') {
					push(&head, index, min, max);
				} else {
					flushInput();
				}
				//printf(" >> Added index %d\n", index);
				break;
			case 'D':
				scanf("%u", &index);
				scanf("%c", &errorCheck);
				if(errorCheck == '\n') {
					pop(&head, index);
				} else {
					flushInput();
				}
				//printf(" >> Deleted index %d\n", index);
				break;
			case 'P':
				scanf("%c", &errorCheck);
				if(errorCheck == '\n') {
					printList(head);
				} else {
					flushInput();
				}
				break;
			case 'Q':
				scanf("%c", &errorCheck);
				if(errorCheck == '\n') {
					running = 0;
				} else {
					flushInput();
				}
				break;
			default:
				flushInput();
				break;
		}
	}

	return 0;
}

void flushInput() { 
	char flush;		// Used for flushing output
	printf("Syntax Error: Invalid command");
	// Read rest of junk input into junk variable (flush)
	do {
		scanf("%c", &flush);
	} while(flush != '\n');
	printf("\n");
}

// Push function
void push(node_t** head, unsigned int index, double min, double max) {

	if (index < 1 || index > 31) {
		printf("%s\n", "Runtime Error: Index out of bounds. Permitted values are 1-31.");
	} else {
		// Create new node
		node_t* newNode = (node_t*)malloc(sizeof(node_t));
		newNode->index = index;
		newNode->min = min;
		newNode->max = max;
		newNode->next = NULL;

		// Place new node index in list, at correct index
		placeNodeSorted(head, newNode);
	}
}

// Adds a node to list so the list remains sorted
void placeNodeSorted(node_t** node, node_t* newNode) {

	// If list does not exist or new node is smaller than head
	// >> Replace head with new node and append old head (can be NULL)
	if (*node == NULL || (*node)->index > newNode->index) {
		newNode->next = *node;
		*node = newNode;	// replace head

		// If new node is same as head
	} else if((*node)->index == newNode->index) {
		(*node)->min = newNode->min;
		(*node)->max = newNode->max;
		free(newNode);

		// Else new node is greater than head
	} else {

		// If next node exists 
		if ((*node)->next) {

			// If new node has larger index than node->next->index, recurse
			if ((*node)->next->index < newNode->index) {
				placeNodeSorted(&(*node)->next, newNode);

				// Else if node->next->index is larger than new node's index, we found the spot
			} else if ((*node)->next->index > newNode->index) {
				newNode->next = (*node)->next;
				(*node)->next = newNode;

				// Else, the index is equal. Replace the data
			} else {
				newNode->next = (*node)->next->next;
				(*node)->next = newNode;
			}

			// If next node is null, append to list
		} else {
			(*node)->next = newNode;
		}
	}
}

// Pop function
void pop(node_t** head, unsigned int index) {

	// Next node
	node_t* current = *head;
	node_t* tmp = NULL;

	// If head has correct index, pop head
	if((*head)->index == index) {
		popHead(head);
		// Else iterate through list
	} else {
		while (current->next) {
			if (current->next->index == index) {
				break;
			}
			current = current->next; 
		}

		// Store pop node in temp variable to refactor pointers
		// tmp is NULL if index did not exist in list
		tmp = current->next;
		if (tmp) {
			current->next = tmp->next;
			free(tmp);
		}
	}
}

// Pops the head
void popHead(node_t** head) {
	node_t* next = (*head)->next; 	// Get next node (the new head)
	free(*head); 					// Free the (old) head
	*head = next; 					// Set new head
}

// Prints the list
void printList(node_t* head) {

	// Set current node to head
	node_t* current = head;

	// Print
	//printf(" >> %s\n", "Printing database...");
	printf("day\tmin\tmax\n");
	while (current) {
		if ((current->index) > 0 && (current->index < 32))
			printf("%d\t%.2lf\t%.2lf\n", current->index, current->min, current->max);
		current = current->next; // Iterate
	}
}
