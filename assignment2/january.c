#include <stdio.h>
#include <stdlib.h>

// Define node
typedef struct node {
	unsigned int index;
	double min;
	double max;
	struct node* next;
} node_t;

// Define functions
void push(node_t** head, unsigned int index, double min, double max);
void pop(node_t** head, unsigned int index);

int main() {

  // Declare variables
	char action;
	char flush;
	int running = 1;
	int size = 0;

	unsigned int index;
	double min;
	double max;

	// Initialize list
	node_t* head = NULL;
	head = malloc(sizeof(node_t));
	head->index = 1;
	head->min = 1;
	head->max = 1;
	head->next = NULL;

  // Program loop
	while(running) {
		scanf(" %c", &action); // Scan first command A, D, P, or Q
		switch(action) {
			case 'A':
				scanf("%u", &index);
				scanf("%lf", &min);
				scanf("%lf", &max);
				push(&head, index, min, max);
				break;
			case 'D':
				printf("2\n");
				break;
			case 'P':
				break;
			case 'Q':
				running = 0;
				break;
			default:
				printf("Syntax error: Invalid command");
				do {
					scanf("%c", &flush);
				} while(flush != '\n');
				printf("\n");
				break;
		}
	}

	return 0;
}

// Push function
void push(node_t** head, unsigned int index, double min, double max) {
	// Create new node
	node_t* newNode = (node_t*)malloc(sizeof(node_t));
	newNode->index = index;
	newNode->min = min;
	newNode->max = max;
	newNode->next = *head;

	// Move head to new node
	*head = newNode;
}