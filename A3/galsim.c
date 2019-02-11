// RUN BY:
// ./galsim 2 circles_N_2.gal 5 0.00001 1
// ./galsim 10 ellipse_N_00010.gal 5 0.00001 1
// time ./galsim 10000 input_data/ellipse_N_10000.gal 100 0.00001 1

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

#define BLOCK_SIZE 1
#define USE_BLOCKING 1

/**
 * Particle struct declaration.
 *
 */
typedef struct particle {
	double x, v_x; // x-position x and x-velocity vx
	double y, v_y; // y-position y and y-velocity vy
	double force_x, force_y; // Force exerted on particle
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
static int readData(particle_t* __restrict particles,
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
static void simulate(particle_t* __restrict particles, const int N,
		const double G, const double eps0,
		const int nsteps, const double delta_t);

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
static void simulateWithGraphics(particle_t* __restrict particles, const int N,
		const double G, const double eps0,
		const int nsteps, const double delta_t,
		const char* __restrict program, const unsigned int windowSize,
		const float circleRadius, const float circleColour);

/**
 * Calculates and stores the aggregate force exerted on every particle by all
 * other particles in array particle_t* particles.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smooth calculations.
 */
inline static void calculateForces(particle_t* __restrict particles, const int N,
		const double G, const double eps0);

/**
 * Updates the state of the particles with timestep delta_t. Does not compute
 * any forces; current forces particles.force_x and particles.force_y need
 * to be computed and set before using this function.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param delta_t   Timestep [seconds].
 */
inline static void updateParticles(particle_t* __restrict particles,
		const int N, const double delta_t);

/**
 * Shows the state of the particles graphically.
 *
 * @param particles Information about every particle.
 */
static void showGraphics(particle_t* __restrict particles, const int N,
		const double circleRadius, const int circleColour);

/**
 * Writes current state of all particles to output file "result.gal".
 *
 * @param particles  Information about every particle.
 * @param brightness Array with brightness information about every particle.
 * @param N          The total number of particles.
 */
static void writeOutput(particle_t* __restrict particles,
		double* __restrict brightness,
		const int N);

/*
   static void printParticles(particle_t* __restrict particles,
   double* __restrict brightness,
   const int N);
   */

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
	//particle_t particles[N]; // Use malloc? N can be large?
	particle_t* particles = (particle_t*)malloc(N * sizeof(particle_t)); // TODO better malloc? array of pointers to structs?
	if(!particles) {
		printf("ERROR: Failed to malloc particles");
		return 1;
	}
	double brightness[N];

	// Parameters for blocking loops
	unsigned int tempSize;	// Not const because blockSize may fail
	if(N % BLOCK_SIZE) {
		printf("ERROR: N particles not divisible by blocksize. No blocking used");
		tempSize = 1;
		return;
	} else {
		tempSize = BLOCK_SIZE;
	}
	const unsigned int blockSize = tempSize;	// const for performance

	// Read data
	if (readData(particles, brightness, filename, N))
		return 1;
	//printParticles(particles, N);

	// Simulate
	if(graphics) {
		// With graphics
		simulateWithGraphics(particles, N, G, eps0, nsteps, delta_t,
				program, windowSize, circleRadius, circleColour, blockSize);
	} else {
		// Movement only
		simulate(particles, N, G, eps0, nsteps, delta_t, blockSize);
	}
	//printParticles(particles, N);

	// Write new state of particles to file
	writeOutput(particles, brightness, N);

	// Free memory
	free(particles);
	return 0;
}

// Simulate the movement of the particles
void simulate(particle_t* __restrict particles, const int N,
		const double G, const double eps0,
		const int nsteps, const double delta_t,
		const unsigned int blockSize) {


	unsigned int i;
	for (i = 0; i < nsteps; i++) {
		calculateForces(particles, N, G, eps0, blockSize);
		updateParticles(particles, N, delta_t);
	}
}

// Simulate the movement of the particles and show graphically
void simulateWithGraphics(particle_t* __restrict particles, const int N,
		const double G, const double eps0,
		const int nsteps, const double delta_t,
		const char* program, const unsigned int windowSize,
		const float circleRadius, const float circleColour,
		const unsigned int blockSize) {

	// Initialize graphics handles
	InitializeGraphics(program, windowSize, windowSize);
	SetCAxes(0,1);	// Color axis (so 0 = white, 1 = black)

	unsigned int i;
	for (i = 0; i < nsteps; i++) {
		calculateForces(particles, N, G, eps0, blockSize);
		updateParticles(particles, N, delta_t);
		showGraphics(particles, N, circleRadius, circleColour);
	}

	// Remove graphics handles
	FlushDisplay();
	CloseDisplay();
}
// Calculates force exerted on every particle
inline void calculateForces(particle_t* __restrict particles, const int N,
		const double G, const double eps0, const unsigned int blockSize) {

	unsigned int i, j; // Loop variables
	double r = 0.0, r_x = 0.0, r_y = 0.0; // r-vector
	double denom = 0.0; // Denominator

	#if USE_BLOCKING

	unsigned int ii, jj;	// Block iterators

	for(i = 0; i < N; i++) {
		// Initialize forces
		particles[i].force_x = 0.0, particles[i].force_y = 0.0;
	}
	for(i = 0; i < N; i+=blockSize) {
		for(j = i; j < N; j+=blockSize) {
			for(ii = i; ii < i+blockSize; ii++) {
				for(jj = j; jj < j+blockSize; jj++) {
					// Calculate r-vector
					r_x = particles[ii].x - particles[jj].x;
					r_y = particles[ii].y - particles[jj].y;
					r = sqrt(r_x*r_x + r_y*r_y);
					// Calculate denominator
					denom = r + eps0;
					denom = 1/(denom*denom*denom); // 1 msec faster than using /demon below
					// Calculate force
					particles[ii].force_x += particles[jj].mass*r_x*denom;
					particles[ii].force_y += particles[jj].mass*r_y*denom;
					particles[jj].force_x -= particles[ii].mass*r_x*denom;
					particles[jj].force_y -= particles[ii].mass*r_y*denom;
				}
			}
		}
		for(ii = i; ii < i+blockSize; ii++) {
			particles[ii].force_x *= -G*particles[ii].mass;
			particles[ii].force_y *= -G*particles[ii].mass;
		}
	}

	#else

	// Initialize forces
	for (i = 0; i < N; i++) {
		particles[i].force_x = 0.0, particles[i].force_y = 0.0;
	}
	// Loop
	for (i = 0; i < N; i++) {
		for (j = i; j < N; j++) { // case i==j is no problem as r will be 0.
			// Calculate r-vector
			r_x = particles[i].x - particles[j].x;
			r_y = particles[i].y - particles[j].y;
			r = sqrt(r_x*r_x + r_y*r_y);
			// Calculate denominator
			denom = r + eps0;
			denom = 1/(denom*denom*denom); // 1 msec faster than using /demon below
			// Calculate force
			particles[i].force_x += particles[j].mass*r_x*denom;
			particles[i].force_y += particles[j].mass*r_y*denom;
			particles[j].force_x -= particles[i].mass*r_x*denom;
			particles[j].force_y -= particles[i].mass*r_y*denom;
		}
		particles[i].force_x *= -G*particles[i].mass;
		particles[i].force_y *= -G*particles[i].mass;
	}

	#endif
}

// Update the particles
inline void updateParticles(particle_t* __restrict particles, const int N,
		const double delta_t) {

	double a_x = 0.0, a_y = 0.0; // Acceleration
	// Loop over all particles
	unsigned int i;
	for (i = 0; i < N; i++) {
		// Calculate acceleration
		a_x = particles[i].force_x/particles[i].mass;
		a_y = particles[i].force_y/particles[i].mass;
		// Update velocity
		particles[i].v_x += delta_t*a_x;
		particles[i].v_y += delta_t*a_y;
		// Update position
		particles[i].x += delta_t*particles[i].v_x;
		particles[i].y += delta_t*particles[i].v_y;
	}
}

// Read data from file.
int readData(particle_t* __restrict particles,
		double* __restrict brightness,
		const char* filename, const int N) {

	// Open file
	FILE* fp = fopen(filename, "r");

	// Check file open went ok
	if (!fp) {
		printf("%s\n", "Error: couldn't open input file. Is it in directory?");
		return 1;
	}

	// Check file size is as expected
	fseek(fp, 0L, SEEK_END); // Seek end
	size_t fileSize = ftell(fp); // Get file size
	fseek(fp, 0L, SEEK_SET); // Reset seek to start of file
	if (fileSize != 6*N*sizeof(double)) { // File size not as expected?
		printf("%s\n", "Error: Input file size is not as expected. Is N correct?");
		return 1;
	}

	// Read file
	unsigned int i;
	for (i = 0; i < N; i++) {
		fread(&particles[i].x, sizeof(double), 1, fp);
		fread(&particles[i].y, sizeof(double), 1, fp);
		fread(&particles[i].mass, sizeof(double), 1, fp);
		fread(&particles[i].v_x, sizeof(double), 1, fp);
		fread(&particles[i].v_y, sizeof(double), 1, fp);
		fread(&brightness[i], sizeof(double), 1, fp);
	}

	// Close file
	if (fclose(fp)) {
		printf("%s\n", "Error: couldn't close input file.");
		return 1;
	}

	return 0;
}

// Show particles graphically
inline void showGraphics(particle_t* __restrict particles, const int N,
		const double circleRadius, const int circleColour) {

	ClearScreen();
	unsigned int i;
	for(i = 0; i < N; i++) {
		DrawCircle(particles[i].x, particles[i].y, 1, 1, circleRadius, circleColour);
	}
	Refresh();
	usleep(3000);	// TODO make variable fps
}

// Write current state of all particles to file
void writeOutput(particle_t* __restrict particles,
		double* __restrict brightness,
		const int N) {

	// Create file to write
	FILE* fp = fopen("result.gal", "w");

	// Write to file
	unsigned int i;
	for (i = 0; i < N; i++) {
		fwrite(&particles[i].x, sizeof(double), 1, fp);
		fwrite(&particles[i].y, sizeof(double), 1, fp);
		fwrite(&particles[i].mass, sizeof(double), 1, fp);
		fwrite(&particles[i].v_x, sizeof(double), 1, fp);
		fwrite(&particles[i].v_y, sizeof(double), 1, fp);
		fwrite(&brightness[i], sizeof(double), 1, fp);
	}

	// Close file
	fclose(fp);
}

/*
// Print particles (for debugging)
void printParticles(particle_t* __restrict particles,
double* __restrict brightness, const int N) {

unsigned int i;
for (i = 0; i < N; i++) {
printf("Particle %d:\n", i+1);
printf("\t(x, y) = (%.2lf, %.2lf)\n", particles[i].x, particles[i].y);
printf("\t(vx, vy) = (%.2lf, %.2lf)\n", particles[i].v_x, particles[i].v_y);
printf("\t(force_x, force_y) = (%.2lf, %.2lf)\n",
particles[i].force_x, particles[i].force_y);
printf("\tmass = %lf\n", particles[i].mass);
printf("\tbrightness = %lf\n", brightness[i]);
}
}
*/
