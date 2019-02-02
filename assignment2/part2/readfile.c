#include <stdio.h>

int main() {

	// Open file
	FILE* f = fopen("little_bin_file", "r");

	// Declare variables in which to store file input
	int intVar;
	double doubleVar;
	char charVar;
	float floatVar;

	// Read file
	fread(&intVar, sizeof(int), 1, f);
	fread(&doubleVar, sizeof(double), 1, f);
	fread(&charVar, sizeof(char), 1, f);
	fread(&floatVar, sizeof(float), 1, f);

	// Close file
	fclose(f);

	// Print what was read from the file
	printf("%d\n", intVar);
	printf("%lf\n", doubleVar);
	printf("%c\n", charVar);
	printf("%f\n", floatVar);

	return 0;
}
