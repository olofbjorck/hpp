#include <stdio.h>

int main() {

	FILE* f = fopen("little_bin_file", "r");

	int intVar;
	double doubleVar;
	char charVar;
	float floatVar;

	fread(&intVar, sizeof(int), 1, f);
	fread(&doubleVar, sizeof(double), 1, f);
	fread(&charVar, sizeof(char), 1, f);
	fread(&floatVar, sizeof(float), 1, f);

	fclose(f);

	printf("%d\n", intVar);
	printf("%lf\n", doubleVar);
	printf("%c\n", charVar);
	printf("%f\n", floatVar);


	return 0;
}
