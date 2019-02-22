/**
 *	galsim.h
 *	Contains functions for simulating the interactions between particles
 *
 */

#pragma once
#include "modules.h"
#include "graphics.h"
#include <math.h>
#include "quadtree.h"
#include <pthread.h>

#define N_THREADS 4

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
void simulate(
		particles_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t,
		const double theta_max);

// Simulate the movement of the particles and show graphically
void simulateWithGraphics(
		particles_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t,
		const double theta_max,
		const char* program,
		const unsigned int windowSize,
		const float circleRadius,
		const float circleColour);
