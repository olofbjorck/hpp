// RUN BY:
// time ./galsim 03000 ../input_data/ellipse_N_03000.gal 100 0.00001 0


// REQUIRES PRE-COMPILATION OF compare_gal_files.c: REMEMBER TO REMOVE BEFORE HANDIN
// ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 0.00001 0
// ../compare_gal_files/compare_gal_files 3000 result.gal ../ref_output_data/ellipse_N_03000_after100steps.gal

/**
 * Simulates galaxy movement in outer space.
 *
 * Original authors: Olof Björck, Gunnlaugur Geirsson
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graphics.h"

/**
 * Particle struct declaration.
 *
 */
typedef struct particle {
	double x, v_x; // x-position x and x-velocity vx
	double y, v_y; // y-position y and y-velocity vy
	double a_x, a_y; // Force exerted on particle
	double mass;  // particle mass.
} particle_t;

/**
 * Reads galaxy data from input file "filename" into particle_t* array
 * particles. Galaxy brightness is stored in separate array double* brightness
 * for speed as brightness isn't used in calculations. Returns 0 if data was
 * read successfully, 1 otherwise.
 *
 * @param  particles  Information about every particle.
 * @param  brightness Array with brightness information about every particle.
 * @param  filename   The input data filename (ending with ".gal").
 * @param  N          The total number of particles.
 * @return            Returns 1 if data was read successfully, else 0.
 */
static int readData(
		particle_t* __restrict particles,
		double* __restrict brightness,
		const char* filename, const int N);

/**
 * Simulates the movement of all the particles in particle_t* particles array.
 * The simulation calculates the forces acting on each particle and then
 * updates every particle's state. The simulation is carried out nsteps times,
 * with timestep delta_t.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smoothe calculations.
 * @param nsteps    Number of timesteps to simulate.
 * @param delta_t   Timestep [seconds].
 */
static void simulate(
		particle_t* __restrict particles, const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t);

/**
 * Extension of function simulate, showing the movements of the particles
 * graphically during the simulation. Unsuitable for performance testing.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smoothe calculations.
 * @param nsteps    Number of timesteps to simulate.
 * @param delta_t   Timestep [seconds].
 */
static void simulateWithGraphics(
		particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t,
		const char* __restrict program,
		const unsigned int windowSize,
		const float circleRadius,
		const float circleColour);

/**
 * Calculates and stores the aggregate force exerted on every particle by all
 * other particles in array particle_t* particles.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smooth calculations.
 */
inline static void updateParticles(
		particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const double delta_t);

/**
 * Shows the state of the particles graphically.
 *
 * @param particles Information about every particle.
 */
static void showGraphics(
		particle_t* __restrict particles,
		const int N,
		const double circleRadius,
		const int circleColour);

/**
 * Writes current state of all particles to output file "result.gal".
 *
 * @param particles  Information about every particle.
 * @param brightness Array with brightness information about every particle.
 * @param N          The total number of particles.
 */
static int writeOutput(
		particle_t* __restrict particles,
		double* __restrict brightness,
		const int N);

/**
 * Main function
 *
 */
int main(int argc, char const *argv[]) {

	// Check proper number of input arguments
	if (argc != 6) {
		printf("%s\n", "Input error: Expected 5 input arguments");
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
	const int graphics = atoi(argv[5]); // Graphics on/off as 1/0

	// Define some constants
	const double G = 100.0/N; // Gravitational constant
	const double eps0 = 0.001; // Plummer sphere constant

	// Graphics
	const unsigned int windowSize = 1000;
	const float circleRadius = 0.0025f;
	const float circleColour = 0.0f;

	// Create array with all particles
	particle_t* particles = (particle_t*) malloc(N * sizeof(particle_t)); // TODO better malloc? array of pointers to structs?
	if (!particles) {
		// Program fail, exit
		printf("ERROR: Failed to malloc particles");
		return 1;
	}
	double* brightness = (double*) malloc(N * sizeof(double));

	// Read data
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
		simulate(particles, N, G, eps0, nsteps, delta_t);
	}

	// Write new state of particles to file
	if (writeOutput(particles, brightness, N))
		return 1;

	// Free memory
	free(particles);
	free(brightness);

	// Success
	return 0;
}

// Simulate the movement of the particles
void simulate(
		particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t) {

	unsigned int i;
	for (i = 0; i < nsteps; i++) {
		updateParticles(particles, N, G, eps0, delta_t);
	}
}

// Simulate the movement of the particles and show graphically
void simulateWithGraphics(
		particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t,
		const char* program,
		const unsigned int windowSize,
		const float circleRadius,
		const float circleColour) {

	// Initialize graphics handles
	InitializeGraphics((char*) program, windowSize, windowSize);
	SetCAxes(0,1);	// Color axis (so 0 = white, 1 = black)

	unsigned int i;
	for (i = 0; i < nsteps; i++) {
		updateParticles(particles, N, G, eps0, delta_t);
		showGraphics(particles, N, circleRadius, circleColour);
	}

	// Remove graphics handles
	FlushDisplay();
	CloseDisplay();
}

// Calculates force exerted on every particle
inline void updateParticles(particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const double delta_t) {

	unsigned int i, j; // Loop variables
	double r = 0.0, r_x = 0.0, r_y = 0.0; // r-vector
	double denom = 0.0; // Denominator

	// Loop for first particle and initialize acceleration to 0.0
	particles[0].a_x = 0.0;
	particles[0].a_y = 0.0;
	for (j = 1; j < N; j++) {
		particles[j].a_x = 0.0;
		particles[j].a_y = 0.0;
		// Calculate r-vector
		r_x = particles[0].x - particles[j].x;
		r_y = particles[0].y - particles[j].y;
		r = sqrt(r_x*r_x + r_y*r_y);
		// Calculate denominator
		denom = r + eps0;
		denom = 1/(denom*denom*denom); // 1 msec faster than using /denom below
		// Calculate acceleration (a_x, a_y == 0 at start due to calloc)
		particles[0].a_x += particles[j].mass*r_x*denom;
		particles[0].a_y += particles[j].mass*r_y*denom;
		// Calculate corresponding acceleration for other particle, using
		// Newton's third law
		particles[j].a_x -= particles[0].mass*r_x*denom;
		particles[j].a_y -= particles[0].mass*r_y*denom;
	}
	// Update velocity
	particles[0].v_x += -G*delta_t*particles[0].a_x;
	particles[0].v_y += -G*delta_t*particles[0].a_y;
	// Update position
	particles[0].x += delta_t*particles[0].v_x;
	particles[0].y += delta_t*particles[0].v_y;

	// Loop remaining particles, assuming acceleration is properly initialized
	for (i = 1; i < N; i++) {
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

// Show particles graphically
inline void showGraphics(
		particle_t* __restrict particles,
		const int N,
		const double circleRadius,
		const int circleColour) {

	ClearScreen();
	unsigned int i;
	for(i = 0; i < N; i++) {
		DrawCircle(particles[i].x, particles[i].y, 1, 1, circleRadius, circleColour);
	}
	Refresh();
	usleep(3000);	// TODO make variable fps
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
			printf("ERROR: Failed to read particle %d from input file\n", i);
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
