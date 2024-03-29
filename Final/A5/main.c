// RUN BY:
// time ./galsim 05000 ../input_data/ellipse_N_05000.gal 100 0.00001 0.1 0 1

// ./galsim 2 ../input_data/circles_N_2.gal 100 0.00001 0.1 0
// ./galsim 4 ../input_data/circles_N_4.gal 100 0.00001 0.1 0

// Check: (from inside compare_gal_files folder)
// ./compare_gal_files 3000 ../A4/result.gal ../ref_output_data/ellipse_N_03000_after100steps.gal
// ../compare_gal_files/compare_gal_files 3000 ../A4/result.gal ../ref_output_data/ellipse_N_03000_after100steps.gal

// ../compare_gal_files/compare_gal_files 2000 ../A4/result.gal ../ref_output_data/ellipse_N_02000_after200steps.gal

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
#include "modules.h"
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
	if (argc != 8) {
		printf("%s\n", "Input error: Expected 7 input arguments");
		return 1;
	}

	// Read input from command line
	const char* program = argv[0];	// Used to return graphics errors
	const int N = atoi(argv[1]); // Nr of stars to simulate
	const char* filename = argv[2]; // Filename
	const int nsteps = atoi(argv[3]); // Nr of filesteps
	const double delta_t = atof(argv[4]); // Timestep
	const double theta_max = atof(argv[5]);
	const int graphics = atoi(argv[6]); // Graphics on/off as 1/0
	const int n_threads = atoi(argv[7]);

	// Constants for the simulation
	const double G = 100.0/N; // Gravitational constant
	const double eps0 = 0.001; // Plummer sphere constant

	// Graphics constants
	const unsigned int windowSize = 1000;
	const float circleRadius = 0.0025f;
	const float circleColour = 0.0f;

	// Add the simulation constants to struct
	simulationConstants_t* simulationConstants =
			(simulationConstants_t*) malloc(sizeof(simulationConstants_t));
	simulationConstants->N = &N;
	simulationConstants->nsteps = &nsteps;
	simulationConstants->delta_t = &delta_t;
	simulationConstants->theta_max = &theta_max;
	simulationConstants->n_threads = &n_threads;
	simulationConstants->G = &G;
	simulationConstants->eps0 = &eps0;

	// Add the graphics constants to struct
	graphicsConstants_t* graphicsConstants =
			(graphicsConstants_t*) malloc(sizeof(graphicsConstants_t));
	graphicsConstants->program = &program;
	graphicsConstants->windowSize = &windowSize;
	graphicsConstants->circleRadius = &circleRadius;
	graphicsConstants->circleColour = &circleColour;

	// Create particles struct and allocate space
	particles_t* particles = (particles_t*) malloc(sizeof(particles_t));
	particles->x = (double*) malloc(N * sizeof(double));
	particles->y = (double*) malloc(N * sizeof(double));
	particles->v_x = (double*) malloc(N * sizeof(double));
	particles->v_y = (double*) malloc(N * sizeof(double));
	particles->mass = (double*) malloc(N * sizeof(double));
	double* brightness = (double*) malloc(N * sizeof(double));

	// Check malloc
	if (!(particles && brightness)) {
		// Program fail, exit
		printf("ERROR: Malloc failure");
		return 1;
	}

	// Read data
	if (readData(particles, brightness, filename, N))
		return 1;

	// Simulate
	if (graphics) {
		// Simulate with graphics
		simulateWithGraphics(particles, simulationConstants, graphicsConstants);
	} else {
		// Simulate movement only (only calculations)
		simulate(particles, simulationConstants);
	}

	// Write new state of particles to file
	if (writeOutput(particles, brightness, N))
		return 1;

	// Free memory
	free(particles->x);
	free(particles->y);
	free(particles->v_x);
	free(particles->v_y);
	free(particles->mass);
	free(particles);
	free(brightness);
	free(graphicsConstants);
	free(simulationConstants);

	// Success
	return 0;
}
