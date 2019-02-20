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
		const double delta_t,
		const double theta_max);

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
		const double theta_max,
		const char* __restrict program,
		const unsigned int windowSize,
		const float circleRadius,
		const float circleColour);

/**
 *
 *
 */
void calculateForces(particle_t* particle,
          node_t* node,
          const double G,
          const double eps0,
          const double delta_t,
          const double theta_max,
	  		double* a_x, double* a_y);

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

void computeCenterOfMass(node_t* node);
void printParticles(particle_t* particles, int N);
void printTotalMass(particle_t*, int);

/**
 * Calculates and stores the aggregate force exerted on every particle by all
 * other particles in array particle_t* particles.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smooth calculations.
 */
inline void updateParticles(particle_t* particles,
		const int N,
		node_t* __restrict root,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max) {

	unsigned int i; // Loop iterators

	// Set acceleration to 0
	/*for (i = 0; i < N; i++) {
		particles[i].a_x = 0.0;
		particles[i].a_y = 0.0;
	}*/
	double a_x;
	double a_y;

	// Loop remaining particles, assuming acceleration is properly initialized
	for (i = 0; i < N; i++) {

		// Set acceleration to zero
		a_x = 0.0;
		a_y = 0.0;

		// Update acceleration
		calculateForces(&(particles[i]), root,
				G, eps0, delta_t, theta_max,
				&a_x, &a_y);

		// Update velocity
		particles[i].v_x += -G*delta_t*a_x;
		particles[i].v_y += -G*delta_t*a_y;

		// Update position
		particles[i].x += delta_t*particles[i].v_x;
		particles[i].y += delta_t*particles[i].v_y;

	}
}
