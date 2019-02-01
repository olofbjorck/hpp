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
void pop_head(node_t** head);
void print_list(node_t* head);

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
	head->index = 0;
	head->min = 0;
	head->max = 0;
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
        print_list(head);
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

// Pop function
void pop(node_t** head, unsigned int index) {

  // Next node
  node_t* current = *head;
  node_t* tmp = NULL;

  if (index == 0) { // Pop head
    pop_head(head);
  } else { // Pop non-head
    while (current->next != NULL) {
      if (current->next->index == index) {
        break;
      }
      current = current->next;
    }

    // Free node
    tmp = current->next; // Set tmp node to the one we remove
    if (tmp != NULL) {
      current->next = tmp->next; // Remove from list
      free(tmp);
    }
  }

}

// Pops the head
void pop_head(node_t** head) {
  node_t* next = (*head)->next; // Get next node (the new head)
  free(*head); // Free the (old) head
  *head = next; // Set new head
}

// Prints the list
void print_list(node_t* head) {

  // Set current node to head
  node_t* current = head;

  // Print
  printf("Index\tMin\tMax\n");
  while (current != NULL) {
    if ((current->index) > 0 && (current->index < 32))
      printf("%d\t%.2lf\t%.2lf\n", current->index, current->min, current->max);
    current = current->next; // Iterate
  }
}
