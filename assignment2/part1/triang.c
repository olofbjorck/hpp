#include <stdio.h>
#include <stdlib.h>

// Declare function
int combination(int n, int k);

int main(int argc, char** argv) {

	// Get input from command line
	int n = 1;
	if(argc > 1) {
		n = atoi(argv[1]);
	}

	// Print the combination
	for(int i=0; i<n; i++) {
		for(int j=0; j<=i; j++) {
			printf("%d ", combination(i,j));
		}
		printf("\n");
	}

	return 0;
}

// Compute the combination
int combination(int n, int k) {
	int sum = 1;
	for(int i=1; i<=k; i++) {
		sum	= sum*(n+1-i)/i;	// avoid mult. assignment as int division floors
	}
	return sum;
}
