#include <stdio.h>
#include <stdlib.h>

int combination(int n, int k);

int main(int argc, char** argv) {

	int n = 1;
	if(argc > 1) {
		n = atoi(argv[1]);
	}

	for(int i=0; i<n; i++) {
		for(int j=0; j<=i; j++) {
			printf("%d ", combination(i,j));
		}
		printf("\n");
	}

	return 0;
}

int combination(int n, int k) {
	int sum = 1;
	for(int i=1; i<=k; i++) {
		sum	= sum*(n+1-i)/i;
	}
	return sum;
}
