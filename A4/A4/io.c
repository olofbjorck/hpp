#include "io.h"

// Read data from file.
int readData(
		particle_t* __restrict particles,
		double* __restrict brightness,
		const char* filename, const int N) {

	// Open file
	FILE* fp = fopen(filename, "r");

	// Check file open went ok
	if (!fp) {
		printf("%s\n", "ERROR: Failed to open input file. Is it in directory?");
		return 1;
	}

	// Check file size is as expected
	fseek(fp, 0L, SEEK_END); // Seek end
	size_t fileSize = ftell(fp); // Get file size
	fseek(fp, 0L, SEEK_SET); // Reset seek to start of file
	if (fileSize != 6*N*sizeof(double)) { // File size not as expected?
		printf("%s\n", "ERROR: Input file size is not as expected. Is N correct?");
		return 1;
	}

	// Read file
	unsigned int i;
	for (i = 0; i < N; i++) {
		if (
				fread(&particles[i].x, sizeof(double), 1, fp) &&
				fread(&particles[i].y, sizeof(double), 1, fp) &&
				fread(&particles[i].mass, sizeof(double), 1, fp)  &&
				fread(&particles[i].v_x, sizeof(double), 1, fp)  &&
				fread(&particles[i].v_y, sizeof(double), 1, fp)  &&
				fread(&brightness[i], sizeof(double), 1, fp)) {
			// Do nothing
		} else {
			printf("ERROR: Failed to read particle %d from input file\n", i);
			return 1;
		}
	}
	// Close file
	if (fclose(fp)) {
		// Fail
		printf("%s\n", "ERROR: Failed to close input file.");
		return 1;
	}

	// Success
	return 0;
}

// Write current state of all particles to file
int writeOutput(
		particle_t* __restrict particles,
		double* __restrict brightness,
		const int N) {

	// Create file to write
	FILE* fp = fopen("result.gal", "w");

	// Check file creation went ok
	if (!fp) {
		printf("%s\n", "ERROR: Failed to create output file");
		return 1;
	}

	// Write to file
	unsigned int i;
	for (i = 0; i < N; i++) {
		if (
				fwrite(&particles[i].x, sizeof(double), 1, fp) &&
				fwrite(&particles[i].y, sizeof(double), 1, fp) &&
				fwrite(&particles[i].mass, sizeof(double), 1, fp)  &&
				fwrite(&particles[i].v_x, sizeof(double), 1, fp)  &&
				fwrite(&particles[i].v_y, sizeof(double), 1, fp)  &&
				fwrite(&brightness[i], sizeof(double), 1, fp)) {
			// Do nothing
		} else {
			printf("ERROR: Failed to write particle %d to output file\n", i);
			return 1;
		}
	}

	// Close file
	if (fclose(fp)) {
		// Fail
		printf("%s\n", "ERROR: Failed to close output file.");
		return 1;
	}

	// Success
	return 0;
}
