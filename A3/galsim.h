/**
 * Simulates galaxy movement in outer space.
 *
 * Original authors: Olof Björck, Gunnlaugur Geirsson
 *
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graphics.h"

#define BLOCK_SIZE 5
#define USE_BLOCKING 0

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
		const int nsteps, const double delta_t,
		const unsigned int blockSize);

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
		const float circleRadius, const float circleColour,
		const unsigned int blockSize);

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
		const double G, const double eps0,
		const unsigned int blockSize);

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
