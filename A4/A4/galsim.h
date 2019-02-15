/**
 *	galsim.h
 *	Contains functions for simulating the interactions between particles
 *
 */

#pragma once
#include "graphics.h"
#include <math.h>

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
void simulateWithGraphics(
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
inline void updateParticles(
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
void showGraphics(
		particle_t* __restrict particles,
		const int N,
		const double circleRadius,
		const int circleColour);
