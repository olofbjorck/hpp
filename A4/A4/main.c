// RUN BY:
// time ./galsim 03000 ../input_data/ellipse_N_03000.gal 100 0.00001 0.1 0


// REQUIRES PRE-COMPILATION OF compare_gal_files.c: REMEMBER TO REMOVE BEFORE HANDIN
// ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 0.00001 0.1 0
// ../compare_gal_files/compare_gal_files 3000 result.gal ../ref_output_data/ellipse_N_03000_after100steps.gal

/**
 * Simulates galaxy movement in outer space.
 *
 * Original authors: Olof Björck, Gunnlaugur Geirsson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "galsim.h"
#include "io.h"
#include "quadtree.h"

/**
 * Main function
 *
 */
int main(int argc, char const *argv[]) {

	// Check proper number of input arguments
	if (argc != 7) {
		printf("%s\n", "Input error: Expected 6 input arguments");
		return 1;
	}

	// Read input from command line
	const char* program = argv[0];	// Used to return graphics errors
	const int N = atoi(argv[1]); // Nr of stars to simulate
	if (N < 2) {
		// Fail. What is there to simulate?
		printf("%s\n", "ERROR: N < 2, what is there to simulate?");
		return 1;
	}
	const char* filename = argv[2]; // Filename
	const int nsteps = atoi(argv[3]); // Nr of filesteps
	const double delta_t = atof(argv[4]); // Timestep
	const double theta_max = atof(argv[5]);
	const int graphics = atoi(argv[6]); // Graphics on/off as 1/0

	// Define some constants
	const double G = 100.0/N; // Gravitational constant
	const double eps0 = 0.001; // Plummer sphere constant

	// Graphics variables
	const unsigned int windowSize = 1000;
	const float circleRadius = 0.0025f;
	const float circleColour = 0.0f;

	// Create array with all particles
	particle_t* particles = (particle_t*) malloc(N * sizeof(particle_t)); // TODO better malloc? array of pointers to structs?
	double* brightness = (double*) malloc(N * sizeof(double));
	if (!(particles && brightness)) {
		// Program fail, exit
		printf("ERROR: Malloc failure");
		return 1;
	}

	// Initialize quadtree root
	printf("%s\n", "Allocating root");
	node_t* root = (node_t*) malloc(sizeof(node_t));

	// Read data
	printf("%s\n", "Reading data");
	if (readData(particles, brightness, filename, N))
		return 1;

	// Simulate
	if (graphics) {
		// With graphics
		simulateWithGraphics(
				particles, N, G, eps0, nsteps, delta_t,
				program, windowSize, circleRadius, circleColour);
	} else {
		// Movement only
		//simulate(particles, N, G, eps0, nsteps, delta_t);
		printf("%s\n", "Building quadtree");
		buildQuadtree(particles, N, root);
	}

	// Write new state of particles to file
	if (writeOutput(particles, brightness, N))
		return 1;

	// Free memory
	free(particles);
	free(brightness);
	freeQuadTree(root);
	free(root);

	// Success
	return 0;
}
