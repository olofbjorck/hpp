// RUN BY:
// time ./galsim 03000 ../input_data/ellipse_N_03000.gal 100 0.00001 0.1 0

// ./galsim 2 ../input_data/circles_N_2.gal 100 0.00001 0.1 0
// ./galsim 4 ../input_data/circles_N_4.gal 100 0.00001 0.1 0


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

void computeForces(particle_t* particles, int N, node_t* root,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max);

void printParticles(particle_t* particles, int N);
void printTotalMass(particle_t*, int);

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

	//printParticles(particles, N);
	printTotalMass(particles, N);

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
		computeCenterOfMass(root);
		computeForces(particles, N, root, G, eps0, delta_t, theta_max);
		printf("root->mass = %lf\n", root->mass);
		printf("root->centerOfMass_x = %lf\n", root->centerOfMass_x);
		printf("root->centerOfMass_y = %lf\n", root->centerOfMass_y);
	}

	// Write new state of particles to file
	if (writeOutput(particles, brightness, N))
		return 1;

	// Free memory
	free(particles);
	free(brightness);
	freeQuadtree(root);

	// Success
	return 0;
}

void computeForces(particle_t* particles, int N, node_t* root,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max) {

	unsigned int i, j; // Loop iterators
	double r = 0.0, r_x = 0.0, r_y = 0.0; // r-vector
	double denom = 0.0; // Denominator
	node_t* node;
	double theta;

	// Set acceleration to 0
	for (i = 0; i < N; i++) {
		particles[i].a_x = 0.0;
		particles[i].a_y = 0.0;
	}

	// Loop remaining particles, assuming acceleration is properly initialized
	for (i = 0; i < N; i++) {
		
		node = find(&particles[i], node);

		for (j = i + 1; j < N; j++) {
			// Calculate r-vector
			r_x = particles[i].x - particles[j].x;
			r_y = particles[i].y - particles[j].y;
			r = sqrt(r_x*r_x + r_y*r_y);
			// Calculate denominator
			denom = r + eps0;
			denom = 1/(denom*denom*denom); // 1 msec faster than using /denom below
			// Calculate acceleration (a_x, a_y == 0 at start due to calloc)
			particles[i].a_x += particles[j].mass*r_x*denom;
			particles[i].a_y += particles[j].mass*r_y*denom;
			// Calculate corresponding acceleration for other particle, using
			// Newton's third law
			particles[j].a_x -= particles[i].mass*r_x*denom;
			particles[j].a_y -= particles[i].mass*r_y*denom;
		}
		// Update velocity
		particles[i].v_x += -G*delta_t*particles[i].a_x;
		particles[i].v_y += -G*delta_t*particles[i].a_y;
		// Update position
		particles[i].x += delta_t*particles[i].v_x;
		particles[i].y += delta_t*particles[i].v_y;
	}
}

void printParticles(particle_t* particles, int N) {
	unsigned int i;
	for (i = 0; i < N; i++) {
		printf("Particle %d:\n", i);
		printf("... (x, y) = (%lf, %lf)\n", particles[i].x, particles[i].y);
		printf("... mass = %lf\n", particles[i].mass);
	}
}

void printTotalMass(particle_t* particles, int N) {
	unsigned int i;
	double mass = 0;
	for (i = 0; i < N; i++) {
		mass += particles[i].mass;
	}
	printf("Total mass = %lf\n", mass);
}
